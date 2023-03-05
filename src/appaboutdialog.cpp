#include "appaboutdialog.h"
#include "ui_appaboutdialog.h"

#include "version.h"

#include <QDateTime>
#include <QIcon>

AppAboutDialog::AppAboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AppAboutDialog)
{
    ui->setupUi(this);

    QIcon icon("://icons/beagle.svg");
    ui->appIconLabel->setPixmap(icon.pixmap(ui->appIconLabel->geometry().size()));

    this->setWindowTitle(tr("About %1").arg(APP_NAME));
    ui->appNameLabel->setText(ui->appNameLabel->text().arg(APP_NAME));
    ui->appVersionLabel->setText(ui->appVersionLabel->text().arg(APP_VERSION, QDateTime::fromSecsSinceEpoch(APP_TIME).toString("yyyy-MM-dd HH:MM")));
}

AppAboutDialog::~AppAboutDialog()
{
    delete ui;
}
