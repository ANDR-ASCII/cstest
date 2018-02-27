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

struct GenerateRowTask final
{
	RowData operator()() const;
};

}