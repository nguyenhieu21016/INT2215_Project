// SkillObject.cpp: Nhận diện các hình vẽ tay như đường ngang, dọc, sét
#include "SkillObject.h"


// Kiểm tra xem chuỗi điểm có tạo thành đường xiên gần thẳng đứng không
bool isVLine(const std::vector<Point>& points)
{
    if (points.size() > 3)
    {
        int x1 = points.front().x, y1 = points.front().y;
        int x2 = points.back().x , y2 = points.back().y;
        // Tính hệ số góc và sai số để loại bỏ đường nằm ngang
        double a = (y1 - y2)/(x1 - x2);
        double b = y1 - a*x1;
        if (abs(a) > 1/4)
        {
            return false;
        }
        Point midP = points[points.size()/2];
        // Kiểm tra khoảng cách từ điểm giữa đến đường thẳng → loại đường nằm ngang
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

// Kiểm tra xem chuỗi điểm có tạo thành đường ngang không
bool isHorizontalLine(const std::vector<Point>& points)
{
    if (points.size() <= 3)
        return false;

    int y1 = points.front().y;
    int y2 = points.back().y;

    if (abs(y1 - y2) > 20)
        return false;

    // Kiểm tra độ lệch theo phương y của từng điểm
    for (const Point& p : points)
    {
        if (abs(p.y - y1) > 30)
            return false;
    }

    return true;
}

// Kiểm tra xem chuỗi điểm có tạo thành đường dọc không
bool isVerticalLine(const std::vector<Point>& points)
{
    if (points.size() <= 3)
        return false;

    int x1 = points.front().x;
    int x2 = points.back().x;

    if (abs(x1 - x2) > 20)
        return false;

    // Kiểm tra độ lệch theo phương x của từng điểm
    for (const Point& p : points)
    {
        if (abs(p.x - x1) > 30)
            return false;
    }

    return true;
}

// Kiểm tra xem chuỗi điểm có tạo thành hình tia sét (zigzag) không
bool isLightningLine(const std::vector<Point>& points)
{
    if (points.size() < 5) return false;

    int zigzagCount = 0;

    // Đếm số lần đổi hướng góc lớn giữa các đoạn liên tiếp
    for (size_t i = 2; i < points.size(); ++i)
    {
        int dx1 = points[i - 1].x - points[i - 2].x;
        int dy1 = points[i - 1].y - points[i - 2].y;
        int dx2 = points[i].x - points[i - 1].x;
        int dy2 = points[i].y - points[i - 1].y;

        // Tính góc giữa 2 đoạn bằng tích vô hướng
        float dot = dx1 * dx2 + dy1 * dy2;
        float mag1 = sqrt(dx1 * dx1 + dy1 * dy1);
        float mag2 = sqrt(dx2 * dx2 + dy2 * dy2);

        if (mag1 == 0 || mag2 == 0) continue;

        float cosAngle = dot / (mag1 * mag2);
        float angle = acos(cosAngle) * 180.0 / M_PI;

        // Nếu góc > 45 độ → tính là đổi hướng
        if (angle > 45)
            ++zigzagCount;
    }

    return zigzagCount >= 2;
}

