#include <vector>
#include <cstdint>
#include <cstddef>
#include <assert.h>

struct Line
{
	std::vector<char> text;

	uint32_t row = 0;
};

class LineBuffer
{
public:
	LineBuffer();
	
	void Insert(
			int16_t row,
			int16_t column,
			char append
		);

	void Delete(int16_t row, int16_t column);

	inline size_t LineCount()
	{
		return m_lines.size();
	}

	inline Line GrabLine(uint16_t row)
	{
		assert((row > 0 || row < m_lines.size()) && "Row out of bounds!");

		return m_lines[row];
	}

	inline size_t LineLength(uint16_t row)
	{
		assert((row > 0 || row < m_lines.size()) && "Row out of bounds!");

		return m_lines[row].text.size();
	}

private:
	std::vector<Line> m_lines;

	Line _CheckColumnRow(int16_t row, int16_t column);

};
