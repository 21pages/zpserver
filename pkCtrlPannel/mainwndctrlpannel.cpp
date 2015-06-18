#include <QMessageBox>
#include <QDialog>
#include <QSettings>
#include <QSqlError>
#include <QSqlQuery>
#include <QFileDialog>
#include <QFileInfo>
#include "mainwndctrlpannel.h"
#include "ui_mainwndctrlpannel.h"
#include "./pklts_ctrl/pklts_methods.h"
#define PKLTS_VIEW ("PKLTS_VIEW")

using namespace ParkinglotsSvr;

mainwndCtrlPannel::mainwndCtrlPannel(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::mainwndCtrlPannel)
{
	ui->setupUi(this);
	m_nTimer = startTimer(2000);
	m_pSettingsDlg = new DialogSettings(this);
	m_pLoginDlg = new DialogLogin(this);
	m_pModel_Summary_PK = new QColorIconSqlModel(this);  ui->listView_sum_pklts->setModel(m_pModel_Summary_PK);
	m_pModel_Summary_MAC = new QColorIconSqlModel(this); ui->listView_sum_macs->setModel(m_pModel_Summary_MAC);
	m_pModel_Summary_DEV = new QColorIconSqlModel(this); ui->listView_sum_devices->setModel(m_pModel_Summary_DEV);
	m_pModel_Detail = new QColorIconSqlModel(this);      ui->tableView_detailed->setModel(m_pModel_Detail);
	m_pModel_EvtHis = new QColorIconSqlModel(this);      ui->tableView_events->setModel(m_pModel_EvtHis);
	m_pModel_MacHis = new QColorIconSqlModel(this);      ui->tableView_macevt->setModel(m_pModel_MacHis);
	m_pModel_Express_DEV = new QColorIconSqlModel(this); ui->listView_express->setModel(m_pModel_Express_DEV);

	LoadSettings();
}

mainwndCtrlPannel::~mainwndCtrlPannel()
{
	delete ui;
}

void mainwndCtrlPannel::changeEvent(QEvent *e)
{
	QMainWindow::changeEvent(e);
	switch (e->type()) {
	case QEvent::LanguageChange:
		ui->retranslateUi(this);
		break;
	default:
		break;
	}
}
void mainwndCtrlPannel::on_actionExit_triggered()
{
	this->close();
}
void mainwndCtrlPannel::on_actionConfig_triggered()
{
	m_pSettingsDlg->exec();
}

void mainwndCtrlPannel::on_action_Connect_triggered(bool bConn)
{
	if (bConn == false)
	{
		m_db.close();
		m_db = QSqlDatabase();
		QSqlDatabase::removeDatabase(PKLTS_VIEW);
	}
	else
	{
		if (m_pLoginDlg->exec()==QDialog::Accepted)
			ui->action_Connect->setChecked(login());
		else
			ui->action_Connect->setChecked(false);

	}
	this->on_action_Refresh_triggered();
}

void mainwndCtrlPannel::on_action_About_triggered()
{
	QMessageBox::aboutQt(this);
}

