#pragma once

#include "GameBase.h"
#include "BaseObject.h"

//Hằng số frame
const int SUNKEN_ANIMATION_FRAMES = 8;
const int WAITING_ANIMATION_FRAMES = 11;
const int DRAWING_ANIMATION_FRAMES = 6;
const int HORIZONTAL_ANIMATION_FRAMES = 5;
const int VERTICAL_ANIMATION_FRAMES = 5;
const int LIGHTNING_ANIMATION_FRAMES = 5;
const int HEART_ANIMATION_FRAMES = 5;
const int HURT_ANIMATION_FRAMES = 4;


class MainObject : public BaseObject
{
public:
    MainObject();
    ~MainObject();
    void show();
    void set_clips(int s);
    void loop();
    void skill();
    void setWaitingTexture(SDL_Texture* textureToLoad);
private:
    SDL_Rect gSpriteClips[20];
    SDL_Texture* waitingTexture;
    int frame_;
    Uint32 last_frame_time_;
    int status_;
    int total_frames_;
};
