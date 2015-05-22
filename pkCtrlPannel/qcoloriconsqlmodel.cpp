#include "qcoloriconsqlmodel.h"
#include <QSqlQuery>
#include <QSqlRecord>
QColorIconSqlModel::QColorIconSqlModel(QObject *parent)
	:QSqlQueryModel(parent)
{
	m_nCurSortCol = 0;
	m_nOrder = 0;
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

void  QColorIconSqlModel::sort(int column, Qt::SortOrder order/* = Qt::AscendingOrder*/)
{
	m_nOrder = order;
	m_nCurSortCol = column;
	QString strSql = m_strQueryPrefix;
	if (m_colNames.size())
	{
		if (m_nCurSortCol>=0 && m_nCurSortCol<m_colNames.size())
		{
			strSql += " order by " + m_colNames[m_nCurSortCol];
			if (m_nOrder == Qt::DescendingOrder)
				strSql += " desc ";
		}

		if (m_nLimit>0 )
			strSql += QString(" limit %1 ").arg(m_nLimit);
		this->setQuery(strSql,m_db);
	}

}
void  QColorIconSqlModel::setQueryPrefix(QString strSQL,QSqlDatabase db,int nInitialSortCol /*= 0*/, int nLimit/* = -1*/, bool desc)
{
	m_strQueryPrefix = strSQL;
	m_db = db;
	m_nLimit = nLimit;
	m_nCurSortCol = nInitialSortCol;
	m_nOrder = desc;

	//Get Col names
	m_colNames.clear();
	QSqlQuery query(m_db);
	QString sql = m_strQueryPrefix + " limit 1";
	if (query.exec(sql)==true)
	{
		QSqlRecord r =  query.record();
		int nc = r.count();
		for (int i=0;i<nc;++i)
			m_colNames.push_back(r.fieldName(i));
	}
	query.clear();

	if (m_nOrder)
		sort(m_nCurSortCol,Qt::DescendingOrder);
	else
		sort(m_nCurSortCol,Qt::AscendingOrder);

}
void QColorIconSqlModel::refresh()
{
	if (m_nOrder)
		sort(m_nCurSortCol,Qt::DescendingOrder);
	else
		sort(m_nCurSortCol,Qt::AscendingOrder);
}
