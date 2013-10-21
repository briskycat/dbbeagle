#include "dbbeagleapplication.h"
#include "mainwindow.h"

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

    if(dbConnection.isOpen())
        dbConnection.close();
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

    mainWindow_.reset(new MainWindow());
}

int DBBeagleApplication::exec()
{
    mainWindow_->show();
    return QApplication::exec();
}