bool mainwndCtrlPannel::login()
{
	QSettings settings(QCoreApplication::applicationFilePath()+".ini",QSettings::IniFormat);
	if (false==QSqlDatabase::contains(PKLTS_VIEW))
	{
		QString strDriver = settings.value("connection/Driver","QMYSQL").toString();
		m_db = QSqlDatabase::addDatabase(strDriver,PKLTS_VIEW);
		if (m_db.isValid()==false)
		{
			QMessageBox::warning(this,tr("DB Connect Error"),m_db.lastError().text());
			m_db = QSqlDatabase();
			return false;
		}
	}

	if (m_db.isOpen()==false)
	{
		QString strHostName = settings.value("connection/HostName","localhost").toString();
		int nPort = settings.value("connection/Port",3306).toInt();
		QString strUserName = settings.value("connection/UserName","").toString();
		QString strPassword = settings.value("connection/Password","").toString();
		QString strDatabaseName = settings.value("connection/DatabaseName","parkinglots").toString();
		m_db.setHostName(strHostName);
		m_db.setPort(nPort);
		m_db.setUserName(strUserName);
		m_db.setPassword(strPassword);
		m_db.setDatabaseName(strDatabaseName);
		if (m_db.open()==false)
		{
			QMessageBox::warning(this,tr("DB Connect Error"),m_db.lastError().text());
			m_db = QSqlDatabase();
			QSqlDatabase::removeDatabase(PKLTS_VIEW);
			return false;
		}
	}
	if (m_db.isOpen()==true)
	{
		QString strUserName = settings.value("login/UserName","").toString();
		QString strPassword = settings.value("login/Password","").toString();
		QSqlQuery query(m_db);
		query.prepare("select * from userlist where username = ? and password = ?;");
		query.addBindValue(strUserName);
		query.addBindValue(strPassword);
		if (false==query.exec())
		{
			QMessageBox::warning(this,tr("DB Query Error"),query.lastError().text());
			m_db.close();
			m_db = QSqlDatabase();
			QSqlDatabase::removeDatabase(PKLTS_VIEW);
			return false;
		}
		else if (query.next()==false)
		{
			QMessageBox::warning(this,tr("Username or password is not correct"),tr("The username you given is not exist, or password is not correct."));
			m_db.close();
			m_db = QSqlDatabase();
			QSqlDatabase::removeDatabase(PKLTS_VIEW);
			return false;
		}
		else
			return true;
	}

	return false;


}

