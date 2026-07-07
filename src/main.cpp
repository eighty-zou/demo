#include "parser.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>

// Convert string to lowercase for case-insensitive comparison
static std::string toLower(const std::string& s) {
    std::string result = s;
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return result;
}

int main() {
    std::cout << "Calculator — type an expression or 'quit' to exit\n";

    std::string line;
    while (true) {
        std::cout << "> ";

        if (!std::getline(std::cin, line)) {
            // EOF (Ctrl+D) — exit cleanly
            std::cout << '\n';
            break;
        }

        // Skip empty lines
        if (line.empty()) {
            continue;
        }

        // Check for quit command (case-insensitive)
        if (toLower(line) == "quit") {
            break;
        }

        // Parse and evaluate
        try {
            Parser parser(line);
            double result = parser.parse();
            std::cout << result << '\n';
        } catch (const std::runtime_error& e) {
            std::cout << "Error: " << e.what() << '\n';
        }
    }

    return 0;
}
