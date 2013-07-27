#include "mainwindow.h"
#include "dbbeagleapplication.h"
#include "dbconnectiondialog.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlField>
#include <QSqlQueryModel>
#include <QSortFilterProxyModel>
#include <QMessageBox>
#include <QProgressDialog>
#include <QKeyEvent>
#include <QClipboard>
#include <QtDebug>

////////////////////////////////////////////////////////////////////////////////
// MainWindow
//
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setupUi(this);

    sqlDialectAdaptor_.reset(new OraSQLDialectAdaptor());

    searchResults_ = new SearchResultsTableModel(this);

    sortProxyModel_ = new QSortFilterProxyModel(this);
    sortProxyModel_->setSourceModel(searchResults_);
    sortProxyModel_->setDynamicSortFilter(true);

    searchResultsTableView->setModel(sortProxyModel_);
    searchResultsTableView->setSortingEnabled(true);
    searchResultsTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    searchResultsTableView->horizontalHeader()->setStretchLastSection(true);
    searchResultsTableView->verticalHeader()->hide();
    searchResultsTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    searchResultsTableView->installEventFilter(this);

    sqlQueryModel_ = new QSqlQueryModel(this);
    queryTableView->setModel(sqlQueryModel_);
    queryTableView->installEventFilter(this);

    connect(exitAction, SIGNAL(triggered()), SLOT(close()));

    connect(connectToDatasourceAction, SIGNAL(triggered()), SLOT(connectToDB_()));

    connect(searchPushButton, SIGNAL(clicked()), SLOT(search_()));
    connect(searchResultsTableView, SIGNAL(doubleClicked(const QModelIndex& )), SLOT(resultsItemActivated_(const QModelIndex&)));

    connect(searchInFoundAction, SIGNAL(triggered()), SLOT(copyTableNamesFromResults_()));

    connect(executeQueryPushButton, SIGNAL(clicked()), SLOT(executeQuery_()));
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::connectToDB_()
{
    if (DBBeagleApplication::instance()->pDb->isOpen())
    {
        DBBeagleApplication::instance()->pDb->close();
        updateConnectionStatus_();
    }

    DBConnectionDialog dbDlg(this);
    dbDlg.readSettings();

    if(dbDlg.exec() == QDialog::Accepted)
    {
        DBBeagleApplication::instance()->pDb->setHostName(dbDlg.getHost());
        DBBeagleApplication::instance()->pDb->setDatabaseName(dbDlg.getDatabase());
        DBBeagleApplication::instance()->pDb->setUserName(dbDlg.getUser());
        DBBeagleApplication::instance()->pDb->setPassword(dbDlg.getPassword());

        bool ok = DBBeagleApplication::instance()->pDb->open();
        if(!ok)
        {
            QMessageBox::warning(this,
                                 tr("Connection Error"),
                                 tr("Could not connect to the datasource.\nThe reason was: \"%1\".").arg(DBBeagleApplication::instance()->pDb->lastError().text())
                                 );
            Ui_MainWindow::statusBar->showMessage(DBBeagleApplication::instance()->pDb->lastError().text());
        }
        else
        {
            dbDlg.writeSettings();
            Ui_MainWindow::statusBar->showMessage(tr("Connected to %1").arg(dbDlg.getDatabase()));
        }

        updateConnectionStatus_();
    }
}

void MainWindow::updateConnectionStatus_()
{
    bool dbOpen = DBBeagleApplication::instance()->pDb->isOpen();
    for (int i = 0; i < searchControlsLayout->count(); ++i)
    {
        QWidget* w = searchControlsLayout->itemAt(i)->widget();
        if(w)
            w->setEnabled(dbOpen);
    }
    searchResultsTableView->setEnabled(dbOpen);

    for (int i = 0; i < sqlControlsLayout->count(); ++i)
    {
        QWidget* w = sqlControlsLayout->itemAt(i)->widget();
        if(w)
            w->setEnabled(dbOpen);
    }
    queryTableView->setEnabled(dbOpen);
}

void MainWindow::show()
{
    updateConnectionStatus_();
    QMainWindow::show();
}