void mainwndCtrlPannel::timerEvent(QTimerEvent * e)
{
	if (e->timerId()==m_nTimer)
	{
		this->killTimer(m_nTimer);
		//this->m_pModel_Summary_PK->refresh();
		//this->m_pModel_Summary_MAC->refresh();
		//this->m_pModel_Summary_DEV->refresh();
		this->m_pModel_Express_DEV->refresh();
		//this->m_pModel_Detail->refresh();
		//this->m_pModel_EvtHis->refresh();
		//this->m_pModel_MacHis->refresh();
		UpdateIconAndColors();
		m_nTimer = startTimer(2000);
	}
	return QMainWindow::timerEvent(e);
}
void mainwndCtrlPannel::on_action_Refresh_triggered()
{
	QSettings settings(QCoreApplication::applicationFilePath()+".ini",QSettings::IniFormat);
	QString strUserName = settings.value("login/UserName","").toString();
	QString strSQL;
	//Refresh Parking Lots
	strSQL = QString("select * from view_summary_park_user where username = '%1' ").arg(strUserName);
	m_pModel_Summary_PK->setQueryPrefix(strSQL,m_db);
	strSQL = QString("SELECT * FROM view_dev_mac_park_user where username = '%1' ").arg(strUserName);
	m_pModel_Detail->setQueryPrefix(strSQL,m_db);
	this->m_pModel_Summary_MAC->refresh();
	this->m_pModel_Summary_DEV->refresh();
	this->m_pModel_Express_DEV->refresh();
	this->m_pModel_EvtHis->refresh();
	this->m_pModel_MacHis->refresh();
	UpdateIconAndColors();
}
void mainwndCtrlPannel::on_listView_sum_pklts_doubleClicked(const QModelIndex & index)
{
	int nrow = index.row();
	if (nrow>=0 && nrow < m_pModel_Summary_PK->rowCount())
	{
		QSettings settings(QCoreApplication::applicationFilePath()+".ini",QSettings::IniFormat);
		QString strUserName = settings.value("login/UserName","").toString();
		int ID = m_pModel_Summary_PK->data(m_pModel_Summary_PK->index(nrow,0)).toInt();
		QString strSQL;
		//Refresh Mac
		strSQL = QString("select * from view_summary_mac_park_user where parkid = %1 and username = '%2' ").arg(ID).arg(strUserName);
		m_pModel_Summary_MAC->setQueryPrefix(strSQL,m_db);
		//strSQL = QString("select CONCAT(IFNULL(devicename,deviceid),'-',IF(occupied=0,'-IDLE','-BUSY')) as currstatus from view_dev_mac_park_user  where parkid = %1 and username = '%2' and deviceid like '0100%' and (status = 0 or status = 1) order by devicename,deviceid asc ").arg(ID).arg(strUserName);
		strSQL = QString("select CONCAT(CONCAT(IFNULL(devicename,'NONAME'), ':',mid(deviceid,17,6)),'-',IF(occupied=0,'-IDLE','-BUSY')) as currstatus from view_dev_mac_park_user  where parkid = %1 and username = '%2' and deviceid like '0100%' and (status = 0 or status = 1) order by devicename,deviceid asc ").arg(ID).arg(strUserName);
		m_pModel_Express_DEV->setQueryPrefix(strSQL,m_db,-1);
	}
}
void mainwndCtrlPannel::on_listView_sum_macs_doubleClicked(const QModelIndex & index)
{
	int nrow = index.row();
	if (nrow>=0 && nrow < m_pModel_Summary_MAC->rowCount())
	{
		QSettings settings(QCoreApplication::applicationFilePath()+".ini",QSettings::IniFormat);
		QString strUserName = settings.value("login/UserName","").toString();
		int ID = m_pModel_Summary_MAC->data(m_pModel_Summary_MAC->index(nrow,0)).toInt();
		ui->lineEdit_tarID_Mac->setText(QString("%1").arg(ID));
		QString strSQL;
		//Refresh Mac
		strSQL = QString("select * from view_dev_mac_park_user where macid = %1 and username = '%2'  ").arg(ID).arg(strUserName);
		m_pModel_Summary_DEV->setQueryPrefix(strSQL,m_db);
		strSQL = QString("select * from macevent where macevt_sourceid = %1 ").arg(ID);
		m_pModel_MacHis->setQueryPrefix(strSQL,m_db,0,1024,true);
		UpdateIconAndColors();
	}
}
void mainwndCtrlPannel::on_listView_sum_devices_doubleClicked(const QModelIndex & index)
{
	int nrow = index.row();
	if (nrow>=0 && nrow < m_pModel_Summary_DEV->rowCount())
	{
		QString ID = m_pModel_Summary_DEV->data(m_pModel_Summary_DEV->index(nrow,0)).toString();
		QString strSQL;
		//Refresh Mac
		strSQL = QString("select * from sensorevent where deviceid = '%1' ").arg(ID);
		ui->lineEdit_tarID_Dev->setText(QString("%1").arg(ID));
		m_pModel_EvtHis->setQueryPrefix(strSQL,m_db,0,1024,true);
	}
}
void  mainwndCtrlPannel::on_tableView_detailed_doubleClicked(const QModelIndex & index)
{
	int nrow = index.row();
	if (nrow>=0 && nrow < m_pModel_Detail->rowCount())
	{
		QString ID = m_pModel_Detail->data(m_pModel_Detail->index(nrow,0)).toString();
		QString strSQL;
		//Refresh Mac
		strSQL = QString("select * from sensorevent where deviceid = '%1' ").arg(ID);
		ui->lineEdit_tarID_Dev->setText(QString("%1").arg(ID));
		m_pModel_EvtHis->setQueryPrefix(strSQL,m_db,0,1024,true);
		UpdateIconAndColors();
	}
}
void mainwndCtrlPannel::UpdateIconAndColors()
{
	QColorIconSqlModel * pModel = 0;
	QColorIconSqlModel *  models_to_update [] = {m_pModel_Summary_DEV, m_pModel_Detail};
	const int mods = sizeof(models_to_update)/sizeof(models_to_update[0]);
	for (int md = 0;md<mods;++md)
	{
		pModel = models_to_update[md];
		int nRows = pModel->rowCount();
		for (int i=0;i<nRows;++i)
		{
			QModelIndex idxDevID = pModel->index(i,0);
			QModelIndex idxOccp = pModel->index(i,5);
			QModelIndex idxStatus = pModel->index(i,6);
			QString devID = pModel->data(idxDevID).toString();
			int occp = pModel->data(idxOccp).toInt();
			int Status = pModel->data(idxStatus).toInt();
			if (Status==0)
			{
				pModel->setData(pModel->index(i,6),QIcon(":/pkCtrlPannel/resources/good.png"),Qt::DecorationRole);
				if (devID.left(4)==QString("0100"))
				{
					if (occp==1)
						pModel->setData(pModel->index(i,0),QIcon(":/pkCtrlPannel/resources/havecar.png"),Qt::DecorationRole);
					else
						pModel->setData(pModel->index(i,0),QIcon(":/pkCtrlPannel/resources/nocar.png"),Qt::DecorationRole);
				}
				else
				{
					pModel->setData(pModel->index(i,0),QIcon(":/pkCtrlPannel/resources/AquaBubble 037.png"),Qt::DecorationRole);
				}
			}
			else if (Status==1)
			{
				pModel->setData(pModel->index(i,6),QIcon(":/pkCtrlPannel/resources/No Battery.png"),Qt::DecorationRole);
				if (devID.left(4)==QString("0100"))
				{
					if (occp==1)
						pModel->setData(pModel->index(i,0),QIcon(":/pkCtrlPannel/resources/havecar.png"),Qt::DecorationRole);
					else
						pModel->setData(pModel->index(i,0),QIcon(":/pkCtrlPannel/resources/nocar.png"),Qt::DecorationRole);
				}
				else
				{
					pModel->setData(pModel->index(i,0),QIcon(":/pkCtrlPannel/resources/AquaBubble 037.png"),Qt::DecorationRole);
				}
				pModel->setData(pModel->index(i,0),QBrush(QColor(255,0,0,64)),Qt::BackgroundColorRole);

			}
			else if (Status>1)
			{
				pModel->setData(pModel->index(i,6),QIcon(":/pkCtrlPannel/resources/Alert_04.png"),Qt::DecorationRole);
				if (devID.left(4)==QString("0100"))
				{
					if (occp==1)
						pModel->setData(pModel->index(i,0),QIcon(":/pkCtrlPannel/resources/havecar.png"),Qt::DecorationRole);
					else
						pModel->setData(pModel->index(i,0),QIcon(":/pkCtrlPannel/resources/nocar.png"),Qt::DecorationRole);
				}
				else
				{
					pModel->setData(pModel->index(i,0),QIcon(":/pkCtrlPannel/resources/AquaBubble 037.png"),Qt::DecorationRole);
				}
				pModel->setData(pModel->index(i,0),QBrush(QColor(255,0,0,128)),Qt::BackgroundColorRole);
			}
		}
	}
	{
		pModel = m_pModel_Express_DEV;
		int nRows = pModel->rowCount();
		for (int i=0;i<nRows;++i)
		{
			QModelIndex idxDevID = pModel->index(i,0);
			QString devID = pModel->data(idxDevID).toString();

			if (devID.right(4)==QString("BUSY"))
				pModel->setData(pModel->index(i,0),QIcon(":/pkCtrlPannel/resources/havecar.png"),Qt::DecorationRole);
			else
				pModel->setData(pModel->index(i,0),QIcon(":/pkCtrlPannel/resources/nocar.png"),Qt::DecorationRole);
		}
	}
}
void mainwndCtrlPannel::LoadSettings()
{
	QSettings settings(QCoreApplication::applicationFilePath()+".ini",QSettings::IniFormat);
	ui->lineEdit_devInfo->setText(settings.value("settings/lineEdit_devInfo","DEV001").toString());
	ui->lineEdit_devName->setText(settings.value("settings/lineEdit_devName","DEV NAME").toString());
	ui->lineEdit_firmwarePath->setText(settings.value("settings/lineEdit_firmwarePath","./default.dat").toString());
	ui->lineEdit_hostInfo->setText(settings.value("settings/lineEdit_hostInfo","MAC TEST 001").toString());
	ui->lineEdit_hostName->setText(settings.value("settings/lineEdit_hostName","MAC001").toString());
	ui->lineEdit_SvrIP->setText(settings.value("settings/lineEdit_SvrIP","127.0.0.1").toString());
	ui->lineEdit_Svr_Port->setText(settings.value("settings/lineEdit_Svr_Port","23457").toString());
	ui->lineEdit_tarID_Dev->setText(settings.value("settings/lineEdit_tarID_Dev","01FEFF000000000001234567890ABCDEF0000000").toString());
	ui->lineEdit_tarID_Mac->setText(settings.value("settings/lineEdit_tarID_Mac","65536").toString());
}

