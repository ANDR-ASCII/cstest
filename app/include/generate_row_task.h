#pragma once

namespace Test
{

struct RowData
{
	QString string;
	unsigned number;
	double floatingPointNumber;
	bool boolValue;
};

Q_DECLARE_METATYPE(RowData)

struct GenerateRowTask final
{
	RowData operator()() const;
};

}