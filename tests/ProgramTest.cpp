#include <gtest/gtest.h>

#include <sstream>
#include <stdexcept>

#include "TestableBrainFuck.hpp"

namespace {
class NonSeekableStringBuffer final : public std::stringbuf {
protected:
	std::streampos seekoff(std::streamoff, std::ios_base::seekdir, std::ios_base::openmode) override {
		return std::streampos{std::streamoff{-1}};
	}

	std::streampos seekpos(std::streampos, std::ios_base::openmode) override {
		return std::streampos{std::streamoff{-1}};
	}
};
}  // namespace

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

TEST(ProgramTest, StoresEndOfFileWhenInputIsExhausted) {
	TestableBrainFuck interpreter{",."};
	interpreter.run();

	const std::string expectedOutput(1, static_cast<char>(std::char_traits<char>::eof()));
	EXPECT_EQ(interpreter.test_output.str(), expectedOutput);
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

TEST(ProgramTest, StepProcessesNoopWhenRequested) {
	TestableBrainFuck interpreter{"x+"};

	EXPECT_TRUE(interpreter.step(false));
	EXPECT_EQ(interpreter.getMemoryCell(), '\0');
	EXPECT_TRUE(interpreter.step(false));
	EXPECT_EQ(interpreter.getMemoryCell(), '\x01');
}

TEST(ProgramTest, EmptyProgramDoesNotExecuteAPhantomInstruction) {
	TestableBrainFuck interpreter{""};

	EXPECT_FALSE(interpreter.step());
	EXPECT_FALSE(interpreter.step());
	EXPECT_EQ(interpreter.getProgramPosition(), 0);
}

TEST(ProgramTest, StopsAfterSkippingOnlyNoops) {
	TestableBrainFuck interpreter{"ignored"};

	EXPECT_FALSE(interpreter.step());
	EXPECT_EQ(interpreter.getProgramPosition(), 7);
}

TEST(ProgramTest, RejectsNonSeekableProgramStreams) {
	std::istringstream input;
	NonSeekableStringBuffer programBuffer;
	std::istream program{&programBuffer};
	std::ostringstream output;

	EXPECT_THROW((BrainFuck{input, program, output}), std::runtime_error);
}

TEST(ProgramTest, RejectsProgramStreamsThatBecomeBad) {
	TestableBrainFuck interpreter{"+"};
	interpreter.test_program.setstate(std::ios::badbit);

	EXPECT_THROW(interpreter.step(), std::runtime_error);
}

TEST(ProgramTest, RejectsProgramStreamsThatFailDuringReading) {
	std::istringstream input;
	std::istringstream program;
	std::ostringstream output;
	BrainFuck interpreter{input, program, output};

	program.setstate(std::ios::failbit);

	EXPECT_THROW(interpreter.step(), std::runtime_error);
}

TEST(ProgramTest, RunsHelloWorldProgram) {
	TestableBrainFuck interpreter{
	    "++++++++++[>+++++++>++++++++++>+++>+<<<<-]>++.>+.+++++++..+++"
	    ".>++.<<+++++++++++++++.>.+++.------.--------.>+.>."};
	interpreter.run();

	EXPECT_EQ(interpreter.test_output.str(), "Hello World!\n");
}
