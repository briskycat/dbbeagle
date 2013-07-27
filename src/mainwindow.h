#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include "adaptors/orasqldialectadaptor.h"

class QSortFilterProxyModel;
class SearchResultsTableModel;
class QSqlQueryModel;

///////////////////////////////////////////////////////////////////////////////
// MainWindow
//
class MainWindow : public QMainWindow, private Ui::MainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);

    void show();

public slots:
    void showAboutApp();

protected:
    void changeEvent(QEvent *e);

    bool eventFilter(QObject *obj, QEvent *event);

private slots:
    void connectToDB_();
    void updateConnectionStatus_();

    void search_();
    void resultsItemActivated_( const QModelIndex& index );
    void copyTableNamesFromResults_();

    void executeQuery_();

private:
    QSortFilterProxyModel* sortProxyModel_;
    SearchResultsTableModel* searchResults_;

    QSqlQueryModel* sqlQueryModel_;

    std::auto_ptr<SQLDialectAdaptor> sqlDialectAdaptor_;
};

///////////////////////////////////////////////////////////////////////////////
// SearchResultsTableModel
//
class SearchResultsTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    SearchResultsTableModel(QObject *parent=0);

    int rowCount(const QModelIndex &index) const;
    int columnCount(const QModelIndex &index) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    void addResult(const QPair<QString, QStringList>& r);
    void clearResults();
    QPair<QString, QStringList> rowAt(const QModelIndex &index) const;
    QList< QPair<QString, QStringList> > list() const;

private:
    QList< QPair<QString, QStringList> > listOfSearchResults_;
};


#endif // MAINWINDOW_H
