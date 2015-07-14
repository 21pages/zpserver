#include "dialogdbmodify.h"
#include "ui_dialogdbmodify.h"
#include <QSqlRelationalTableModel>
#include <QSqlRelationalDelegate>
#include <QModelIndex>
#include <QModelIndexList>
#include <QItemSelectionModel>
#include <QSet>
#include <QSqlError>
#include <QMessageBox>
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
void DialogDbModify::on_pushButton_insert_clicked()
{
	m_pDModel->insertRow(m_pDModel->rowCount());
}

void DialogDbModify::on_pushButton_delete_clicked()
{
	QModelIndexList lst = ui->tableView_db->selectionModel()->selectedIndexes();
	QSet<int> rows;
	foreach(const QModelIndex &idx,lst)
		rows.insert(idx.row());
	foreach (int row, rows)
		m_pDModel->removeRow(row);
}

void DialogDbModify::on_pushButton_commit_clicked()
{
	if (QMessageBox::Ok==QMessageBox::information(
				this,
				tr("Commit"),
				tr("Are you sure to commit changes?"),
				QMessageBox::Ok,
				QMessageBox::Cancel
				))
		if (false==m_pDModel->submitAll())
		{
			QSqlError err = m_pDModel->lastError();
			QMessageBox::warning(this,tr("Failed"),err.databaseText()+":"+err.driverText()+"-"+err.text());
		}

}

void DialogDbModify::on_pushButton_revert_clicked()
{
	if (QMessageBox::Ok==QMessageBox::information(
				this,
				tr("Commit"),
				tr("Are you sure to revert changes?"),
				QMessageBox::Ok,
				QMessageBox::Cancel
				))
		m_pDModel->revertAll();

}
