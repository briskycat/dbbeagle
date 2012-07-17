#ifndef DBEAGLEAPPLICATION_H
#define DBEAGLEAPPLICATION_H

#include <QApplication>
#include <memory>

class QSqlDatabase;

class DBBeagleApplication : public QApplication
{
    Q_OBJECT
public:
    DBBeagleApplication(int& argc, char** argv);
    ~DBBeagleApplication();

    static DBBeagleApplication* instance();
    static void initialize();
    static QString dbDriver();

signals:

public:
    std::auto_ptr<QSqlDatabase> pDB;

public slots:

};

#endif // DBEAGLEAPPLICATION_H
