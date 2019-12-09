/*************************************************************************************/
// 16. Поиск подстроки
// Ограничение времени	0.1 секунда
// Ограничение памяти	32Mb
// Ввод	стандартный ввод или input.txt
// Вывод	стандартный вывод или output.txt
// Найдите все вхождения шаблона в строку. 
// Длина шаблона – p, длина строки – n. Время O(n + p), доп. память – O(p).
// p <= 30000, n <= 300000. 
// Использовать один из методов: 
// - С помощью префикс-функции;
// - С помощью z-функции.
// Формат ввода
// Шаблон, символ перевода строки, строка.
// Формат вывода
// Позиции вхождения шаблона в строке.
/*************************************************************************************/
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
