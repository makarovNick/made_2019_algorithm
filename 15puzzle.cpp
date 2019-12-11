/*******************************************************/
// A. Пятнашки (A)
// Ограничение времени	6 секунд
// Ограничение памяти	1Gb
// Ввод	стандартный ввод или input.txt
// Вывод	стандартный вывод или output.txt
// Написать алгоритм для решения игры в “пятнашки”. Решением задачи является приведение к виду: 
//                                                             [ 1   2   3   4 ] 
//                                                             [ 5   6   7   8 ] 
//                                                             [ 9  10  11  12 ] 
//                                                             [ 13 14  15   0 ]
// , где 0 задает пустую ячейку. 
// Достаточно найти хотя бы какое-то решение. Число перемещений костяшек не обязано быть минимальным.
//
// Формат ввода
// Начальная расстановка.
//
// Формат вывода
// Если вам удалось найти решение, то в первой строке файла выведите число перемещений, 
// которое требуется сделать в вашем решении. 
// А во второй строке выведите соответствующую последовательность ходов: 
// L – влево, 
// R – вправо, 
// U – вверх, 
// D – вниз. 
// Если же выигрышная конфигурация недостижима, то выведите в выходной файл одно число −1.
/*******************************************************/
#include <unordered_set>
#include <unordered_map>
#include     <algorithm>
#include      <iostream>
#include        <vector>
#include         <queue>

constexpr uint_fast32_t N = 4;

constexpr uint_fast32_t SOLVED = 0x123456789ABCDEF0;

constexpr uint_fast32_t GET_MASK[N * N]
    = { 0xF000000000000000, 0x0F00000000000000, 0x00F0000000000000, 0x000F000000000000,
        0x0000F00000000000, 0x00000F0000000000, 0x000000F000000000, 0x0000000F00000000,
        0x00000000F0000000, 0x000000000F000000, 0x0000000000F00000, 0x00000000000F0000,
        0x000000000000F000, 0x0000000000000F00, 0x00000000000000F0, 0x000000000000000F };

constexpr uint_fast32_t SET_MASK[N * N]
    = { 0x0FFFFFFFFFFFFFFF, 0xF0FFFFFFFFFFFFFF, 0xFF0FFFFFFFFFFFFF, 0xFFF0FFFFFFFFFFFF,
        0xFFFF0FFFFFFFFFFF, 0xFFFFF0FFFFFFFFFF, 0xFFFFFF0FFFFFFFFF, 0xFFFFFFF0FFFFFFFF,
        0xFFFFFFFF0FFFFFFF, 0xFFFFFFFFF0FFFFFF, 0xFFFFFFFFFF0FFFFF, 0xFFFFFFFFFFF0FFFF,
        0xFFFFFFFFFFFF0FFF, 0xFFFFFFFFFFFFF0FF, 0xFFFFFFFFFFFFFF0F, 0xFFFFFFFFFFFFFFF0 };

class Solver
{
    struct State
    {
        uint_fast32_t field;

        uint16_t heuristic;
        uint16_t move_to;

        State(uint_fast32_t f, uint16_t _priority, uint16_t moves)
            : field(f)
            , heuristic(_priority)
            , move_to(moves)
        {
        }
    };

public:
    void        setTile(uint16_t pos, uint16_t value);
    std::string Solve();

private:
    void        makeMove(uint_fast32_t& field, uint16_t pos);

    uint16_t    getTile(uint_fast32_t field, uint16_t pos) const;
    uint16_t    getZeroPos(uint_fast32_t field);
    auto        getMoves(uint_fast32_t field);
    uint16_t    getHeuristic(uint_fast32_t field);

    void        changeTile(uint_fast32_t& field, uint16_t pos, uint16_t value);

    bool        isSolvable() const;

    uint_fast32_t start_field = 0;

    struct comp
    {
        bool operator()(const State& a, const State& b)
        {
            return b.heuristic < a.heuristic;
        }
    };
};

void Solver::makeMove(uint_fast32_t& field, uint16_t pos)
{
    changeTile(field, getZeroPos(field), getTile(field, pos));
    changeTile(field, pos, 0);
}

uint16_t Solver::getZeroPos(uint_fast32_t field)
{
    for (uint16_t i = 0; i < N * N; ++i)
    {
        if (getTile(field, i) == 0)
        {
            return i;
        }
    }
}

