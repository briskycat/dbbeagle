#ifndef DBEAGLEAPPLICATION_H
#define DBEAGLEAPPLICATION_H

#include <QApplication>
#include <QSqlDatabase>
#include <QTranslator>

class MainWindow;

class DBBeagleApplication : public QApplication
{
    Q_OBJECT
public:
    DBBeagleApplication(int& argc, char *argv[]);
    ~DBBeagleApplication();

    static DBBeagleApplication& instance();

    int exec();

signals:

public:
    QSqlDatabase qsqlDb;

public slots:

private:
    DBBeagleApplication(DBBeagleApplication const&) = delete;
    void operator=(DBBeagleApplication const&) = delete;

    void initialize_();

private:
    static DBBeagleApplication* app_;

    QScopedPointer<MainWindow> mainWindow_;

    QTranslator qtTranslator_;
    QTranslator appTranslator_;

};

#endif // DBEAGLEAPPLICATION_H
