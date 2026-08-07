#include <gtest/gtest.h>

#include "TestableBrainFuck.hpp"

TEST(ProgramTest, IgnoresNonInstructionCharacters) {
	TestableBrainFuck interpreter{"This text is ignored\n+."};
	interpreter.run();

	EXPECT_EQ(interpreter.test_output.str(), "\x01"s);
}

TEST(ProgramTest, ReadsInputAndWritesItBack) {
	TestableBrainFuck interpreter{",.", "K"};
	interpreter.run();

	EXPECT_EQ(interpreter.test_output.str(), "K");
}

TEST(ProgramTest, PreservesMemoryWhenMovingLeftOfTheFirstCell) {
	TestableBrainFuck interpreter{"++<+>."};
	interpreter.run();

	EXPECT_EQ(interpreter.test_output.str(), "\x02"s);
	EXPECT_EQ(interpreter.getMemoryPointer(), 1);
	ASSERT_EQ(interpreter.getMemory().size(), 2);
	EXPECT_EQ(interpreter.getMemory()[0], '\x01');
	EXPECT_EQ(interpreter.getMemory()[1], '\x02');
}

TEST(ProgramTest, StepSkipsNonInstructionsByDefault) {
	TestableBrainFuck interpreter{"ignored+."};

	EXPECT_TRUE(interpreter.step());
	EXPECT_EQ(interpreter.getMemoryCell(), '\x01');
	EXPECT_TRUE(interpreter.step());
	EXPECT_EQ(interpreter.test_output.str(), "\x01"s);
	EXPECT_FALSE(interpreter.step());
}

TEST(ProgramTest, RunsHelloWorldProgram) {
	TestableBrainFuck interpreter{
	    "++++++++++[>+++++++>++++++++++>+++>+<<<<-]>++.>+.+++++++..+++"
	    ".>++.<<+++++++++++++++.>.+++.------.--------.>+.>."};
	interpreter.run();

	EXPECT_EQ(interpreter.test_output.str(), "Hello World!\n");
}