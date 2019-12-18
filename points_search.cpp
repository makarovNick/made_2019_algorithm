#include <vector>
#include <iostream>

struct Point
{
    double x;
    double y;
    Point(double _x, double _y)
        : x(_x)
        , y(_y)
    {
    }
};

bool isPointInRectengular(const Point& point , const std::pair<Point, Point>& rect)
{
    if(point.x < rect.first.x || point.y < rect.first.y || point.x >= rect.second.x || point.y >= rect.second.y)
    {
        return false;
    }
    
    return true;
}

int main()
{
    size_t N;
    std::cin>>N;
    std::vector<Point> points;
    for (size_t i = 0; i < N; i++)
    {
        double X,Y;
        std::cin>>X>>Y;
        points.push_back(Point(X,Y));
    }
    std::cin>>N;
    std::vector<std::pair<Point, Point>> rects;
    for (size_t i = 0; i < N; i++)
    {
        double x1,y1,x2,y2;
        std::cin>>x1>>y1>>x2>>y2;
        rects.push_back(std::make_pair(Point(x1,y1), Point(x2,y2)));
    }
    std::vector<int> times_in(N, 0);
    for (auto& p : points)
    {
        for (size_t i = 0; i < N; i++)
        {
            if(isPointInRectengular(p, rects[i]))
            {
                times_in[i]++;
            }
        }
    }
    for (auto i : times_in)
    {
        std::cout<<i<<std::endl;
    }
    
}