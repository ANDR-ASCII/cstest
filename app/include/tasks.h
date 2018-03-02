#pragma once

namespace Test
{

class RowsCollection;

class SerializationTask final
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

class DeserializationTask final
{
public:
	DeserializationTask(const std::shared_ptr<RowsCollection>& collection, const QString& filepath);

	void operator()();

private:
	void readRows(QXmlStreamReader& reader);

private:
	std::shared_ptr<RowsCollection> m_collection;
	QString m_filepath;
};


}