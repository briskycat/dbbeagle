#include "dbconnectiondialog.h"

#include <QSettings>

DBConnectionDialog::DBConnectionDialog(const QStringList &dbDrivers, QWidget *parent) :
    QDialog(parent){
    setupUi(this);

    dbDriverComboBox->clear();
    dbDriverComboBox->addItems(dbDrivers);
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
    int dbDriverIdx = dbDriverComboBox->findText(s.value("driver", "").toString());
    if(dbDriverIdx == -1)
        dbDriverIdx = 0;
    dbDriverComboBox->setCurrentIndex(dbDriverIdx);
    hostLineEdit->setText(s.value("host", "").toString());
    databaseLineEdit->setText(s.value("database", "").toString());
    userLineEdit->setText(s.value("user", "").toString());
    s.endGroup();
}

void DBConnectionDialog::writeSettings()
{
    QSettings s;
    s.beginGroup("lastdatasource");
    s.setValue("driver", getDriver());
    s.setValue("host", getHost());
    s.setValue("database", getDatabase());
    s.setValue("user", getUser());
    s.endGroup();
}
