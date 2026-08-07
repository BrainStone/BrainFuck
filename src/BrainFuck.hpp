#pragma once

#include <istream>
#include <optional>
#include <ostream>
#include <stack>
#include <string_view>
#include <vector>

class BrainFuck {
public:
	using pos_t = std::istream::pos_type;
	using memory_cell_t = char;

	constexpr static std::string_view validInstructions{"><+-[],."};

protected:
	constexpr static memory_cell_t memoryCellDefault = 0;

	std::istream& input;
	std::istream& program;
	std::ostream& output;

	std::size_t memoryPointer;
	std::vector<memory_cell_t> memory;
	pos_t programPosition;
	std::stack<pos_t> loopStack;

	memory_cell_t& getMemoryCell();
	std::optional<char> readChar();

public:
	BrainFuck(std::istream& input, std::istream& program, std::ostream& output);

	void run();
	bool step(bool ignoreNoop = true);

	[[nodiscard]] const decltype(loopStack)& getMemory() const;
};
