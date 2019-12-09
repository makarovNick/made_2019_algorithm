#include <iostream>
#include <string>
#include <vector>

std::vector<int> KMP(const std::string& string, const std::string& pattern)
{
    std::vector<int> prefix_function(pattern.length(), 0);
    std::vector<int> result;

    for (int k = 0, i = 1; i < pattern.length(); ++i)
    {
        while (k > 0 && pattern[i] != pattern[k])
        {
            k = prefix_function[k - 1];
        }

        if (pattern[i] == pattern[k])
        {
            k++;
        }

        prefix_function[i] = k;
    }

    for (int k = 0, i = 0; i < string.length(); ++i)
    {
        while (k > 0 && pattern[k] != string[i])
        {
            k = prefix_function[k - 1];
        }

        if (pattern[k] == string[i])
        {
            k++;
        }

        if (k == pattern.length())
        {
            result.push_back(i - pattern.length() + 1);
        }
    }

    return result;
}

int main()
{
    std::string pattern, string;
    std::cin >> pattern >> string;

    auto indexes = KMP(string, pattern);
    for (auto i : indexes)
    {
        std::cout << i << " ";
    }

    return 0;
}
