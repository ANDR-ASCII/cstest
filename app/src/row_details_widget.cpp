#include "row_details_widget.h"
#include "rows_collection.h"

namespace Test
{

RowDetailsWidget::RowDetailsWidget(QWidget* parent)
	: QWidget(parent)
	, m_rowsCountLabel(new QLabel(this))
{
	QFont font;
	font.setBold(true);
	font.setWeight(75);

	QGridLayout* gridLayout = new QGridLayout(this);
	gridLayout->setMargin(0);

	QLabel* rowsCountLabelStub = new QLabel("Rows count: ");
	rowsCountLabelStub->setFont(font);

	QLabel* currentRowLabel = new QLabel(QString("Current row:"), this);
	currentRowLabel->setFont(font);

	gridLayout->addWidget(rowsCountLabelStub, 0, 0);
	gridLayout->addWidget(m_rowsCountLabel, 0, 1);
	gridLayout->addWidget(currentRowLabel, 1, 0);

	for (int i = 0; i < m_collection->columnCount(); ++i)
	{
		QLabel* columnNameLabel = new QLabel(QString("Column %1:").arg(i + 1), this);
		QLabel* valueLabel = new QLabel("select a row", this);

		gridLayout->addWidget(columnNameLabel, i + 2, 0);
		gridLayout->addWidget(valueLabel, i + 2, 1);

		m_labels.push_back(valueLabel);
	}
}

void RowDetailsWidget::setCollection(const std::shared_ptr<RowsCollection>& collection)
{
	m_collection = collection;

	onRowsCountChanged();

	VERIFY(connect(m_collection.get(), &RowsCollection::collectionChanged,
		this, &RowDetailsWidget::onRowsCountChanged, Qt::QueuedConnection));
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

void RowDetailsWidget::onRowsCountChanged()
{
	m_rowsCountLabel->setText(QString::number(m_collection->rowCount()));
}

}