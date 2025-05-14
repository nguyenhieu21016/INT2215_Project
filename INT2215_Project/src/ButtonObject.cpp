// ButtonObject: Xử lý nút bấm, kiểm tra sự kiện click chuột
#include "ButtonObject.h"

ButtonObject::ButtonObject(int x_, int y_)
{
    xpos = x_;
    ypos = y_;
    isClicked = false;
}
ButtonObject::~ButtonObject()
{}

// Xử lý sự kiện chuột, kiểm tra click vào nút
void ButtonObject::handleEvent(SDL_Event* gEvent)
{
    if (gEvent->type == SDL_MOUSEBUTTONDOWN)
    {
        // Lấy vị trí chuột
        int x, y;
        SDL_GetMouseState(&x, &y);
        // Kiểm tra nếu chuột nằm trong vùng nút
        if (x >= xpos && x <= xpos + mWidth &&
            y >= ypos && y <= ypos + mHeight)
        {
            std::cout << "clicked" << std::endl;
            isClicked = true;
        }
    }
}
// Trả về true nếu nút vừa được nhấn, sau đó reset trạng thái
bool ButtonObject::wasClicked()
{
    if (isClicked)
    {
        isClicked = false;
        return true;
    }
    return false;
}

