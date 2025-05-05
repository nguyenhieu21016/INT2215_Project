#include "ButtonObject.h"

ButtonObject::ButtonObject(int x_, int y_)
{
    xpos = x_;
    ypos = y_;
    isClicked = false;
};
ButtonObject::~ButtonObject()
{}

void ButtonObject::handleEvent(SDL_Event* gEvent)
{
    if (gEvent->type == SDL_MOUSEBUTTONDOWN)
    {
        int x, y;
        SDL_GetMouseState(&x, &y);
        if (x >= xpos && x <= xpos + mWidth &&
            y >= ypos && y <= ypos + mHeight)
        {
            isClicked = true;
        }
    }
}
bool ButtonObject::wasClicked()
{
    if (isClicked)
    {
        std::cout << "đm" << std::endl;
        isClicked = false;
        return true;
    }
    return false;
}

