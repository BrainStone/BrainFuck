#include "BrainFuck.hpp"

#include <iostream>
#include <stdexcept>

BrainFuck::BrainFuck(std::istream& input, std::istream& program, std::ostream& output)
    : input{input}, program{program}, output{output}, memoryPointer{0}, memory{memoryCellDefault}, programPosition{0} {
	if (const pos_t pos = program.tellg(); pos == pos_t{-1}) {
		throw std::runtime_error{"Program stream is not seekable"};
	}
}

BrainFuck::memory_cell_t& BrainFuck::getMemoryCell() {
	return memory[memoryPointer];
}

std::optional<char> BrainFuck::readChar() {
	if (!program.good()) {
		throw std::runtime_error{"Program stream is not good"};
	}

	const auto instruction = program.get();

	if (instruction == std::istream::traits_type::eof()) {
		return std::nullopt;
	}

	programPosition += pos_t{1};
	return {static_cast<char>(instruction)};
}

void BrainFuck::run() {
	while (step()) {
	}
}

bool BrainFuck::step(const bool ignoreNoop) {
	using namespace std::literals;

	if (program.eof()) {
		return false;
	}

	std::optional<char> instruction;

	do {
		instruction = readChar();

		if (!instruction.has_value()) {
			return false;
		}
	} while (ignoreNoop && !validInstructions.contains(instruction.value()));

	switch (instruction.value()) {
	case '>':
		++memoryPointer;

		if (memoryPointer >= memory.size()) {
			memory.push_back(memoryCellDefault);
		}
		break;
	case '<':
		if (memoryPointer == 0) {
			// Super inefficient! Avoid if possible
			memory.insert(memory.begin(), memoryCellDefault);
		} else {
			--memoryPointer;
		}
		break;

	case '+':
		++getMemoryCell();
		break;
	case '-':
		--getMemoryCell();
		break;

	case '[':
		if (getMemoryCell() != 0) {
			loopStack.push(programPosition);
		} else {
			std::size_t nestingLevel = 1;

			do {
				instruction = readChar();

				if (!instruction.has_value()) {
					return false;
				}

				if (instruction.value() == '[') {
					++nestingLevel;
				} else if (instruction.value() == ']') {
					--nestingLevel;
				}
			} while (nestingLevel > 0);
		}
		break;
	case ']':
		if (loopStack.empty()) {
			break;
		}

		if (getMemoryCell() != 0) {
			const pos_t diff = loopStack.top() - programPosition;

			program.clear();
			program.seekg(diff, std::ios_base::cur);
			programPosition += diff;
		} else {
			loopStack.pop();
		}
		break;

	case ',':
		getMemoryCell() = static_cast<memory_cell_t>(input.get());
		break;
	case '.':
		output.put(static_cast<char>(getMemoryCell()));
		break;

	default:
		// NOOP
		break;
	}

	return true;
}

[[nodiscard]] const decltype(BrainFuck::memory)& BrainFuck::getMemory() const {
	return memory;
}

BrainFuck::pos_t BrainFuck::getProgramPosition() const {
	return programPosition;
}
