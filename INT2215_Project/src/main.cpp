#include "GameBase.h"
#include "BaseObject.h"
#include "MainObject.h"
#include "SkillObject.h"
#include "EnemyObject.h"

using namespace std;
MainObject gPlayer;
BaseObject gBackground;
std::vector <Point> points;
std::vector <EnemyObject> enemies;
bool loadMedia()
{
    bool success = true;
    
    if (!gBackground.loadFromFile("assets/background.png"))
    {
        success = false;
    }
    
    return success;
}


int main(int argc, char* argv[])
{
    //Căn chỉnh FPS
    ImpTimer fps_timer;
    if (!InitData()) return -1;
    if (!loadMedia()) return -1;
 
    
    bool drawing = false;
    bool isRunning = true;

    gPlayer.loadFromFile("assets/waiting.png");
    gPlayer.waiting();
    gPlayer.set_clips(WAITING_ANIMATION_FRAMES);
    
    //Vòng lặp chính
    while (isRunning)
    {
        fps_timer.start();
        while (SDL_PollEvent(&gEvent))
        {
            if (gEvent.type == SDL_QUIT) //Nhấn x thì thoát
            {
                isRunning = false;
            }
            else if (gEvent.type == SDL_MOUSEBUTTONDOWN)
            {
                //Xoá hết bảng point cũ
                points.clear();
                //Bật chế độ vẽ
                drawing = true;
                gPlayer.loadFromFile("assets/drawing.png");
                
                gPlayer.set_clips(DRAWING_ANIMATION_FRAMES);
            } else if (gEvent.type == SDL_MOUSEBUTTONUP)
            {
                //Tắt chế độ vẽ
                drawing = false;
                gPlayer.loadFromFile("assets/waiting.png");
                gPlayer.set_clips(WAITING_ANIMATION_FRAMES);
                if (isHorizontalLine(points)) //Nhận diện đường thẳng ngang
                {
                    gPlayer.loadFromFile("assets/horizontal.png");
                    gPlayer.set_clips(HORIZONTAL_ANIMATION_FRAMES);
                    gPlayer.skill();
                    cout << "ĐƯỜNG NGANG" << endl;
                } else if (isVLine(points)) //Nhận diện chữ V
                {
    
                    gPlayer.loadFromFile("assets/sunken.png");
                    gPlayer.set_clips(SUNKEN_ANIMATION_FRAMES);
                    gPlayer.skill();
                    cout << "CHỮ V" << endl;
                } else if (isVerticalLine(points)) //Nhận diện đường thẳng dọc
                {
                    gPlayer.loadFromFile("assets/vertical.png");
                    gPlayer.set_clips(VERTICAL_ANIMATION_FRAMES);
                    gPlayer.skill();
                    cout << "ĐƯỜNG DỌC" << endl;
                } else //Không nhận gì
                {
                    cout << "ĐÉO CÓ GÌ" << endl;
                }
            } else if (gEvent.type == SDL_MOUSEMOTION && drawing)
            {
                //Đẩy toạ độ hiện tại của chuột vào vector points
                points.push_back({gEvent.motion.x, gEvent.motion.y});
                //In ra toạ độ hiện tại của chuột
                cout << gEvent.motion.x << " " << gEvent.motion.y << endl;
            }
        }
        //Vẽ lên màn hình
        //Xoá màn hình với màu trắng
        SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
        SDL_RenderClear(gRenderer);
        //Vẽ background full màn hình
        gBackground.render(0, 0, NULL);
        //Render skill
        if (points.size() > 3)
        {
            //gSkill.render(SCREEN_WIDTH/2 - 100, SCREEN_HEIGHT/2 - 100, 200, 200, NULL);
        }
        //Vẽ nhân vật
        gPlayer.show();
        spawnEnemy(enemies);
        //Cập nhật khung hình mới lên màn hình
        SDL_RenderPresent(gRenderer);

        //Căn chỉnh FPS
        int real_time = fps_timer.get_ticks();
        int time_one_frame = 1000 / FRAME_PER_SECOND;
        if (real_time < time_one_frame)
        {
            int delay_time = time_one_frame - real_time;
            if (delay_time >= 0)
            {
                SDL_Delay(delay_time);
            }
        }
    }
    //Dọn
    gBackground.free();
    gPlayer.free();
    return 0;
}
