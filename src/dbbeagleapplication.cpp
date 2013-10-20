#include "dbbeagleapplication.h"
#include "mainwindow.h"

#include <QSqlDatabase>
#include <QLibraryInfo>

DBBeagleApplication* DBBeagleApplication::app_ = nullptr;

DBBeagleApplication::DBBeagleApplication(int& argc, char* argv[]) :
    QApplication(argc, argv)
{
    initialize_();
    app_ = this;
}

DBBeagleApplication::~DBBeagleApplication()
{
    app_ = nullptr;

    if(pDb.data())
        pDb->close();
}

DBBeagleApplication* DBBeagleApplication::instance()
{
    return app_;
}

void DBBeagleApplication::initialize_()
{
    setOrganizationName("DBBeagle");
    setApplicationName("DBBeagle");

    qtTranslator_.load("qt_" + QLocale::system().name(),
                      QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    installTranslator(&qtTranslator_);

    appTranslator_.load("dbbeagle_" + QLocale::system().name());
    installTranslator(&appTranslator_);

    pDb.reset(new QSqlDatabase(QSqlDatabase::addDatabase(dbDriver())));

    mainWindow_.reset(new MainWindow());
}

QString DBBeagleApplication::dbDriver()
{
    // return QString("QODBC");
    return QString("QOCI");
}

int DBBeagleApplication::exec()
{
    mainWindow_->show();
    return QApplication::exec();
}
