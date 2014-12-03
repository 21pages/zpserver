#include "qcoloriconsqlmodel.h"

QColorIconSqlModel::QColorIconSqlModel(QObject *parent)
	:QSqlQueryModel(parent)
{
}
//Enable select text
Qt::ItemFlags QColorIconSqlModel::flags(const QModelIndex &index) const
{
	Qt::ItemFlags flags = QSqlQueryModel::flags(index);
	flags |= Qt::ItemIsEditable;
	return flags;
}

bool QColorIconSqlModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if (role != Qt::DisplayRole)
		m_map_data[index][role] = value;
	return QSqlQueryModel::setData(index,value,role);
}

QVariant QColorIconSqlModel::data ( const QModelIndex & index, int role ) const
{
	if (role != Qt::DisplayRole)
	{
		if (m_map_data.find(index)!=m_map_data.end())
			if (m_map_data[index].find(role)!=m_map_data[index].end())
				return m_map_data[index][role];
	}
	return QSqlQueryModel::data(index,role);
}

void QColorIconSqlModel::queryChange ()
{
	m_map_data.clear();
}
