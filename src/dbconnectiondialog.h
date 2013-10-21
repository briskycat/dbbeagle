#ifndef DBCONNECTIONDIALOG_H
#define DBCONNECTIONDIALOG_H

#include "ui_dbconnectiondialog.h"

class DBConnectionDialog : public QDialog, private Ui::DBConnectionDialog {
    Q_OBJECT
public:
    DBConnectionDialog(QStringList const& dbDrivers, QWidget *parent = nullptr);

    QString getDriver() const { return dbDriverComboBox->currentText(); }
    QString getHost() const { return hostLineEdit->text(); }
    QString getDatabase() const { return databaseLineEdit->text(); }
    QString getUser() const { return userLineEdit->text(); }
    QString getPassword() const { return passwordLineEdit->text(); }

    void readSettings();
    void writeSettings();

protected:
    void changeEvent(QEvent *e);
};

#endif // DBCONNECTIONDIALOG_H
