#include <QSettings>
#include <QMessageBox>
#include <QCoreApplication>
#include "dialogsettings.h"
#include "ui_dialogsettings.h"

DialogSettings::DialogSettings(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::DialogSettings)
{
	ui->setupUi(this);
	QStringList drivers = QSqlDatabase::drivers();
	ui->comboBox_driver->addItems(drivers);
	Read_cfg();
}

DialogSettings::~DialogSettings()
{
	delete ui;
}

void DialogSettings::changeEvent(QEvent *e)
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
void DialogSettings::Read_cfg()
{
	QSettings settings(QCoreApplication::applicationFilePath()+".ini",QSettings::IniFormat);
	QString strDriver = settings.value("connection/Driver","QMYSQL").toString();
	QString strHostName = settings.value("connection/HostName","localhost").toString();
	int nPort = settings.value("connection/Port",3306).toInt();
	QString strUserName = settings.value("connection/UserName","").toString();
	QString strPassword = settings.value("connection/Password","").toString();
	QString strDatabaseName = settings.value("connection/DatabaseName","parkinglots").toString();

	ui->comboBox_driver->setCurrentIndex(ui->comboBox_driver->findText(strDriver));
	ui->lineEdit_Database->setText(strDatabaseName);
	ui->lineEdit_Host->setText(strHostName);
	ui->lineEdit_Password->setText(strPassword);
	ui->lineEdit_Port->setText(QString("%1").arg(nPort));
	ui->lineEdit_Username->setText(strUserName);

	QString strServer = settings.value("Service/BackendSvr","localhost:23456").toString();
	ui->lineEdit_BkServer->setText(strServer);
}
void DialogSettings::Save_cfg()
{
	QSettings settings(QCoreApplication::applicationFilePath()+".ini",QSettings::IniFormat);

	QString strDriver = ui->comboBox_driver->currentText();
	QString strHost = ui->lineEdit_Host->text();
	QString strPort = ui->lineEdit_Port->text();
	QString strDB = ui->lineEdit_Database->text();
	QString strUsr = ui->lineEdit_Username->text();
	QString strPassword = ui->lineEdit_Password->text();

	settings.setValue("connection/Driver",strDriver);
	settings.setValue("connection/HostName",strHost);
	settings.setValue("connection/Port",strPort);
	settings.setValue("connection/UserName",strUsr);
	settings.setValue("connection/Password",strPassword);
	settings.setValue("connection/DatabaseName",strDB);


	QString strServer = ui->lineEdit_BkServer->text();
	settings.setValue("Service/BackendSvr",strServer);

}

void DialogSettings::on_pushButton_ok_clicked()
{
	Save_cfg();
	hide();
}
void DialogSettings::on_pushButton_cancel_clicked()
{
	hide();

}
