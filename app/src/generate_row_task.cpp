#include "generate_row_task.h"
#include "helpers.h"

namespace Test
{

RowData GenerateRowTask::operator()() const
{
	RowData rowData;

	constexpr int stringLength = 5;

	for (int i = 0; i < stringLength; ++i)
	{
		const char character = Helpers::randomValueFromRange('a', 'z');

		rowData.string += character;
	}

	rowData.number = Helpers::randomValueFromRange(0, 100);

	return rowData;
}

}