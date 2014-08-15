#ifndef SQLDIALECTADAPTOR_H
#define SQLDIALECTADAPTOR_H

#include <QString>

class SQLDialectAdaptor
{
public:
    virtual ~SQLDialectAdaptor(){}

    virtual QString addSQLLimitClause(const QString& src, unsigned long limit) = 0;

    template<typename Adaptor>
    static Adaptor* create() { return Adaptor::create(); }

protected:
    SQLDialectAdaptor() {}
};

#endif // SQLDIALECTADAPTOR_H
