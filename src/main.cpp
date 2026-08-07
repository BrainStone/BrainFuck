#include <fstream>
#include <iostream>

#include "BrainFuck.hpp"

int main(int argc, char* argv[]) {
	if (argc < 2) {
		std::cerr << "Usage: " << (argc > 0 ? argv[0] : "brainfuck_app") << " <program_file>\n";
		return 1;
	}

	std::ifstream programFile(argv[1]);
	if (!programFile) {
		std::cerr << "Error: Could not open file '" << argv[1] << "'\n";
		return 1;
	}

	BrainFuck interpreter(std::cin, programFile, std::cout);
	interpreter.run();

	return 0;
}
