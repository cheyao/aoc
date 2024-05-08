// Capitalize all words in a string
#include <iostream>
#include <string>

int main(int argc, char** argv) {
	if (argc != 2) {
		std::cerr << "Error! Wrong argument count\n"
			  << "Usage: ./00 \"Hello this is a test string\""
			  << std::endl;
		return 1;
	}

	std::string input(argv[1]);

	for (int i = 0; i < input.length(); ++i) {
		if (i == 0 || input[i - 1] == ' ') {
			input[i] = std::toupper(input[i]);
		}
	}

	// Print resulting string
	std::cout << input;

	return 0;
}
