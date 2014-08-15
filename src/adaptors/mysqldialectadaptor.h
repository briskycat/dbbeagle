#ifndef MYSQLDIALECTADAPTOR_H
#define MYSQLDIALECTADAPTOR_H

#include "sqldialectadaptor.h"

class MySQLDialectAdaptor : public SQLDialectAdaptor
{
public:
    MySQLDialectAdaptor();

    virtual QString addSQLLimitClause(const QString& src, unsigned long limit);

    static MySQLDialectAdaptor* create() { return new MySQLDialectAdaptor(); }
};

#endif // MYSQLDIALECTADAPTOR_H
