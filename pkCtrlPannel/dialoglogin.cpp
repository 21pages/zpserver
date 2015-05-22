#include <QSettings>
#include <QMessageBox>
#include "dialoglogin.h"
#include "ui_dialoglogin.h"

DialogLogin::DialogLogin(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::DialogLogin)
{
	ui->setupUi(this);
	Read_cfg();
}

DialogLogin::~DialogLogin()
{
	delete ui;
}

void DialogLogin::changeEvent(QEvent *e)
{
	QDialog::changeEvent(e);
	switch (e->type()) {
	case QEvent::LanguageChange:
		ui->retranslateUi(this);
		break;
	default:
		break;
	}
}
void DialogLogin::Read_cfg()
{
	QSettings settings(QCoreApplication::applicationFilePath()+".ini",QSettings::IniFormat);
	QString strUserName = settings.value("login/UserName","").toString();
	QString strPassword = settings.value("login/Password","").toString();
	ui->lineEdit_username->setText(strPassword);
	ui->lineEdit_password->setText(strUserName);
}
void DialogLogin::Save_cfg()
{
	QSettings settings(QCoreApplication::applicationFilePath()+".ini",QSettings::IniFormat);
	QString strUsr = ui->lineEdit_username->text();
	QString strPassword = ui->lineEdit_password->text();
	settings.setValue("login/UserName",strUsr);
	settings.setValue("login/Password",strPassword);
}
void DialogLogin::on_buttonBox_accepted()
{
	Save_cfg();
	hide();
}
