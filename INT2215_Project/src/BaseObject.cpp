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
        SDL_FreeSurface(loadedSurface); // giải phóng surface sau khi dùng xong
    }
    
    mTexture = newTexture;
    std::cout << "mWidth: " << mWidth << " mHeight: " << mHeight << std::endl;
    return (mTexture != NULL);
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

int BaseObject::getWidth()
{
    return mWidth;
}
int BaseObject::getHeight()
{
    return mHeight;
}
