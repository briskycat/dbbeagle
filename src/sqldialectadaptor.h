#ifndef SQLDIALECTADAPTOR_H
#define SQLDIALECTADAPTOR_H

#include <QString>

class SQLDialectAdaptor
{
public:
    virtual ~SQLDialectAdaptor(){}

    virtual QString addSQLLimitClause(const QString& src, unsigned long limit) = 0;

protected:
    SQLDialectAdaptor() {}
};

#endif // SQLDIALECTADAPTOR_H