void mainwndCtrlPannel::SaveSettings()
{
	QSettings settings(QCoreApplication::applicationFilePath()+".ini",QSettings::IniFormat);
	settings.setValue("settings/lineEdit_devInfo",ui->lineEdit_devInfo->text());
	settings.setValue("settings/lineEdit_devName",ui->lineEdit_devName->text());
	settings.setValue("settings/lineEdit_firmwarePath",ui->lineEdit_firmwarePath->text());
	settings.setValue("settings/lineEdit_hostInfo",ui->lineEdit_hostInfo->text());
	settings.setValue("settings/lineEdit_hostName",ui->lineEdit_hostName->text());
	settings.setValue("settings/lineEdit_SvrIP",ui->lineEdit_SvrIP->text());
	settings.setValue("settings/lineEdit_Svr_Port",ui->lineEdit_Svr_Port->text());
	settings.setValue("settings/lineEdit_tarID_Dev",ui->lineEdit_tarID_Dev->text());
	settings.setValue("settings/lineEdit_tarID_Mac",ui->lineEdit_tarID_Mac->text());
}

void  mainwndCtrlPannel::ui_pntf(const char * pFmt , ...)
{
	int     len = 1024;
	int ret = 0;
	do{
		len *= 2;
		va_list args;
		char    *buffer = 0;
		// retrieve the variable arguments
		va_start( args, pFmt );
		buffer = (char*)malloc( len * sizeof(char) );
		ret = vsnprintf( buffer,len-1, pFmt, args );
		va_end (args);

		if (ret >=0 || len >=65536)
			ui->plainTextEdit_result->appendPlainText(QString::fromLocal8Bit(buffer));

		free( buffer );
	} while (ret <0 && len < 65536);
}

