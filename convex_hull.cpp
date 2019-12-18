#include <algorithm>
#include <iomanip>
#include <iostream>
#include <cmath>
#include <vector>

struct Point 
{
	double x;
    double y;

	bool operator <(const Point &p) const 
    {
		return x < p.x || (x == p.x && y < p.y);
	}
};

double SquaredDistance(const Point& first, const Point& second)
{
    return (first.x - second.x) * (first.x - second.x) + (first.y - second.y) * (first.y - second.y);
}

double crossProduct(const Point &O, const Point &A, const Point &B)
{
	return (A.x - O.x) * (B.y - O.y) - (A.y - O.y) * (B.x - O.x);
}

std::vector<Point> convexHull(std::vector<Point> points)
{
	if (points.size() <= 3)
    {
        return points;
    }
	std::vector<Point> hull(points.size());

	sort(points.begin(), points.end());

	size_t hull_size = 0;
	for (size_t i = 0; i < points.size(); ++i) 
    {
		while (hull_size >= 2 && crossProduct(hull[hull_size-2], hull[hull_size-1], points[i]) <= 0)
        {
            hull_size--;
        }
		hull[hull_size++] = points[i];
	}

	for (size_t i = points.size()-1, t = hull_size+1; i > 0; --i) 
    {
		while (hull_size >= t && crossProduct(hull[hull_size-2], hull[hull_size-1], points[i-1]) <= 0)
        {
            hull_size--;
        }
		hull[hull_size++] = points[i-1];
	}
	hull.resize(hull_size-1);

	return hull;
}

long double Perimeter(const std::vector<Point>& points)
{
    long double P = 0.0;
    for (size_t i = 0; i < points.size(); ++i)
    {
        if(i == points.size() - 1)
        {
            P += sqrt(SquaredDistance(points[i], points.front()));
            break;
        }
        P += sqrt(SquaredDistance(points[i], points[i+1]));
    }
    
    return P;
}

int main()
{
    std::vector<Point> points;
    size_t N;
    std::cin>>N;
    std::cout<<std::setprecision(10);

    for (size_t i = 0; i < N; i++)
    {
        double X, Y;
        std::cin>>X>>Y;
        points.push_back({X, Y});
    }
    
    std::cout<<Perimeter(convexHull(points)); 

    return 0;
}