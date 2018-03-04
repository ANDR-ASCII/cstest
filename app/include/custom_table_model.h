#pragma once

namespace Test
{

class RowsCollection;

class CustomTableModel : public QAbstractTableModel
{
	Q_OBJECT

public:
	CustomTableModel(const std::shared_ptr<RowsCollection>& collection, QObject* parent = nullptr);

	virtual int columnCount(const QModelIndex& parent = QModelIndex()) const override;
	virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;
	virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
	virtual Qt::ItemFlags flags(const QModelIndex& index) const override;

private slots:
	void onAboutCollectionChanged();

private:
	std::shared_ptr<RowsCollection> m_collection;
};

}