void MainWindow::search_()
{
    if (tablesLineEdit->text().isEmpty() && !excludeTablesCheckBox->isChecked())
    {
        QMessageBox::information(this,
                             tr("Usage information"),
                             tr("You have not defined a set of tables to search in. Please, enter a comma-separated list of table names or use '*' sign to indicate all tables.")
                             );
        return;
    }

    QSet<QString> availableTables;
    foreach (QString str, DBBeagleApplication::instance()->pDb->tables())
        availableTables.insert(str.toLower());



    if (availableTables.isEmpty())
    {
        QMessageBox::information(this,
                             tr("Search"),
                             tr("The database does not contain any tables.")
                             );
        return;

    }

    QSet<QString> tablesToSearch;

    if (!excludeTablesCheckBox->isChecked()) {
        if (tablesLineEdit->text() == "*")
        {
            tablesToSearch = availableTables;
        }
        else
        {
            tablesToSearch =
                    QSet<QString>::fromList(
                            tablesLineEdit->text().toLower().split(QRegExp("\\s*\\,\\s*"), QString::SkipEmptyParts)
                            );
            foreach (QString str, tablesToSearch)
            {
                if (!availableTables.contains(str))
                {
                    QMessageBox::warning(this,
                                         tr("Search"),
                                         tr("The database does not contain a table named \"%1\".").arg(str)
                                         );
                    return;
                }
            }
        }
    } else {
        QSet<QString> tablesToExclude =
                QSet<QString>::fromList(
                        tablesLineEdit->text().toLower().split(QRegExp("\\s*\\,\\s*"), QString::SkipEmptyParts)
                        );
        tablesToSearch = availableTables - tablesToExclude;
    }

    if (tablesToSearch.isEmpty())
    {
        QMessageBox::warning(this,
                             tr("Search"),
                             tr("There are no tables to search in.")
                             );
        return;
    }

    if (valueLineEdit->text().isEmpty())
    {
        QMessageBox::warning(this,
                             tr("Search"),
                             tr("Please, enter a value to search.")
                             );
        return;
    }

    searchResults_->clearResults();

    QProgressDialog progress(tr("Searching..."), tr("Abort Search"), 0, tablesToSearch.count(), this);
    progress.setWindowModality(Qt::WindowModal);
    int searchCount = 0;
    foreach (QString curTable, tablesToSearch)
    {
        progress.setValue(searchCount);
        if (progress.wasCanceled())
            break;
        searchCount++;

        QString queryStr = QString("select * from %1").arg(curTable);
        QSqlQuery sqlQuery(sqlDialectAdaptor_->addSQLLimitClause(queryStr, 0), *(DBBeagleApplication::instance()->pDb.get()));
        sqlQuery.setForwardOnly(true);
        QSqlRecord rec = sqlQuery.record();
        if (rec.isEmpty())
        {
            qDebug() << tr("Encountered a record with no fields.");
            qDebug() << tr("the query was \"%1\"").arg(sqlQuery.lastQuery());
            continue;
        }

        for (int i = 0; i < rec.count(); i++)
        {
            QVariant val(valueLineEdit->text());
            QVariant defVal(QString(""));
            defVal.convert(rec.field(i).type());
            val.convert(rec.field(i).type());
            if (val == defVal && val.toString() != valueLineEdit->text())
            {
                rec.remove(i);
                i--;
                continue;
            }
        }
        if (rec.isEmpty())
        {
            qDebug() << tr("No fields in %1 match the type of the value.").arg(curTable);
            continue;
        }

        queryStr += QString(" where %1=?").arg(rec.fieldName(0));
        for (int i = 1; i < rec.count(); i++)
        {
            queryStr += QString(" or %1=?").arg(rec.fieldName(i));
        }
        queryStr = sqlDialectAdaptor_->addSQLLimitClause(queryStr, 1);

        if(!sqlQuery.prepare(queryStr))
        {
            qDebug() << tr("Failed to prepare the query \"%1\".").arg(queryStr);
            qDebug() << sqlQuery.lastError();
            continue;
        }

        for (int i = 0; i < rec.count(); i++)
        {
            sqlQuery.bindValue(i, QVariant(valueLineEdit->text()));
        }

        if (!sqlQuery.exec())
            qDebug() << tr("Failed to execute the query \"%1\".").arg(sqlQuery.lastQuery());
            qDebug() << sqlQuery.lastError();

        if (sqlQuery.next())
        {
            qDebug() << tr("Found %1 in %2").arg(valueLineEdit->text(), curTable);

            QSqlRecord	rec = sqlQuery.record();
            QStringList columns;
            for (int i = 0 ; i < rec.count(); i++)
            {
                if(sqlQuery.value(i) == QVariant(valueLineEdit->text()))
                    columns.push_back(rec.fieldName(i));
            }
            searchResults_->addResult(QPair<QString, QStringList>(curTable, columns));
        }

    }
    progress.setValue(tablesToSearch.count());

}

