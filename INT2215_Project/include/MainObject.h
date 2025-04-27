#pragma once

#include "GameBase.h"
#include "BaseObject.h"

//Hằng số frame
const int SUNKEN_ANIMATION_FRAMES = 8;
const int WAITING_ANIMATION_FRAMES = 20;
const int DRAWING_ANIMATION_FRAMES = 6;
const int HORIZONTAL_ANIMATION_FRAMES = 5;
const int VERTICAL_ANIMATION_FRAMES = 5;

class MainObject : public BaseObject
{
public:
    MainObject();
    ~MainObject();
    
    void show();
    void set_clips(int s);
    void waiting();
    void skill();
    SDL_Texture* getTexture();
    
private:
    SDL_Rect gSpriteClips[20];
    enum Animation
    {
        RUNNING, SCARED, WAITING, DRAWING, HEART,
        SUNKEN, HURT, DIE, VICTORY,
    };
    int frame_;
    Uint32 last_frame_time_;
    int status_;
    int total_frames_;
};
