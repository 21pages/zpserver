#ifndef MAINWNDCTRLPANNEL_H
#define MAINWNDCTRLPANNEL_H

#include <QMainWindow>
#include <QSqlDatabase>
#include "dialoglogin.h"
#include "dialogsettings.h"
#include "qcoloriconsqlmodel.h"
namespace Ui {
	class mainwndCtrlPannel;
}

class mainwndCtrlPannel : public QMainWindow
{
	Q_OBJECT

public:
	explicit mainwndCtrlPannel(QWidget *parent = 0);
	~mainwndCtrlPannel();

protected:
	void changeEvent(QEvent *e);
	void timerEvent(QTimerEvent * e);
	DialogSettings * m_pSettingsDlg;
	DialogLogin * m_pLoginDlg;
	QColorIconSqlModel * m_pModel_Summary_PK;		QString m_str_sqlSummary_PK;
	QColorIconSqlModel * m_pModel_Summary_MAC;		QString m_str_sqlSummary_MAC;
	QColorIconSqlModel * m_pModel_Summary_DEV;		QString m_str_sqlSummary_DEV;
	QColorIconSqlModel * m_pModel_Detail;			QString m_str_sqlDetail;
	QColorIconSqlModel * m_pModel_EvtHis;			QString m_str_sqlEvtHis;
	QColorIconSqlModel * m_pModel_MacHis;			QString m_str_sqlMacHis;;
private:
	int m_nTimer;
	Ui::mainwndCtrlPannel *ui;
	QSqlDatabase m_db;
	bool login();
	void UpdateIconAndColors();
public slots:
	void on_actionExit_triggered();
	void on_actionConfig_triggered();
	void on_action_Connect_triggered(bool);
	void on_action_About_triggered();
	void on_action_Refresh_triggered();
	//User clicked
	void on_listView_sum_pklts_doubleClicked(const QModelIndex & index);
	void on_listView_sum_macs_doubleClicked(const QModelIndex & index);
	void on_listView_sum_devices_doubleClicked(const QModelIndex & index);
	void on_tableView_detailed_doubleClicked(const QModelIndex & index);
};

#endif // MAINWNDCTRLPANNEL_H
