#include "dbbeagleapplication.h"

#include <cstdlib>
#include <QSqlDatabase>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    DBBeagleApplication app(argc, argv);

    if(!QSqlDatabase::drivers().contains(DBBeagleApplication::dbDriver()))
    {
        QMessageBox::critical(0,
                             DBBeagleApplication::tr("Initialization Error"),
                             DBBeagleApplication::tr("The database driver \"%1\" is not available").arg(DBBeagleApplication::dbDriver())
                             );
        return EXIT_FAILURE;
    }

    return app.exec();
}
