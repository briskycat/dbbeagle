#include "dbbeagleapplication.h"
#include "mainwindow.h"

#include <cstdlib>
#include <QSqlDatabase>
#include <QTranslator>
#include <QLibraryInfo>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    DBBeagleApplication app(argc, argv);

    QTranslator qtTranslator;
    qtTranslator.load("qt_" + QLocale::system().name(),
                      QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    app.installTranslator(&qtTranslator);

    QTranslator appTranslator;
    appTranslator.load("dbbeagle_" + QLocale::system().name());
    app.installTranslator(&appTranslator);

    if(!QSqlDatabase::drivers().contains(DBBeagleApplication::dbDriver()))
    {
        QMessageBox::critical(0,
                             DBBeagleApplication::tr("Initialization Error"),
                             DBBeagleApplication::tr("The database driver '%1' is not available").arg(DBBeagleApplication::dbDriver())
                             );
        return EXIT_FAILURE;
    }

    DBBeagleApplication::initialize();

    MainWindow w;
    w.show();

    return app.exec();
}
