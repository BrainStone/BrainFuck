#include <gtest/gtest.h>

#include "TestableBrainFuck.hpp"

TEST(InstructionTest, MemoryPointerIncrement) {
	TestableBrainFuck interpreter{">"};
	interpreter.run();

	EXPECT_EQ(interpreter.getMemoryPointer(), 1);
	EXPECT_EQ(interpreter.getMemory().size(), 2);
}

TEST(InstructionTest, MemoryPointerDecrement) {
	TestableBrainFuck interpreter{"<"};
	interpreter.run();

	EXPECT_EQ(interpreter.getMemoryPointer(), 0);
	EXPECT_EQ(interpreter.getMemory().size(), 2);
}

TEST(InstructionTest, MemoryIncrement) {
	TestableBrainFuck interpreter{"+"};
	interpreter.run();

	EXPECT_EQ(interpreter.getMemoryCell(), '\x01');
	EXPECT_EQ(interpreter.getMemory().size(), 1);
}

TEST(InstructionTest, MemoryDecrement) {
	TestableBrainFuck interpreter{"-"};
	interpreter.run();

	EXPECT_EQ(interpreter.getMemoryCell(), '\xff');
	EXPECT_EQ(interpreter.getMemory().size(), 1);
}

// Loop instructions by themselves are impossible to test

TEST(InstructionTest, Input) {
	TestableBrainFuck interpreter{",", "a"};
	interpreter.run();

	EXPECT_EQ(interpreter.getMemoryCell(), 'a');
	EXPECT_EQ(interpreter.getMemory().size(), 1);
}

TEST(InstructionTest, Output) {
	TestableBrainFuck interpreter{"."};
	interpreter.run();

	EXPECT_EQ(interpreter.test_output.str(), "\0"s);
	EXPECT_EQ(interpreter.getMemoryCell(), '\0');
}
