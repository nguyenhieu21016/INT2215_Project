#pragma once

#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_mixer/SDL_mixer.h>
#include <SDL2_ttf/SDL_ttf.h>

extern SDL_Window* gWindow;
extern SDL_Renderer* gRenderer;
static SDL_Event gEvent;
extern TTF_Font* gFont;


//Screen
const int FRAME_PER_SECOND = 60;
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const int SCREEN_BPP = 32;
const int FRAME_CHARACTER_WIDTH = 190;
const int FRAME_CHARACTER_HEIGHT = 170;
const int SYMBOl_SUNKEN = 0;
const int SYMBOL_LIGHTNING = 1;
const int SYMBOL_VERTICAL = 2;
const int SYMBOL_HORIZONTAL = 3;

//Base
void close();
bool InitData();
bool checkCollision(SDL_Rect a, SDL_Rect b);
enum Animation
{
    RUNNING, SCARED, WAITING, DRAWING, HEART,
    SUNKEN, HURT, DIE, VICTORY,
};
//Xử lí FPS
class ImpTimer
{
public:
    ImpTimer();
    ~ImpTimer();
    
    void start();
    void stop();
    void pause();
    void unpause();
    
    int get_ticks();
    
    bool is_started();
    bool is_paused();


private:
    int start_tick_;
    int paused_tick_;
    
    bool is_paused_;
    bool is_started_;
};
