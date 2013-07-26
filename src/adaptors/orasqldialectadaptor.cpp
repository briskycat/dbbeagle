#include "orasqldialectadaptor.h"

OraSQLDialectAdaptor::OraSQLDialectAdaptor()
{
}

QString OraSQLDialectAdaptor::addSQLLimitClause(const QString &src, unsigned long limit)
{
    QString res;

    // limit = (limit == 0 ? 1 : limit);
    if(src.contains(" where ",  Qt::CaseInsensitive)) {
        res = src + QString(" and rownum <= %1").arg(limit);
    } else {
        res = src + QString(" where rownum <= %1").arg(limit);
    }

    return res;
}