void mainwndCtrlPannel::on_pushButton_getMacInfo_clicked()
{
	SaveSettings();
	ui->plainTextEdit_result->clear();
	//First, Get The Mac ID you want to ask.
	//Then, define a structure, to hold result.
	stMsg_GetHostDetailsRsp rsp;

	quint32 nMacID = ui->lineEdit_tarID_Mac->text().toUInt();
	std::string strAddr = ui->lineEdit_SvrIP->text().toStdString();
	const char * address = strAddr.c_str();
	quint16 port = ui->lineEdit_Svr_Port->text().toShort();
	//And then, Call the method directly, just like a native method.
	//Inside the function, a remote call will be executed.
	int res = st_getMACInfo(address,port,nMacID, &rsp);

	//Check the result, and print the result.
	ui_pntf ("Res = %d\n",res);
	if (res == ALL_SUCCEED)
	{
		ui_pntf ("rsp.DoneCode = %d\n",(unsigned int)rsp.DoneCode);
		ui_pntf ("rsp.HostType = %d\n",(unsigned int)rsp.HostType);
		ui_pntf ("rsp.FirmwareVersion = %d\n",(unsigned int)rsp.FirmwareVersion);
		ui_pntf ("rsp.HostName = %s\n",rsp.HostName);
		ui_pntf ("rsp.HostInfo = %s\n",rsp.HostInfo);
		ui_pntf ("rsp.ConnetType = %d\n",(unsigned int)rsp.ConnetType);
		ui_pntf ("...\n");
		ui_pntf ("rsp.SensorNum = %d\n",(unsigned int)rsp.SensorNum);
		ui_pntf ("rsp.RelayNum = %d\n",(unsigned int)rsp.RelayNum);
		ui_pntf ("rsp.ANSensorNum = %d\n",(unsigned int)rsp.ANSensorNum);
		ui_pntf ("rsp.ANRelayNum = %d\n",(unsigned int)rsp.ANRelayNum);
	}
}

