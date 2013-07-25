#include "dbbeagleapplication.h"

#include <QSqlDatabase>

DBBeagleApplication::DBBeagleApplication(int& argc, char** argv) :
    QApplication(argc, argv)
{
}

DBBeagleApplication::~DBBeagleApplication()
{
    if(pDb.get())
        pDb->close();
}

DBBeagleApplication* DBBeagleApplication::instance()
{
    return (static_cast<DBBeagleApplication*>(QCoreApplication::instance()));
}

void DBBeagleApplication::initialize()
{
    setOrganizationName("DBBeagle");
    setApplicationName("DBBeagle");

    instance()->pDb.reset(new QSqlDatabase(QSqlDatabase::addDatabase(dbDriver())));
}

QString DBBeagleApplication::dbDriver()
{
    return QString("QODBC");
}
