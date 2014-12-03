#ifndef DIALOGLOGIN_H
#define DIALOGLOGIN_H

#include <QDialog>

namespace Ui {
	class DialogLogin;
}

class DialogLogin : public QDialog
{
	Q_OBJECT

public:
	explicit DialogLogin(QWidget *parent = 0);
	~DialogLogin();

protected:
	void changeEvent(QEvent *e);
	void Read_cfg();
	void Save_cfg();
private:
	Ui::DialogLogin *ui;
protected slots:
	void on_buttonBox_accepted();
};

#endif // DIALOGLOGIN_H
