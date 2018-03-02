#include "serialization_task.h"
#include "rows_collection.h"

namespace
{

const QString s_contentKey = QLatin1String("content");
const QString s_rowKey = QLatin1String("row");

const QString s_stringKey = QLatin1String("string");
const QString s_numberKey = QLatin1String("number");
const QString s_floatingPointNumberKey = QLatin1String("floating_point_number");
const QString s_boolValueKey = QLatin1String("bool_value");

}

namespace Test
{

SerializationTask::SerializationTask(const std::shared_ptr<RowsCollection>& collection, const QString& filepath)
	: m_collection(collection)
	, m_filepath(filepath)
{
}

void SerializationTask::operator()()
{
	QFile fileDescriptor(m_filepath);

	if (!fileDescriptor.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		throw std::runtime_error(fileDescriptor.errorString().toStdString());
	}

	QXmlStreamWriter writer(&fileDescriptor);

	writer.setAutoFormatting(true);
	writer.writeStartDocument();
	writer.writeStartElement(s_contentKey);

	saveRows(writer);

	writer.writeEndElement();
	writer.writeEndDocument();
}

void SerializationTask::saveRows(QXmlStreamWriter& writer)
{
	static const QString* dataKeys[] = 
	{
		&s_stringKey,
		&s_numberKey,
		&s_floatingPointNumberKey,
		&s_boolValueKey
	};

	for (int i = 0; i < m_collection->rowCount(); ++i)
	{
		writer.writeStartElement(s_rowKey);

		for (int j = 0; j < m_collection->columnCount(); ++j)
		{
			writer.writeTextElement(*dataKeys[j], qvariant_cast<QString>(m_collection->itemAt(i, j)));
		}

		writer.writeEndElement();
	}
}

}