void mainwndCtrlPannel::on_pushButton_getDevList_clicked()
{
	SaveSettings();
	ui->plainTextEdit_result->clear();
	//First, Get The Mac ID you want to ask.
	quint32 nMacID = ui->lineEdit_tarID_Mac->text().toUInt();
	std::string strAddr = ui->lineEdit_SvrIP->text().toStdString();
	const char * address = strAddr.c_str();
	quint16 port = ui->lineEdit_Svr_Port->text().toShort();

	//Then, define a structure, to hold result.
	stMsg_GetDeviceListRsp * rsp = 0;
	//And then, Call the method directly, just like a native method.
	//Inside the function, a remote call will be executed.
	int res = st_getDeviceList(address,port,nMacID, &rsp);
	//Check the result, and print the result.
	ui_pntf ("Res = %d\n",res);
	if (res == ALL_SUCCEED)
	{
		ui_pntf ("rsp.DoneCode = %d\n",(unsigned int)rsp->DoneCode);
		ui_pntf ("rsp.nDevCount = %d\n",(unsigned int)rsp->nDevCount);
		for (int i=0;i<rsp->nDevCount;++i)
		{
			ui_pntf ("DeviceName[%5d] = %s\n",i,rsp->devicetable[i].DeviceName);
			ui_pntf ("DeviceNO  [%5d] = %s\n",i,rsp->devicetable[i].No);
			ui_pntf ("DeviceID  [%5d] = ",i);
			for (int j=0;j<24;++j)	ui_pntf ("%02x",rsp->devicetable[i].DeviceID[j]);
			ui_pntf ("\n");
		}
	}
	if (rsp)
		st_freeDeviceList(rsp);
}

void mainwndCtrlPannel::on_pushButton_getDevPara_clicked()
{
	SaveSettings();
	ui->plainTextEdit_result->clear();
	//First, Get The Mac ID you want to ask.
	quint32 nMacID = ui->lineEdit_tarID_Mac->text().toUInt();
	std::string strAddr = ui->lineEdit_SvrIP->text().toStdString();
	const char * address = strAddr.c_str();
	quint16 port = ui->lineEdit_Svr_Port->text().toShort();

	stMsg_GetDeviceParamReq req;
	//Get Device ID
	std::string strDevID = ui->lineEdit_tarID_Dev->text().toStdString();
	if (false==devidStr2Array(strDevID,req.DeviceID,sizeof(req.DeviceID)))
	{
		ui_pntf ("Error Reading Hex Data! ");
		return;
	}

	req.Opt_DALStatus = 1;
	req.Opt_DeviceInfo = 1;
	req.Opt_DeviceName = 1;

	//Then, define a structure, to hold result.
	stMsg_GetDeviceParamRsp  * rsp;	//And then, Call the method directly, just like a native method.
	//Inside the function, a remote call will be executed.
	int res = st_getDeviceParam(address,port,nMacID, &req,&rsp);

	//Check the result, and print the result.
	ui_pntf ("Res = %d\n",res);
	if (res == ALL_SUCCEED)
	{
		ui_pntf ("rsp.DoneCode = %d\n",(unsigned int)rsp->DoneCode);
		if (rsp->Opt_DeviceName) ui_pntf ("rsp.DeviceName = %s\n",rsp->DeviceName);
		if (rsp->Opt_DeviceInfo) ui_pntf ("rsp.DeviceName = %s\n",rsp->DeviceInfo);
		if (rsp->Opt_DALStatus)
		{
			ui_pntf ("rsp.DALLen = %d\n",(unsigned int)rsp->DALStatusBytesLen);
			for (size_t i=0;i<(unsigned int)rsp->DALStatusBytesLen;++i)
				ui_pntf ("%02X",rsp->DALStatusBytes[i]);
			ui_pntf ("\n");
		}
	}
	st_freeDeviceParam(rsp);

}
void mainwndCtrlPannel::on_toolButton_brff_clicked()
{
	SaveSettings();
	QSettings settings(QCoreApplication::applicationFilePath()+".ini",QSettings::IniFormat);
	QString lastOpenDir = settings.value("history/lastfudir","./").toString();
	QString fm = QFileDialog::getOpenFileName(
				this,
				tr("Open firmware update cab"),
				lastOpenDir,
				QString("dat files(*.dat);;cab files(*.cab);;bin files(*.bin);;All files(*.*)")
				);

	if (fm.size()<2)
		return;

	QFileInfo info(fm);
	QString abspath = info.absolutePath();

	settings.setValue("history/lastfudir",abspath);

	ui->lineEdit_firmwarePath->setText(info.absoluteFilePath());

}
void mainwndCtrlPannel::on_pushButton_setMacInfo_clicked()
{
	SaveSettings();
	ui->plainTextEdit_result->clear();
	//First, Get The Mac ID you want to ask.
	//Then, define a structure, to hold result.
	stMsg_SetHostDetailsRsp rsp;
	stMsg_SetHostDetailsReq req;

	quint32 nMacID = ui->lineEdit_tarID_Mac->text().toUInt();
	std::string strAddr = ui->lineEdit_SvrIP->text().toStdString();
	const char * address = strAddr.c_str();
	quint16 port = ui->lineEdit_Svr_Port->text().toShort();


	std::string strName = ui->lineEdit_hostName->text().toStdString();
	std::string strInfo = ui->lineEdit_hostInfo->text().toStdString();

	strncpy(req.HostName,strName.c_str(),sizeof(req.HostName)-1);
	strncpy(req.HostInfo,strInfo.c_str(),sizeof(req.HostInfo)-1);
	//And then, Call the method directly, just like a native method.
	//Inside the function, a remote call will be executed.
	int res = st_setHostDetails(address,port,nMacID,&req, &rsp);

	//Check the result, and print the result.
	ui_pntf ("Res = %d\n",res);
	if (res == ALL_SUCCEED)
	{
		ui_pntf ("rsp.DoneCode = %d\n",(unsigned int)rsp.DoneCode);
	}
}

