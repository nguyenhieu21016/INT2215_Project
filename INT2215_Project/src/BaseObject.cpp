#include "BaseObject.h"

BaseObject::BaseObject()
{
    mTexture = NULL;
    mWidth = 0;
    mHeight= 0;
}

BaseObject::~BaseObject()
{
    free();
}

bool BaseObject::loadFromFile(std::string path)
{
    free();
    SDL_Texture* newTexture = NULL;
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL)
    {
        std::cout << "Load hình ảnh thất bại: " << SDL_GetError() << std::endl;
    }
    else
    {
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if (newTexture != NULL)
        {
            mWidth = loadedSurface->w;
            mHeight = loadedSurface->h;
        }
        SDL_FreeSurface(loadedSurface);
    }
    mTexture = newTexture;
    return (mTexture != NULL);
}
bool BaseObject::loadFromRenderedText(std::string textureText, SDL_Color textColor)
{
    free();
    SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, textureText.c_str(), textColor );
        if( textSurface == NULL )
        {
            printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
        }
        else
        {
            mTexture = SDL_CreateTextureFromSurface( gRenderer, textSurface );
            if( mTexture == NULL )
            {
                printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
            }
            else
            {
                mWidth = textSurface->w;
                mHeight = textSurface->h;
            }

            SDL_FreeSurface( textSurface );
        }
        
    return mTexture != NULL;
}
void BaseObject::render(int x, int y, const SDL_Rect* clip)
{
    if (mTexture == NULL || gRenderer == NULL)
        {
            std::cout << "Lỗi: Texture hoặc Renderer không hợp lệ." << std::endl;
            return;
        }
    SDL_Rect renderQuad = {x, y, mWidth, mHeight};
    //Vẽ texture tại vị trí renderquad
    SDL_RenderCopy(gRenderer, mTexture, clip, &renderQuad);
}

void BaseObject::free()
{
    if (mTexture != NULL)
    {
        SDL_DestroyTexture(mTexture);
        mTexture = NULL;
        mWidth = 0;
        mHeight = 0;
    }
}
void BaseObject::setTexture(SDL_Texture* textureToRender)
{
    mTexture = textureToRender;
    if (mTexture != NULL)
    {
        SDL_QueryTexture(mTexture, NULL, NULL, &mWidth, &mHeight);
    }
}
int BaseObject::getWidth()
{
    return mWidth;
}
int BaseObject::getHeight()
{
    return mHeight;
}
SDL_Texture* BaseObject::getTexture()
{
    return mTexture;
}
void renderScore(SDL_Renderer* renderer, int score, int x, int y, SDL_Texture* digitTextures[10])
{
    std::string scoreStr = std::to_string(score);
    int offsetX = 0;
    for (char c : scoreStr)
    {
        int digit = c - '0';
        int w, h;
        SDL_QueryTexture(digitTextures[digit], NULL, NULL, &w, &h);
        SDL_Rect renderQuad = {x + offsetX, y, w, h};
        SDL_RenderCopy(renderer, digitTextures[digit], NULL, &renderQuad);
        offsetX += w + 3;
    }
}
void renderHP(std::vector <SDL_Texture*> hp)
{
    int offsetX = 1000;
    for (SDL_Texture* tex : hp)
    {
        SDL_Rect renderQuad = {offsetX, 30, 40, 40};
        SDL_RenderCopy(gRenderer, tex, NULL, &renderQuad);
        offsetX+=47;
    }
}
