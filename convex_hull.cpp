/*******************************************************/
// 18. Выпуклая оболочка 2D
// Все языки	Python 3.7.3
// Ограничение времени	1 секунда	3 секунды
// Ограничение памяти	5Mb	15Mb
// Ввод	стандартный ввод или input.txt
// Вывод	стандартный вывод или output.txt
// Дано множество точек на плоскости (x, y). Постройте выпуклую оболочку этого множества и вычислите ее периметр.
// Формат ввода
// Количество точек, далее на каждой строке координаты точек x, y.
// Общее количество точек не превосходит 100000. Координаты точек - числа с плавающей точкой в диапазоне [-1000, 1000].
// Формат вывода
// Выведите периметр выпуклой оболочки.
/*******************************************************/
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <cmath>
#include <vector>

struct Point
{
    double x;
    double y;
};

double SquaredDistance(const Point& first, const Point& second)
{
    return (first.x - second.x) * (first.x - second.x)
        + (first.y - second.y) * (first.y - second.y);
}

double crossProduct(const Point& O, const Point& A, const Point& B)
{
    return (A.x - O.x) * (B.y - O.y) - (A.y - O.y) * (B.x - O.x);
}

std::vector<Point> convexHull(std::vector<Point>& points)
{
    if (points.size() <= 3)
    {
        return points;
    }
    std::vector<Point> hull;

    sort(points.begin(), points.end(), [](auto& a, auto&b)
    {
        return a.x < b.x || (a.x == b.x && a.y < b.y);
    });

    for (size_t i = 0; i < points.size(); ++i)
    {
        while (hull.size() >= 2
            && crossProduct(hull[hull.size() - 2], hull[hull.size() - 1], points[i]) <= 0)
        {
            hull.pop_back();
        }
        hull.push_back(points[i]);
    }

    for (size_t i = points.size() - 1, prev_size = hull.size() + 1; i > 0; --i)
    {
        while (hull.size() >= prev_size
            && crossProduct(hull[hull.size() - 2], hull[hull.size() - 1], points[i - 1]) <= 0)
        {
            hull.pop_back();
        }
        hull.push_back(points[i - 1]);
    }

    return hull;
}

double Perimeter(const std::vector<Point>& points)
{
    double P = 0.0;
    for (size_t i = 0; i < points.size(); ++i)
    {
        if (i == points.size() - 1)
        {
            P += sqrt(SquaredDistance(points[i], points.front()));
            break;
        }
        P += sqrt(SquaredDistance(points[i], points[i + 1]));
    }

    return P;
}

int main()
{
    std::vector<Point> points;
    size_t N;
    std::cin >> N;
    std::cout << std::setprecision(10);

    for (size_t i = 0; i < N; ++i)
    {
        double X, Y;
        std::cin >> X >> Y;
        points.push_back({ X, Y });
    }

    std::cout << Perimeter(convexHull(points));

    return 0;
}