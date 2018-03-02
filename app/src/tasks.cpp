#include "tasks.h"
#include "rows_collection.h"
#include "row_generator.h"
#include "helpers.h"

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

//////////////////////////////////////////////////////////////////////////

DeserializationTask::DeserializationTask(const std::shared_ptr<RowsCollection>& collection, const QString& filepath)
	: m_collection(collection)
	, m_filepath(filepath)
{
}

void DeserializationTask::operator()()
{
	QFile fileDescriptor(m_filepath);

	if (!fileDescriptor.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		throw std::runtime_error(fileDescriptor.errorString().toStdString());
	}

	QXmlStreamReader reader(&fileDescriptor);

	while (!reader.atEnd())
	{
		reader.readNext();

		if (!reader.isStartElement())
		{
			continue;
		}

		if (reader.qualifiedName() == s_contentKey)
		{
			readRows(reader);
		}
	}
}

void DeserializationTask::readRows(QXmlStreamReader& reader)
{
	while (reader.qualifiedName() != s_contentKey || !reader.isEndElement())
	{
		reader.readNext();

		if (!reader.isStartElement())
		{
			continue;
		}

		if (reader.qualifiedName() == s_rowKey)
		{
			RowData rowData;

			while (!reader.isEndElement() || reader.qualifiedName() != s_rowKey)
			{
				reader.readNext();

				if (!reader.isStartElement())
				{
					continue;
				}

				if (reader.qualifiedName() == s_stringKey)
				{
					const std::string str = reader.readElementText().toStdString();

					ASSERT(str.size() == Helpers::arrayLength(rowData.string) - 1);

					std::copy(str.begin(), str.end(), std::begin(rowData.string));

					rowData.string[Helpers::arrayLength(rowData.string) - 1] = 0;
				}

				if (reader.qualifiedName() == s_numberKey)
				{
					rowData.number = reader.readElementText().toInt();
				}

				if (reader.qualifiedName() == s_floatingPointNumberKey)
				{
					rowData.floatingPointNumber = reader.readElementText().toDouble();
				}

				if (reader.qualifiedName() == s_boolValueKey)
				{
					const QString boolValue = reader.readElementText();

					rowData.boolValue = boolValue == QString::fromUtf8("true") ? true : false;
				}
			}

			m_collection->addRow(rowData);
		}
	}
}

}