void mainwndCtrlPannel::on_pushButton_setDevInfo_clicked()
{
	SaveSettings();
	ui->plainTextEdit_result->clear();
	//First, Get The Mac ID you want to ask.
	//Then, define a structure, to hold result.
	stMsg_setDeviceParamRsp rsp;
	stMsg_setDeviceParamReq req;

	quint32 nMacID = ui->lineEdit_tarID_Mac->text().toUInt();
	std::string strAddr = ui->lineEdit_SvrIP->text().toStdString();
	const char * address = strAddr.c_str();
	quint16 port = ui->lineEdit_Svr_Port->text().toShort();

	//Get Device ID
	std::string strDevID = ui->lineEdit_tarID_Dev->text().toStdString();
	if (false==devidStr2Array(strDevID,req.DeviceID,sizeof(req.DeviceID)))
	{
		ui_pntf ("Error Reading Hex Data! ");
		return;
	}
	req.Opt_DeviceInfo = 1;
	req.Opt_DeviceName = 1;


	std::string strName = ui->lineEdit_devName->text().toStdString();
	std::string strInfo = ui->lineEdit_devInfo->text().toStdString();

	strncpy(req.DeviceName,strName.c_str(),sizeof(req.DeviceName)-1);
	strncpy(req.DeviceInfo,strInfo.c_str(),sizeof(req.DeviceInfo)-1);
	//And then, Call the method directly, just like a native method.
	//Inside the function, a remote call will be executed.
	int res = st_setDeviceParam(address,port,nMacID,&req, &rsp);

	//Check the result, and print the result.
	ui_pntf ("Res = %d\n",res);
	if (res == ALL_SUCCEED)
	{
		ui_pntf ("rsp.DoneCode = %d\n",(unsigned int)rsp.DoneCode);
	}
}

void mainwndCtrlPannel::on_pushButton_removeDev_clicked()
{
	SaveSettings();
	ui->plainTextEdit_result->clear();
	//First, Get The Mac ID you want to ask.
	//Then, define a structure, to hold result.
	stMsg_RemoveDeviceRsp rsp;
	stMsg_RemoveDeviceReq req;

	quint32 nMacID = ui->lineEdit_tarID_Mac->text().toUInt();
	std::string strAddr = ui->lineEdit_SvrIP->text().toStdString();
	const char * address = strAddr.c_str();
	quint16 port = ui->lineEdit_Svr_Port->text().toShort();

	//Get Device ID
	std::string strDevID = ui->lineEdit_tarID_Dev->text().toStdString();
	if (false==devidStr2Array(strDevID,req.DeviceID,sizeof(req.DeviceID)))
	{
		ui_pntf ("Error Reading Hex Data! ");
		return;
	}

	//And then, Call the method directly, just like a native method.
	//Inside the function, a remote call will be executed.
	int res = st_removeDevice(address,port,nMacID,&req, &rsp);

	//Check the result, and print the result.
	ui_pntf ("Res = %d\n",res);
	if (res == ALL_SUCCEED)
	{
		ui_pntf ("rsp.DoneCode = %d\n",(unsigned int)rsp.DoneCode);
	}
}

