#include "SkillObject.h"


bool isVLine(const std::vector<Point>& points)
{
    if (points.size() > 3)
    {
        int x1 = points.front().x, y1 = points.front().y;
        int x2 = points.back().x , y2 = points.back().y;
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
bool isLightningLine(const std::vector<Point>& points)
{
    if (points.size() > 3)
    {
        int x1 = points.front().x, y1 = points.front().y;
        int x2 = points.front().x, y2 = points.front().x;
        double a = (y1-y2)/(x1-x2);
        double b = y1 - a*x1;
        Point point14 = points[points.size()/4];
        Point point34 = points[points.size()*3/4];
        if ((a*point14.x-point14.y+b)*(a*point34.x-point34.y+b)<0)
        {
            return false;
        }
    }
    return true;
}

