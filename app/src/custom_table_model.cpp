#include "custom_table_model.h"
#include "rows_collection.h"

namespace Test
{

CustomTableModel::CustomTableModel(const std::shared_ptr<RowsCollection>& collection, QObject* parent)
	: QAbstractTableModel(parent)
	, m_collection(collection)
{
	VERIFY(connect(m_collection.get(), &RowsCollection::rowAdded, this, &CustomTableModel::onRowAdded, Qt::QueuedConnection));
	VERIFY(connect(m_collection.get(), &RowsCollection::allRowsRemoved, this, &CustomTableModel::onAboutAllDataRemoved, Qt::QueuedConnection));
}

int CustomTableModel::columnCount(const QModelIndex&) const
{
	return m_collection->columnCount();
}

int CustomTableModel::rowCount(const QModelIndex&) const
{
	return m_collection->rowCount();
}

QVariant CustomTableModel::data(const QModelIndex& index, int role) const
{
	if (role == Qt::DisplayRole)
	{
		return m_collection->itemAt(index.row(), index.column());
	}

	return QVariant();
}

Qt::ItemFlags CustomTableModel::flags(const QModelIndex& index) const
{
	return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

void CustomTableModel::onRowAdded(int index)
{
	beginInsertRows(QModelIndex(), index, index);
	endInsertRows();
}

void CustomTableModel::onAboutAllDataRemoved()
{
	beginResetModel();
	endResetModel();
}

}