void MainWindow::resultsItemActivated_( const QModelIndex& index )
{
    QPair<QString, QStringList> p = searchResults_->rowAt(
            dynamic_cast<QAbstractProxyModel*>(searchResultsTableView->model())->mapToSource(index)
            );
    QString query = QString("select * from %1").arg(p.first);
    query += QString(" where %1='%2'").arg(p.second.at(0), valueLineEdit->text());
    for (int i = 1; i < p.second.count(); i++)
        query += QString(" or %1='%2'").arg(p.second.at(i), valueLineEdit->text());
    queryLineEdit->setText(query);
    executeQuery_();
    tabWidget->setCurrentIndex(1);
}

void MainWindow::executeQuery_()
{
    sqlQueryModel_->setQuery(queryLineEdit->text(), *(DBBeagleApplication::instance()->pDb.get()));
    if (sqlQueryModel_->lastError().isValid())
    {
        QMessageBox::warning(this,
                             tr("SQL Query"),
                             tr("Error while executing the query:\n\"%1\".").arg(sqlQueryModel_->lastError().text())
                             );
        Ui_MainWindow::statusBar->showMessage(sqlQueryModel_->lastError().text());
    }
}

void MainWindow::copyTableNamesFromResults_()
{
    QStringList l;
    QList< QPair<QString, QStringList> > rl = searchResults_->list();
    QPair<QString, QStringList>  p;
    foreach (p, rl)
        l.append(p.first);

    if (!l.isEmpty())
        tablesLineEdit->setText(l.join(", "));
}

bool MainWindow::eventFilter(QObject *object, QEvent *event)
{
    if ((object == queryTableView || object == searchResultsTableView) &&
        event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if (keyEvent->modifiers() & Qt::ControlModifier &&
            keyEvent->key() == Qt::Key_C) {
            QTableView* tv = dynamic_cast<QTableView*>(object);
            if (!tv)
                return false;

            QStringList sl;
            QModelIndexList idxList = tv->selectionModel()->selectedIndexes();
            QModelIndexList::const_iterator idxI= idxList.begin();
            for (int i = 0; idxI != idxList.end() && i < 100; i++, idxI++) {
                sl.append(idxI->data().toString());
            }
            if (!sl.isEmpty())
                QApplication::clipboard()->setText(sl.join(", "));

            return true;
        } else
            return false;
    }
    return false;
}

////////////////////////////////////////////////////////////////////////////////
// SearchResultsTableModel
//
SearchResultsTableModel::SearchResultsTableModel(QObject *parent) : QAbstractTableModel(parent)
{
}

int SearchResultsTableModel::rowCount(const QModelIndex &index) const
{
    Q_UNUSED(index);
    return listOfSearchResults_.size();
}

int SearchResultsTableModel::columnCount(const QModelIndex &index) const
{
    Q_UNUSED(index);
    return 2;
}

QVariant SearchResultsTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= listOfSearchResults_.size() || index.row() < 0)
        return QVariant();

    if (role == Qt::DisplayRole) {
        QPair<QString, QStringList> pair = listOfSearchResults_.at(index.row());

        if (index.column() == 0)
            return pair.first;
        else if (index.column() == 1)
            return pair.second.join(", ");
    }
    return QVariant();
}

QVariant SearchResultsTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        switch (section) {
            case 0:
                return tr("Table");

            case 1:
                return tr("Columns");

            default:
                return QVariant();
        }
    }
    return QVariant();
}

void SearchResultsTableModel::addResult(const QPair<QString, QStringList>& r)
{
    beginInsertRows(QModelIndex(), listOfSearchResults_.count(), listOfSearchResults_.count());
    listOfSearchResults_.push_back(r);
    endInsertRows();
}

void SearchResultsTableModel::clearResults()
{
    beginRemoveRows(QModelIndex(), 0, listOfSearchResults_.count());
    listOfSearchResults_.clear();
    endRemoveRows();
}

QPair<QString, QStringList> SearchResultsTableModel::rowAt(const QModelIndex &index) const
{
    QPair<QString, QStringList> result;

    if (!index.isValid())
        return result;

    if (index.row() >= listOfSearchResults_.size() || index.row() < 0)
        return result;

    result = listOfSearchResults_.at(index.row());

    return result;
}

QList< QPair<QString, QStringList> > SearchResultsTableModel::list() const
{
    QList< QPair<QString, QStringList> > result;

    result = listOfSearchResults_;

    return result;
}
