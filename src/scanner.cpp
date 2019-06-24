#include "scanner.h"

#include <sstream>

namespace
{
    enum NumberCand
    {
        NC_0    = 0x0001,
        NC_1    = 0x0002,
        NC_2    = 0x0004,
        NC_3    = 0x0008,
        NC_4    = 0x0010,
        NC_5    = 0x0020,
        NC_6    = 0x0040,
        NC_7    = 0x0080,
        NC_8    = 0x0100,
        NC_9    = 0x0200
    };

    class Error : public std::stringstream
    {
        public:
            Error();
            void raise();
    };

    Error::Error() : std::stringstream() {}

    void Error::raise()
    {
        std::string msg(this->str());
        throw std::invalid_argument(msg.c_str());
    }

    std::map<std::string, unsigned int> cands_top()
    {
        std::map<std::string, unsigned int> cands;
        cands.insert(std::make_pair(" _ ",
                    NC_0 | NC_2 | NC_3 | NC_5 | NC_6 | NC_7 | NC_8 | NC_9));
        cands.insert(std::make_pair("   ", NC_1 | NC_4));
        return cands;
    }

    std::map<std::string, unsigned int> cands_mid()
    {
        std::map<std::string, unsigned int> cands;
        cands.insert(std::make_pair("| |", NC_0));
        cands.insert(std::make_pair("  |", NC_1 | NC_7));
        cands.insert(std::make_pair(" _|", NC_2 | NC_3));
        cands.insert(std::make_pair("|_|", NC_4 | NC_8 | NC_9));
        cands.insert(std::make_pair("|_ ", NC_5 | NC_6));
        return cands;
    }

    std::map<std::string, unsigned int> cands_bot()
    {
        std::map<std::string, unsigned int> cands;
        cands.insert(std::make_pair("|_|", NC_0 | NC_6 | NC_8));
        cands.insert(std::make_pair("  |", NC_1 | NC_4 | NC_7));
        cands.insert(std::make_pair("|_ ", NC_2));
        cands.insert(std::make_pair(" _|", NC_3 | NC_5 | NC_9));
        return cands;
    }

    std::map<unsigned int, unsigned int> get_enum_to_num()
    {
        std::map<unsigned int, unsigned int> nums;
        nums.insert(std::make_pair(NC_0, 0));
        nums.insert(std::make_pair(NC_1, 1));
        nums.insert(std::make_pair(NC_2, 2));
        nums.insert(std::make_pair(NC_3, 3));
        nums.insert(std::make_pair(NC_4, 4));
        nums.insert(std::make_pair(NC_5, 5));
        nums.insert(std::make_pair(NC_6, 6));
        nums.insert(std::make_pair(NC_7, 7));
        nums.insert(std::make_pair(NC_8, 8));
        nums.insert(std::make_pair(NC_9, 9));
        return nums;
    }

    void select_num_cands(
            std::vector<unsigned int>& numbers,
            int line_number,
            const std::string& line,
            const std::map<std::string, unsigned int>& cands_map)
    {
        for (unsigned int i = 0; i < line.length() / 3; ++i)
        {
            std::string triple = line.substr(i*3, 3);
            const auto& cand_iter = cands_map.find(triple);
            if (cand_iter == cands_map.end())
            {
                Error t;
                t << "Invalid digit part '" << triple << "' in line "
                    << line_number << " column " << (i*3 + 1);
                t.raise();
            }
            numbers[i] = (numbers[i] & cand_iter->second);
        }
    }
}

scanner::NumberScanner::NumberScanner()
    :
        top_cands(cands_top()),
        mid_cands(cands_mid()),
        bot_cands(cands_bot()),
        enum_to_num(get_enum_to_num())
{
}

std::vector<unsigned int> scanner::NumberScanner::scan(std::istream& in) const
{
    std::string line_1;
    std::string line_2;
    std::string line_3;

    std::getline(in, line_1);
    std::getline(in, line_2);
    std::getline(in, line_3);

    return scan(line_1, line_2, line_3);
}

std::vector<unsigned int> scanner::NumberScanner::scan(
                    const std::string& fst_line,
                    const std::string& snd_line,
                    const std::string& thrd_line) const
{
    if ((fst_line.length() != snd_line.length()) ||
            (fst_line.length() != thrd_line.length()))
    {
        throw std::invalid_argument("Malformed number (line lengths differ)");
    }

    if (fst_line.length() % 3 != 0)
    {
        throw std::invalid_argument("Malformed number (incomplete digit)");
    }

    static const unsigned int all_bits = static_cast<unsigned int>(~0);
    std::vector<unsigned int> numbers(fst_line.length() / 3, all_bits);

    select_num_cands(numbers, 1, fst_line, top_cands);
    select_num_cands(numbers, 2, snd_line, mid_cands);
    select_num_cands(numbers, 3, thrd_line, bot_cands);

    return translate_to_num(numbers);
}


std::vector<unsigned int>
scanner::NumberScanner::translate_to_num(
        const std::vector<unsigned int>& numbers) const
{
    std::vector<unsigned int> result;
    for (auto cand : numbers)
    {
        const auto& digit_iter = enum_to_num.find(cand);
        if (digit_iter == enum_to_num.end())
        {
            throw std::invalid_argument("Malformed digit");
        }
        result.push_back(digit_iter->second);
    }
    return result;
}
