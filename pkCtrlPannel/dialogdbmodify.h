#ifndef DIALOGDBMODIFY_H
#define DIALOGDBMODIFY_H

#include <QDialog>
#include <QSqlRelationalTableModel>
namespace Ui {
	class DialogDbModify;
}

class DialogDbModify : public QDialog
{
	Q_OBJECT

public:
	explicit DialogDbModify(QSqlRelationalTableModel * pModel ,QWidget *parent = 0);
	~DialogDbModify();

private:
	Ui::DialogDbModify *ui;
	QSqlRelationalTableModel * m_pDModel;

protected slots:
	void on_pushButton_insert_clicked();
	void on_pushButton_delete_clicked();
	void on_pushButton_commit_clicked();
	void on_pushButton_revert_clicked();
};

#endif // DIALOGDBMODIFY_H
