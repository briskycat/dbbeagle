#ifndef APPABOUTDIALOG_H
#define APPABOUTDIALOG_H

#include <QDialog>

namespace Ui {
class AppAboutDialog;
}

class AppAboutDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit AppAboutDialog(QWidget *parent = nullptr);
    ~AppAboutDialog();
    
private:
    Ui::AppAboutDialog *ui;
};

#endif // APPABOUTDIALOG_H
