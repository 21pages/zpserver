#include "dialogdbmodify.h"
#include "ui_dialogdbmodify.h"
#include <QSqlRelationalTableModel>
#include <QSqlRelationalDelegate>
DialogDbModify::DialogDbModify(QSqlRelationalTableModel * pModel ,QWidget *parent) :
	QDialog(parent),
	m_pDModel(pModel),
	ui(new Ui::DialogDbModify)
{
	ui->setupUi(this);
	ui->tableView_db->setModel(pModel);
	ui->tableView_db->setItemDelegate(new QSqlRelationalDelegate(ui->tableView_db));
}

DialogDbModify::~DialogDbModify()
{
	delete ui;
}
