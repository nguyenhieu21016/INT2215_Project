// GameBase.h: Khai báo các biến toàn cục và các hàm xử lý game cơ bản
#pragma once

#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_mixer/SDL_mixer.h>
#include <SDL2_ttf/SDL_ttf.h>
#include <fstream>

// === Biến toàn cục ===
extern SDL_Window* gWindow;
extern SDL_Renderer* gRenderer;
static SDL_Event gEvent;
extern TTF_Font* gFont;
extern TTF_Font* gFontBig;


// === Các tham số cấu hình game ===
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

// === Các trạng thái animation của nhân vật ===
enum Animation
{
    RUNNING, SCARED, WAITING, DRAWING, HEART,
    SUNKEN, HURT, DIE, VICTORY,
};


//Lưu điểm
void saveBestScore(int score);
int loadBestScore();


//Xử lí FPS
// === Lớp quản lý thời gian FPS ===
class ImpTimer
{
public:
    ImpTimer();
    ~ImpTimer();
    
    // Bắt đầu đếm thời gian
    void start();
    // Dừng đếm thời gian
    void stop();
    // Tạm dừng đếm thời gian
    void pause();
    // Tiếp tục đếm thời gian
    void unpause();
    
    // Lấy số mili-giây đã trôi qua
    int get_ticks();
    
    // Kiểm tra thời gian đã bắt đầu
    bool is_started();
    // Kiểm tra thời gian bị tạm dừng
    bool is_paused();


private:
    int start_tick_;
    int paused_tick_;
    
    bool is_paused_;
    bool is_started_;
};
