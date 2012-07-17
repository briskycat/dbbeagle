#ifndef DBCONNECTIONDIALOG_H
#define DBCONNECTIONDIALOG_H

#include "ui_dbconnectiondialog.h"

class DBConnectionDialog : public QDialog, private Ui::DBConnectionDialog {
    Q_OBJECT
public:
    DBConnectionDialog(QWidget *parent = 0);

    QString getHost() { return hostLineEdit->text(); }
    QString getDatabase() { return databaseLineEdit->text(); }
    QString getUser() { return userLineEdit->text(); }
    QString getPassword() { return passwordLineEdit->text(); }

    void readSettings();
    void writeSettings();

protected:
    void changeEvent(QEvent *e);
};

#endif // DBCONNECTIONDIALOG_H