void mainwndCtrlPannel::on_pushButton_runfu_clicked()
{
	SaveSettings();
	QFile file (ui->lineEdit_firmwarePath->text());
	if (file.open(QIODevice::ReadOnly)==false)
		return;

	//file total size
	int totalsz = file.size();
	int groups = totalsz / 4096 + totalsz % 4096==0?0:1;

	ui->plainTextEdit_result->clear();
	//First, Get The Mac ID you want to ask.
	//Then, define a structure, to hold result.
	stMsg_PushFirmUpPackRsp rsp;
	stMsg_PushFirmUpPackReq req;

	quint32 nMacID = ui->lineEdit_tarID_Mac->text().toUInt();
	std::string strAddr = ui->lineEdit_SvrIP->text().toStdString();
	const char * address = strAddr.c_str();
	quint16 port = ui->lineEdit_Svr_Port->text().toShort();


	quint8 buf[4096];

	int nRed = 0;
	int gp = 0;
	while (nRed = file.read((char *)buf,4096))
	{
		req.SectionIndex = gp;
		req.SectionNum = groups;
		req.SectionLen = nRed;

		//And then, Call the method directly, just like a native method.
		//Inside the function, a remote call will be executed.
		int res = st_updateFirmware(address,port,nMacID,&req,buf, &rsp);

		//Check the result, and print the result.
		ui_pntf ("Res = %d\n",res);
		int retry = 0;
		while (res == ALL_SUCCEED && retry < 10)
		{
			if (rsp.DoneCode!=0)
			{
				ui_pntf ("Block %d:%d retrying...!\n",gp,groups);
				res = st_updateFirmware(address,port,nMacID,&req,buf, &rsp);
			}
			else
			{
				ui_pntf ("Block %d%d succeeded...!\n",gp,groups);
				break;
			}
			++retry;
		}
		if (res != ALL_SUCCEED || rsp.DoneCode!=0)
		{
			ui_pntf ("Update Failed!\n");
			break;
		}

	}

	file.close();
}
void mainwndCtrlPannel::on_pushButton_dalctrl_clicked()
{
	SaveSettings();
	ui->plainTextEdit_result->clear();
	//First, Get The Mac ID you want to ask.
	//Then, define a structure, to hold result.
	stMsg_DeviceCtrlRsp rsp;
	stMsg_DeviceCtrlReq req;

	quint32 nMacID = ui->lineEdit_tarID_Mac->text().toUInt();
	std::string strAddr = ui->lineEdit_SvrIP->text().toStdString();
	const char * address = strAddr.c_str();
	quint16 port = ui->lineEdit_Svr_Port->text().toShort();

	//Get Device ID
	std::string strDevID = ui->lineEdit_tarID_Dev->text().toStdString();
	if (false==devidStr2Array(strDevID,req.DeviceID,sizeof(req.DeviceID)))
	{
		ui_pntf ("Error Reading Hex Data! ");
		return;
	}
	//get the dAL message
	QByteArray arrayDAL;
	std::string strDAL = ui->plainTextEdit_DAL->toPlainText().toStdString();
	if (false==HexStr2Array(strDAL,&arrayDAL))
	{
		ui_pntf ("Error Reading DAL HEX String Data! no \\n or spaces between letters. ");
		return;
	}

	req.DALArrayLength = arrayDAL.size();

	//And then, Call the method directly, just like a native method.
	//Inside the function, a remote call will be executed.
	int res = st_deviceCtrl(address,port,nMacID,&req,(const quint8 *)arrayDAL.constData(), &rsp);

	//Check the result, and print the result.
	ui_pntf ("Res = %d\n",res);
	if (res == ALL_SUCCEED)
	{
		ui_pntf ("rsp.DoneCode = %d\n",(unsigned int)rsp.DoneCode);
	}

}
