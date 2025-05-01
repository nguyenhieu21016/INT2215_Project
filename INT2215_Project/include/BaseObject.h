#pragma once

#include "GameBase.h"

class BaseObject
{
public:
    BaseObject();
    ~BaseObject();
    bool loadFromFile(std::string path);
    void render(int x, int y, const SDL_Rect* clip);
    void free();
    int getWidth();
    int getHeight();
    SDL_Texture* getTexture();
    void setTexture(SDL_Texture* textureToLoad);
private:

protected:
    SDL_Texture* mTexture;
    int mWidth;
    int mHeight;
    SDL_Rect rect_;
};



