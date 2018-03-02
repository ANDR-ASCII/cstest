#pragma once

namespace Test
{

class RowsCollection;

class SerializationTask
{
public:
	SerializationTask(const std::shared_ptr<RowsCollection>& collection, const QString& filepath);

	void operator()();

private:
	void saveRows(QXmlStreamWriter& writer);

private:
	std::shared_ptr<RowsCollection> m_collection;
	QString m_filepath;
};

}