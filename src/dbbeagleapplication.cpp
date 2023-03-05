#include "dbbeagleapplication.h"
#include "mainwindow.h"

#include <QLibraryInfo>
#include <iostream>

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

    if(qsqlDb.isOpen())
        qsqlDb.close();
}

DBBeagleApplication& DBBeagleApplication::instance()
{
    return *app_;
}

void DBBeagleApplication::initialize_()
{
    setOrganizationName("DBBeagle");
    setApplicationName("DBBeagle");

    const QString localeName = QLocale::system().name();

    if (!qtTranslator_.load("qt_" + localeName,
                            QLibraryInfo::path(QLibraryInfo::TranslationsPath)))
    {
        std::cerr << "Failed to load Qt translation file" << std::endl;
    }
    installTranslator(&qtTranslator_);

    if (!appTranslator_.load("dbbeagle_" + localeName))
    {
        std::cerr << "Failed to load application translation file" << std::endl;
    }
    installTranslator(&appTranslator_);

    mainWindow_.reset(new MainWindow());
}

int DBBeagleApplication::exec()
{
    mainWindow_->show();
    return QApplication::exec();
}
