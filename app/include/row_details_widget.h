#pragma once

namespace Test
{

class RowsCollection;

class RowDetailsWidget : public QWidget
{
	Q_OBJECT

public:
	RowDetailsWidget(QWidget* parent = nullptr);
	
	void setCollection(const std::shared_ptr<RowsCollection>& collection);
	
public slots:
	void showDetailsFor(const QModelIndex& index);

private:
	std::shared_ptr<RowsCollection> m_collection;
	std::vector<QLabel*> m_labels;
};

}