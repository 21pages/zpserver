#ifndef QCOLORICONSQLMODEL_H
#define QCOLORICONSQLMODEL_H

#include <QSqlQueryModel>

class QColorIconSqlModel : public QSqlQueryModel
{
	Q_OBJECT
public:
	QColorIconSqlModel(QObject *parent = 0);
protected:
	QMap<QModelIndex,QMap<int,QVariant>> m_map_data;
public:
	Qt::ItemFlags flags(const QModelIndex &index) const;
	bool setData(const QModelIndex &index, const QVariant &value, int role);
	QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const;
	void queryChange ();
};

#endif // QCOLORICONSQLMODEL_H
