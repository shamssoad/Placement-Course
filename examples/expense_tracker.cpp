// C++17. Standalone teaching example; saves are not crash-atomic.
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

struct Expense {
    long long amountMinor;
    std::string description;
};

bool atEnd(std::istream& input) {
    input >> std::ws;
    return input.eof();
}

long long total(const std::vector<Expense>& expenses) {
    long long result = 0;
    for (const auto& expense : expenses) {
        if (expense.amountMinor <= 0 ||
            expense.amountMinor > std::numeric_limits<long long>::max() - result)
            throw std::runtime_error("Invalid amount or total overflow");
        result += expense.amountMinor;
    }
    return result;
}

Expense parseExpense(std::istream& input) {
    Expense expense{};
    if (!(input >> expense.amountMinor))
        throw std::runtime_error("Expected an integer amount");
    input >> std::ws;
    if (input.peek() != '"' || !(input >> std::quoted(expense.description)) ||
        expense.amountMinor <= 0 || expense.description.empty() || !atEnd(input))
        throw std::runtime_error("Expected positive amount and quoted description");
    if (expense.description.find_first_of("\r\n") != std::string::npos)
        throw std::runtime_error("Descriptions must fit one line");
    return expense;
}

std::vector<Expense> load(const std::string& path) {
    std::error_code error;
    bool exists = std::filesystem::exists(path, error);
    if (error) throw std::runtime_error("Cannot inspect data path");
    if (!exists) return {};
    std::ifstream input(path);
    if (!input) throw std::runtime_error("Cannot open data file");
    std::vector<Expense> expenses;
    std::string line;
    int lineNumber = 0;
    while (std::getline(input, line)) {
        ++lineNumber;
        std::istringstream record(line);
        try {
            expenses.push_back(parseExpense(record));
        } catch (const std::exception& exception) {
            throw std::runtime_error("Line " + std::to_string(lineNumber) +
                                     ": " + exception.what());
        }
    }
    if (input.bad()) throw std::runtime_error("Read failed");
    total(expenses); // Validate accumulated range before accepting the dataset.
    return expenses;
}

void save(const std::string& path, const std::vector<Expense>& expenses) {
    std::ofstream output(path, std::ios::trunc);
    if (!output) throw std::runtime_error("Cannot open output file");
    for (const auto& expense : expenses)
        output << expense.amountMinor << ' ' << std::quoted(expense.description) << '\n';
    output.flush();
    if (!output) throw std::runtime_error("Write failed");
    output.close();
    if (!output) throw std::runtime_error("Close failed");
}

int main(int argc, char* argv[]) {
    if (argc > 2) {
        std::cerr << "Usage: expense_tracker [data-file]\n";
        return 1;
    }
    const std::string path = argc == 2 ? argv[1] : "expenses.txt";
    std::vector<Expense> expenses;
    try {
        expenses = load(path);
    } catch (const std::exception& exception) {
        std::cerr << "Load failed: " << exception.what() << '\n';
        return 1;
    }
    std::cout << "Commands: add <minor-units> \"description\", list, total, save, quit\n";
    std::string line;
    while (std::cout << "> " && std::getline(std::cin, line)) {
        try {
            std::istringstream input(line);
            std::string command;
            if (!(input >> command)) continue;
            if (command == "add") {
                Expense expense = parseExpense(input);
                long long current = total(expenses);
                if (expense.amountMinor > std::numeric_limits<long long>::max() - current)
                    throw std::runtime_error("Total would overflow");
                expenses.push_back(std::move(expense));
                std::cout << "Added; use save to persist\n";
            } else {
                if (!atEnd(input)) throw std::runtime_error("Unexpected arguments");
                if (command == "list") {
                    for (std::size_t i = 0; i < expenses.size(); ++i)
                        std::cout << i + 1 << ": " << expenses[i].amountMinor << ' '
                                  << std::quoted(expenses[i].description) << '\n';
                } else if (command == "total") {
                    std::cout << total(expenses) << " minor units\n";
                } else if (command == "save") {
                    save(path, expenses);
                    std::cout << "Saved\n";
                } else if (command == "quit") {
                    break;
                } else {
                    throw std::runtime_error("Unknown command");
                }
            }
        } catch (const std::exception& exception) {
            std::cerr << "Error: " << exception.what() << '\n';
        }
    }
}
