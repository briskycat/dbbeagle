#ifndef ORASQLDIALECTADAPTOR_H
#define ORASQLDIALECTADAPTOR_H

#include "sqldialectadaptor.h"

class OraSQLDialectAdaptor : public SQLDialectAdaptor
{
public:
    OraSQLDialectAdaptor();

    virtual QString addSQLLimitClause(const QString& src, unsigned long limit);
};

#endif // ORASQLDIALECTADAPTOR_H
