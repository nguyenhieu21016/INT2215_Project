#pragma once

#include "GameBase.h"

class BaseObject
{
public:
    BaseObject();
    ~BaseObject();
    bool loadFromFile(std::string path);
    bool loadFromRenderedText(std::string textureText, SDL_Color textColor);
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
void renderScore(SDL_Renderer* renderer, int score, int x, int y, SDL_Texture* digitTextures[10]);
void renderHP(std::vector <SDL_Texture*> hp);


