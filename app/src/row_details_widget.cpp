#include "row_details_widget.h"
#include "rows_collection.h"

namespace Test
{

RowDetailsWidget::RowDetailsWidget(QWidget* parent)
	: QWidget(parent)
{
	QVBoxLayout* thisLayout = new QVBoxLayout(this);
	thisLayout->setMargin(0);

	QHBoxLayout* stubLayout = new QHBoxLayout;
	QLabel* stub = new QLabel(QString("Current row:"), this);

	stubLayout->addWidget(stub);

	QFont font;
	font.setBold(true);
	font.setWeight(75);

	stub->setFont(font);

	thisLayout->addLayout(stubLayout);

	for (int i = 0; i < m_collection->columnCount(); ++i)
	{
		QHBoxLayout* layout = new QHBoxLayout;
		QLabel* columnNameLabel = new QLabel(QString("Column %1:").arg(i + 1), this);
		QLabel* valueLabel = new QLabel(QString("select a row"), this);

		columnNameLabel->setFont(font);

		layout->addWidget(columnNameLabel);
		layout->addWidget(valueLabel);
		thisLayout->addLayout(layout);

		m_labels.push_back(valueLabel);
	}
}

void RowDetailsWidget::setCollection(const std::shared_ptr<RowsCollection>& collection)
{
	m_collection = collection;
}

void RowDetailsWidget::showDetailsFor(const QModelIndex& index)
{
	ASSERT(m_collection->rowCount() > index.row());

	for (int i = 0; i < m_collection->columnCount(); ++i)
	{
		const QVariant item = m_collection->itemAt(index.row(), i);

		const QString string = item.type() == QVariant::Double ?
			QString::number(item.toDouble(), 'f', 3) :
			qvariant_cast<QString>(item);

		m_labels[i]->setText(string);
	}
}

}