auto Solver::getMoves(uint_fast32_t field)
{
    std::vector<std::pair<uint_fast32_t, char> > possible_moves;

    if (getZeroPos(field) > N - 1)
    {
        uint_fast32_t temp = field;
        makeMove(temp, getZeroPos(field) - N);

        possible_moves.push_back(std::make_pair(temp, 'D'));
    }

    if (getZeroPos(field) < N * (N - 1))
    {
        uint_fast32_t temp = field;
        makeMove(temp, getZeroPos(field) + N);

        possible_moves.push_back(std::make_pair(temp, 'U'));
    }

    if (getZeroPos(field) % N != 0)
    {
        uint_fast32_t temp = field;
        makeMove(temp, getZeroPos(field) - 1);

        possible_moves.push_back(std::make_pair(temp, 'R'));
    }

    if (getZeroPos(field) % N != N - 1)
    {
        uint_fast32_t temp = field;
        makeMove(temp, getZeroPos(field) + 1);

        possible_moves.push_back(std::make_pair(temp, 'L'));
    }

    return possible_moves;
}

void Solver::changeTile(uint_fast32_t& field, uint16_t pos, uint16_t value)
{
    field = (field & SET_MASK[pos]) | static_cast<uint_fast32_t>(value) << (N * (N * N - pos - 1));
}

void Solver::setTile(uint16_t pos, uint16_t value)
{
    changeTile(start_field, pos, value);
}

uint16_t Solver::getTile(const uint_fast32_t field, uint16_t pos) const
{
    return (field & GET_MASK[pos]) >> (N * (N * N - 1 - pos));
}

uint16_t Solver::getHeuristic(uint_fast32_t field)
{
    uint16_t h = 0;

    for (uint16_t i = 0; i < N * N; ++i)
    {
        uint16_t temp = getTile(field, i);

        if (temp == 0)
        {
            continue;
        }

        h += abs((temp - 1) / N - i / N) + abs((temp - 1) % N - i % N);

        if (i / N == (temp - 1) / N)
        {
            h += std::abs(temp - 1 - i);
        }
    }

    return h;
}

bool Solver::isSolvable() const
{

    uint16_t inv = 0;
    for (uint16_t i = 0; i < N * N; ++i)
    {
        for (uint16_t j = i + 1; j < N * N; ++j)
        {
            uint_fast32_t a = getTile(start_field, i);
            uint_fast32_t b = getTile(start_field, j);

            if (a == 0 || b == 0)
            {
                continue;
            }
            if (a > b)
            {
                inv++;
            }
        }

        if (getTile(start_field, i) == 0)
        {
            inv += 1 + i / N;
        }
    }

    return inv % 2 == 0;
}

std::string Solver::Solve()
{
    if (!isSolvable())
    {
        return "-1";
    }

    std::unordered_map<uint_fast32_t, std::pair<uint_fast32_t, char> > previos_states;
    std::priority_queue<State, std::vector<State>, comp> queue;
    std::unordered_set<uint_fast32_t> visited;

    std::string path;

    queue.push(State(start_field, 0, 0));
    previos_states[start_field] = std::make_pair(-1, 'D');

    while (!queue.empty())
    {
        State current_state = queue.top();
        queue.pop();

        if (queue.size() > 400)
        {
            std::priority_queue<State, std::vector<State>, comp> new_queue;
            for (uint16_t i = 0; i < 200; ++i)
            {
                new_queue.push(queue.top());
                queue.pop();
            }

            queue = new_queue;
        }

        if (current_state.field == SOLVED)
        {
            break;
        }

        if (visited.find(current_state.field) != visited.end())
        {
            continue;
        }

        auto possible_moves = getMoves(current_state.field);

        for (auto move : possible_moves)
        {
            if (visited.find(move.first) != visited.end())
            {
                continue;
            }
            queue.push(State(move.first, 1 + getHeuristic(move.first), current_state.move_to + 1));

            previos_states[move.first] = std::make_pair(current_state.field, move.second);
        }

        visited.insert(current_state.field);
    }

    uint_fast32_t current_state = SOLVED;
    while (current_state != start_field)
    {
        path.push_back(previos_states[current_state].second);
        current_state = previos_states[current_state].first;
    }

    std::reverse(path.begin(), path.end());

    return path;
}


int main()
{
    Solver solver;

    for (uint16_t i = 0; i < N * N; ++i)
    {
        uint16_t temp;
        std::cin >> temp;
        solver.setTile(i, temp);
    }

    std::string res = solver.Solve();
    if (res != "-1")
    {
        std::cout << res.length() << std::endl;
    }
    std::cout << res << std::endl;

    return 0;
}
