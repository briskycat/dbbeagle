#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include "adaptors/orasqldialectadaptor.h"
#include "adaptors/mysqldialectadaptor.h"

#include <QStyledItemDelegate>
#include <unordered_map>
#include <functional>

class QSortFilterProxyModel;
class SearchResultsTableModel;
class QSqlQueryModel;

///////////////////////////////////////////////////////////////////////////////
// MainWindow
//
class MainWindow : public QMainWindow, private Ui::MainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);

    void show();

public slots:
    void showAboutApp();

protected:
    void changeEvent(QEvent *e) override;

    bool eventFilter(QObject *obj, QEvent *event) override;

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

    std::unique_ptr<SQLDialectAdaptor> sqlDialectAdaptor_;

    const std::unordered_map<std::string, std::function<SQLDialectAdaptor*()>> dialectAdaptors_ = {
                                {"QOCI", SQLDialectAdaptor::create<OraSQLDialectAdaptor>},
                                {"QMYSQL", SQLDialectAdaptor::create<MySQLDialectAdaptor>}
                            };

};

///////////////////////////////////////////////////////////////////////////////
// SearchResultsTableModel
//
class SearchResultsTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    typedef QPair<QString, QStringList> SearchResultType;
    typedef QList<SearchResultType> ListOfSearchResultsType;

    SearchResultsTableModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &index) const override;
    int columnCount(const QModelIndex &index) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    void addResult(const SearchResultType& r);
    void clearResults();
    SearchResultType rowAt(const QModelIndex &index) const;
    ListOfSearchResultsType list() const;

private:
    ListOfSearchResultsType listOfSearchResults_;
};

////////////////////////////////////////////////////////////////////////////////
class QueryViewItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    using QStyledItemDelegate::QStyledItemDelegate;

    virtual QString	displayText(const QVariant & value, const QLocale & locale) const override;
};

#endif // MAINWINDOW_H
