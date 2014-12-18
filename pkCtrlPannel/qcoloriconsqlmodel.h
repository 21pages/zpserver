#ifndef QCOLORICONSQLMODEL_H
#define QCOLORICONSQLMODEL_H

#include <QSqlQueryModel>
#include <QVector>
class QColorIconSqlModel : public QSqlQueryModel
{
	Q_OBJECT
public:
	QColorIconSqlModel(QObject *parent = 0);
	void setQueryPrefix(QString strSQL,QSqlDatabase db = QSqlDatabase::database(),int nInitialSortCol = 0, int nLimit = -1, bool desc = false);
	void refresh();
protected:
	QMap<QModelIndex,QMap<int,QVariant> > m_map_data;
	QVector<QString> m_colNames;
	int m_nLimit = -1;
	QString m_strQueryPrefix;
	QSqlDatabase m_db;
	int m_nCurSortCol;
	bool m_nOrder;
public:
	Qt::ItemFlags flags(const QModelIndex &index) const;
	bool setData(const QModelIndex &index, const QVariant &value, int role);
	QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const;
	void queryChange ();
	void sort(int column, Qt::SortOrder order = Qt::AscendingOrder);

};

#endif // QCOLORICONSQLMODEL_H
