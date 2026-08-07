#pragma once

#include <sstream>

#include "BrainFuck.hpp"

using namespace std::string_literals;

class TestStreams {
public:
	std::istringstream test_input;
	std::istringstream test_program;
	std::ostringstream test_output;

protected:
	explicit TestStreams(const std::string_view programText) : test_program{std::string{programText}} {}
	TestStreams(const std::string_view programText, const std::string_view inputText)
	    : test_input{std::string{inputText}}, test_program{std::string{programText}} {}
};

class TestableBrainFuck : public TestStreams, public BrainFuck {
public:
	explicit TestableBrainFuck(const std::string_view programText)
	    : TestStreams{programText}, BrainFuck{test_input, test_program, test_output} {}
	TestableBrainFuck(const std::string_view programText, const std::string_view inputText)
	    : TestStreams{programText, inputText}, BrainFuck{test_input, test_program, test_output} {}

	 memory_cell_t& getMemoryCell() {
		return BrainFuck::getMemoryCell();
	}

	[[nodiscard]] std::size_t getMemoryPointer() const {
		return memoryPointer;
	}
};
