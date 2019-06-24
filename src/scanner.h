#pragma once

#include <string>
#include <vector>
#include <map>
#include <istream>

namespace scanner
{
    class NumberScanner
    {
        public:
            NumberScanner();

            std::vector<unsigned int> scan(std::istream& in) const;

            std::vector<unsigned int> scan(
                    const std::string& fst_line,
                    const std::string& snd_line,
                    const std::string& thrd_line) const;

        private:
            std::vector<unsigned int>
                translate_to_num(const std::vector<unsigned int>& numbers)
                const;

            const std::map<std::string, unsigned int> top_cands;
            const std::map<std::string, unsigned int> mid_cands;
            const std::map<std::string, unsigned int> bot_cands;
            const std::map<unsigned int, unsigned int> enum_to_num;
    };
}
