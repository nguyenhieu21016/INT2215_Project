#include "SkillObject.h"


bool isVLine(const std::vector<Point>& points)
{
    if (points.size() > 3)
    {
        int x1 = points.front().x, y1 = points.front().y;
        int x2 = points.back().x , y2 = points.back().y;
        //Tính đường thẳng y = ax+b đi qua 2 điểm đầu cuối
        double a = (y1 - y2)/(x1 - x2);
        double b = y1 - a*x1;
        if (abs(a) > 1/4)
        {
            return false;
        }
        Point midP = points[points.size()/2];
        double khoangcach = abs(a*midP.x - midP.y +b)/sqrt(a*a+1);
        if (khoangcach <30)
        {
            return false;
        }
    } else
    {
        return false;
    }
    return true;
}
bool isHorizontalLine(const std::vector<Point>& points)
{
    if (points.size() > 3)
    {
        int x1 = points.front().x, y1 = points.front().y;
        int x2 = points.back().x , y2 = points.back().y;
        //Tính đường thẳng y = ax+b đi qua 2 điểm đầu cuối
        double a = (y1 - y2)/(x1 - x2);
        double b = y1 - a*x1;
        if (abs(a) > 0.176)
        {
            return false;
        }
        for (Point p : points)
        {
            double khoangcach = abs(a*p.x - p.y +b)/sqrt(a*a+1);
            if (khoangcach > 30)
            {
                return false;
            }
        }
    } else
    {
        return false;
    }
    return true;
}

bool isVerticalLine(const std::vector<Point>& points)
{
    if (points.size() > 3)
    {
        int x1 = points.front().x, y1 = points.front().y;
        int x2 = points.back().x , y2 = points.back().y;
        //Tính đường thẳng y = ax+b đi qua 2 điểm đầu cuối
        double a = (y1 - y2)/(x1 - x2);
        double b = y1 - a*x1;
        if (abs(a) > 5.76)
        {
            return false;
        }
        for (Point p : points)
        {
            double khoangcach = abs(a*p.x - p.y +b)/sqrt(a*a+1);
            if (khoangcach > 30)
            {
                return false;
            }
        }
    } else
    {
        return false;
    }
    return true;
}

