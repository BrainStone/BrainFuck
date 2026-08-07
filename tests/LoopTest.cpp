#include <gtest/gtest.h>

#include "TestableBrainFuck.hpp"

TEST(LoopTest, ExcessiveOpening) {
	TestableBrainFuck interpreter{"["};
	interpreter.run();

	EXPECT_EQ(interpreter.getMemoryCell(), '\0');
	EXPECT_EQ(interpreter.getMemory().size(), 1);
}

TEST(LoopTest, ExcessiveClosing) {
	TestableBrainFuck interpreter{"]"};
	interpreter.run();

	EXPECT_EQ(interpreter.getMemoryCell(), '\0');
	EXPECT_EQ(interpreter.getMemory().size(), 1);
}

TEST(LoopTest, NoopLoop) {
	TestableBrainFuck interpreter{".[.]"};
	interpreter.run();

	EXPECT_EQ(interpreter.test_output.str(), "\0"s);
}

TEST(LoopTest, SimpleLoop) {
	TestableBrainFuck interpreter{"++.[-.]"};
	interpreter.run();

	EXPECT_EQ(interpreter.test_output.str(), "\x02\x01\x00"s);
}

TEST(LoopTest, MultipleInstructionsPerIteration) {
	TestableBrainFuck interpreter{"+++[>++<-]>."};
	interpreter.run();

	EXPECT_EQ(interpreter.test_output.str(), "\x06"s);
}

TEST(LoopTest, NestedLoops) {
	TestableBrainFuck interpreter{"++[>++[>+<-]<-]>>."};
	interpreter.run();

	EXPECT_EQ(interpreter.test_output.str(), "\x04"s);
}

TEST(LoopTest, LoopMovingLeft) {
	TestableBrainFuck interpreter{">+++[<+>-]<."};
	interpreter.run();

	EXPECT_EQ(interpreter.test_output.str(), "\x03"s);
}
