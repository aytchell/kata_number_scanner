#include <iostream>
#include <fstream>
#include "scanner.h"

void print_number(const std::vector<unsigned int>& digits)
{
    for (auto d : digits)
    {
        char printable = '0' + d;
        std::cout << printable;
    }
    std::cout << std::endl;
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage:\n\t" << argv[0] << " NUMBER_FILE\n\n";
        return 1;
    }

    try
    {
        std::fstream num_file(argv[1], std::ios_base::in);
        if (!num_file)
            throw std::invalid_argument("Failed to open NUMBER_FILE");

        scanner::NumberScanner scn;
        const std::vector<unsigned int>& digits = scn.scan(num_file);

        print_number(digits);
    }
    catch (std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}
