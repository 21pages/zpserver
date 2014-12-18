#include <QMessageBox>
#include <QDialog>
#include <QSettings>
#include <QSqlError>
#include <QSqlQuery>
#include "mainwndctrlpannel.h"
#include "ui_mainwndctrlpannel.h"
#include "./pklts_ctrl/st_ctrl.h"
#define PKLTS_VIEW ("PKLTS_VIEW")

using namespace ParkinglotsSvr;

mainwndCtrlPannel::mainwndCtrlPannel(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::mainwndCtrlPannel)
{
	ui->setupUi(this);
	m_nTimer = startTimer(60000);
	m_pSettingsDlg = new DialogSettings(this);
	m_pLoginDlg = new DialogLogin(this);
	m_pModel_Summary_PK = new QColorIconSqlModel(this);  ui->listView_sum_pklts->setModel(m_pModel_Summary_PK);
	m_pModel_Summary_MAC = new QColorIconSqlModel(this); ui->listView_sum_macs->setModel(m_pModel_Summary_MAC);
	m_pModel_Summary_DEV = new QColorIconSqlModel(this); ui->listView_sum_devices->setModel(m_pModel_Summary_DEV);
	m_pModel_Detail = new QColorIconSqlModel(this);      ui->tableView_detailed->setModel(m_pModel_Detail);
	m_pModel_EvtHis = new QColorIconSqlModel(this);      ui->tableView_events->setModel(m_pModel_EvtHis);
	m_pModel_MacHis = new QColorIconSqlModel(this);      ui->tableView_macevt->setModel(m_pModel_MacHis);
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
		this->m_pModel_Summary_PK->setQuery(m_str_sqlSummary_PK,m_db);
		this->m_pModel_Summary_MAC->setQuery(m_str_sqlSummary_MAC,m_db);
		this->m_pModel_Summary_DEV->setQuery(m_str_sqlSummary_DEV,m_db);
		this->m_pModel_Detail->setQuery(m_str_sqlDetail,m_db);
		this->m_pModel_EvtHis->setQuery(m_str_sqlEvtHis,m_db);
		this->m_pModel_MacHis->setQuery(m_str_sqlMacHis,m_db);
		UpdateIconAndColors();
	}
	return QMainWindow::timerEvent(e);
}
void mainwndCtrlPannel::on_action_Refresh_triggered()
{
	QSettings settings(QCoreApplication::applicationFilePath()+".ini",QSettings::IniFormat);
	QString strUserName = settings.value("login/UserName","").toString();
	QString strSQL;
	//Refresh Parking Lots
	strSQL = QString("select * from view_summary_park_user where username = '%1' order by parkid;").arg(strUserName);
	m_pModel_Summary_PK->setQuery(strSQL,m_db);	m_str_sqlSummary_PK = strSQL;
	strSQL = QString("SELECT * FROM view_dev_mac_park_user where username = '%1' order by parkid, macid , deviceid;").arg(strUserName);
	m_pModel_Detail->setQuery(strSQL,m_db);    m_str_sqlDetail = strSQL;
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
		strSQL = QString("select * from view_summary_mac_park_user where parkid = %1 and username = '%2' order by macid").arg(ID).arg(strUserName);
		m_pModel_Summary_MAC->setQuery(strSQL,m_db);
		m_str_sqlSummary_MAC = strSQL;
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
		QString strSQL;
		//Refresh Mac
		strSQL = QString("select * from view_dev_mac_park_user where macid = %1 and username = '%2'  order by deviceid").arg(ID).arg(strUserName);
		m_pModel_Summary_DEV->setQuery(strSQL,m_db);	m_str_sqlSummary_DEV = strSQL;
		strSQL = QString("select * from macevent where macevt_sourceid = %1 order by evtid desc limit 65536").arg(ID);
		m_pModel_MacHis->setQuery(strSQL,m_db);			m_str_sqlMacHis = strSQL;
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
		strSQL = QString("select * from sensorevent where deviceid = '%1' order by evtid desc limit 65536").arg(ID);
		m_pModel_EvtHis->setQuery(strSQL,m_db);			m_str_sqlEvtHis = strSQL;
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
		strSQL = QString("select * from sensorevent where deviceid = '%1' order by evtid desc limit 65536").arg(ID);
		m_pModel_EvtHis->setQuery(strSQL,m_db);			m_str_sqlEvtHis = strSQL;
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
					if (occp==0)
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
					if (occp==0)
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
					if (occp==0)
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
}

void  mainwndCtrlPannel::ui_pntf(const char * pFmt , ...)
{
	va_list args;
	int     len;
	char    *buffer;

	// retrieve the variable arguments
	va_start( args, pFmt );

	len = _vscprintf( pFmt, args ) // _vscprintf doesn't count
			+ 1; // terminating '\0'

	buffer = (char*)malloc( len * sizeof(char) );

	vsprintf( buffer, pFmt, args ); // C4996

	ui->plainTextEdit_result->appendPlainText(QString::fromLocal8Bit(buffer));
	puts( buffer );
	free( buffer );

}

void mainwndCtrlPannel::on_pushButton_getMacInfo_clicked()
{
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
	ui->plainTextEdit_result->clear();
	//First, Get The Mac ID you want to ask.
	quint32 nMacID = ui->lineEdit_tarID_Mac->text().toUInt();
	std::string strAddr = ui->lineEdit_SvrIP->text().toStdString();
	const char * address = strAddr.c_str();
	quint16 port = ui->lineEdit_Svr_Port->text().toShort();

	stMsg_GetDeviceParamReq req;
	//Get Device ID
	std::string strDevID = ui->lineEdit_tarID_Dev->text().toStdString();
	char buf[256];
	strncpy(buf,strDevID.c_str(),49);
	for (int i=0;i<24;++i)
	{
		unsigned __int8 cv = 0;
		if (buf[i*2] >='0' &&  buf[i*2] <='9')	cv += buf[i*2]-'0';
		else if (buf[i*2] >='a' &&  buf[i*2] <='f') cv += buf[i*2]-'a' + 10;
		else if (buf[i*2] >='A' &&  buf[i*2] <='F') cv += buf[i*2]-'A' + 10;
		else {ui_pntf ("Error Reading Hex Data! "); return;};
		cv <<= 4;
		if (buf[i*2+1] >='0' &&  buf[i*2+1] <='9')	cv += buf[i*2+1]-'0';
		else if (buf[i*2+1] >='a' &&  buf[i*2+1] <='f') cv += buf[i*2+1]-'a' + 10;
		else if (buf[i*2+1] >='A' &&  buf[i*2+1] <='F') cv += buf[i*2+1]-'A' + 10;
		else {ui_pntf ("Error Reading Hex Data! "); return;};
		req.DeviceID[i] = cv;
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
