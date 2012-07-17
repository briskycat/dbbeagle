#include "dbconnectiondialog.h"

#include <QSettings>

DBConnectionDialog::DBConnectionDialog(QWidget *parent) :
    QDialog(parent){
    setupUi(this);
}

void DBConnectionDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}

void DBConnectionDialog::readSettings()
{
    QSettings s;
    s.beginGroup("lastdatasource");
    hostLineEdit->setText(s.value("host", "").toString());
    databaseLineEdit->setText(s.value("database", "").toString());
    userLineEdit->setText(s.value("user", "").toString());
    s.endGroup();
}

void DBConnectionDialog::writeSettings()
{
    QSettings s;
    s.beginGroup("lastdatasource");
    s.setValue("host", getHost());
    s.setValue("database", getDatabase());
    s.setValue("user", getUser());
    s.endGroup();
}
