#include "line_buffer.hpp"

#include <cstdio>

LineBuffer::LineBuffer()
{
	m_lines.push_back(Line());
}

void LineBuffer::Insert(
	int16_t row,
	int16_t column,
	char append
)
{
	Line line = _CheckColumnRow(row, column);

	m_lines[row].text.insert(m_lines[row].text.begin() + column, append);
}

void LineBuffer::Delete(int16_t row, int16_t column)
{
	Line line = _CheckColumnRow(row, column);	
}

Line LineBuffer::_CheckColumnRow(int16_t row, int16_t column)
{
	const size_t lineCount = m_lines.size();

	assert((row <= static_cast<int16_t>(lineCount)) && "Row cannot be greater than to the line count!");

	Line line = m_lines[row];

	const size_t lineLength = line.text.size();

	assert((column <= static_cast<int16_t>(lineLength)) && "Column cannot be greater than to the length of the line");

	return line;
}
