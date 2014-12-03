#ifndef DIALOGSETTINGS_H
#define DIALOGSETTINGS_H

#include <QDialog>
#include <QSqlDatabase>
namespace Ui {
	class DialogSettings;
}

class DialogSettings : public QDialog
{
	Q_OBJECT

public:
	explicit DialogSettings(QWidget *parent = 0);
	~DialogSettings();

protected:
	void changeEvent(QEvent *e);

private:
	Ui::DialogSettings *ui;
	void Read_cfg();
	void Save_cfg();
public slots:
	void on_pushButton_ok_clicked();
	void on_pushButton_cancel_clicked();
};

#endif // DIALOGSETTINGS_H
