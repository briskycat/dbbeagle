#include "mysqldialectadaptor.h"

MySQLDialectAdaptor::MySQLDialectAdaptor()
{
}

QString MySQLDialectAdaptor::addSQLLimitClause(const QString &src, unsigned long limit)
{
    QString res;
    res = src + QString(" limit %1").arg(limit);

    return res;
}
