#include "GameBase.h"
#include "BaseObject.h"
#include "MainObject.h"
#include "SkillObject.h"
#include "EnemyObject.h"

using namespace std;
MainObject gPlayer;
BaseObject gBackground;
BaseObject gWaiting;
BaseObject gSunken;
BaseObject gVertical;
BaseObject gDrawing;
BaseObject gLightning;
BaseObject gHorizontal;
SDL_Texture* tWaiting;
SDL_Texture* tDrawing;
SDL_Texture* tSunken;
SDL_Texture* tVertical;
SDL_Texture* tLightning;
SDL_Texture* tHorizontal;
std::vector <Point> points;
std::vector <EnemyObject> enemies;

struct LoadAsset {
    BaseObject* object;
    const char* path;
    SDL_Texture** texturePtr;
};
std::vector <LoadAsset> assets = {
    { &gBackground, "assets/background.png", NULL},
    { &gWaiting, "assets/waiting.png", &tWaiting},
    { &gDrawing, "assets/drawing.png", &tDrawing},
    { &gSunken, "assets/sunken.png", &tSunken},
    { &gVertical, "assets/vertical.png", &tVertical},
    { &gLightning, "assets/lightning.png", &tLightning},
    { &gHorizontal, "assets/horizontal.png", &tHorizontal}
};

bool loadMedia()
{
    bool success = true;
    for (const auto& asset : assets)
    {
        if (!asset.object->loadFromFile(asset.path))
        {
            success = false;
        } else if (asset.texturePtr)
        {
            *asset.texturePtr = asset.object->getTexture();
        }
    }
    return success;
}
int main(int argc, char* argv[])
{
    //Căn chỉnh FPS
    ImpTimer fps_timer;
    if (!InitData()) return -1;
    if (!loadMedia()) return -1;
    bool drawingMode = false;
    bool isRunning = true;
    gPlayer.setWaitingTexture(tWaiting);
    gPlayer.setTexture(tWaiting);
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
                drawingMode = true;
                gPlayer.setTexture(tDrawing);
                gPlayer.set_clips(DRAWING_ANIMATION_FRAMES);
                gPlayer.loop();
            } else if (gEvent.type == SDL_MOUSEBUTTONUP)
            {
                drawingMode = false;
                gPlayer.setTexture(tWaiting);
                gPlayer.set_clips(WAITING_ANIMATION_FRAMES);
                if (isHorizontalLine(points))
                {
                    gPlayer.setTexture(tHorizontal);
                    gPlayer.set_clips(HORIZONTAL_ANIMATION_FRAMES);
                    gPlayer.skill();
                    cout << "ĐƯỜNG NGANG" << endl;
                } else if (isHeartLine(points))
                {
                    gPlayer.loadFromFile("assets/heart.png");
                    gPlayer.set_clips(HEART_ANIMATION_FRAMES);
                    gPlayer.skill();
                    cout << "ĐƯỜNG TRÁI TIM" << endl;
                } else if (isVLine(points))
                {
                    gPlayer.setTexture(tSunken);
                    gPlayer.set_clips(SUNKEN_ANIMATION_FRAMES);
                    gPlayer.skill();
                    cout << "CHỮ V" << endl;
                } else if (isVerticalLine(points))
                {
                    gPlayer.loadFromFile("assets/vertical.png");
                    gPlayer.set_clips(VERTICAL_ANIMATION_FRAMES);
                    gPlayer.skill();
                    cout << "ĐƯỜNG DỌC" << endl;
                } else if (isLightningLine(points))
                {
                    gPlayer.setTexture(tLightning);
                    gPlayer.set_clips(LIGHTNING_ANIMATION_FRAMES);
                    gPlayer.skill();
                    cout << "ĐƯỜNG SÉT" << endl;
                } else
                {
                    cout << "ĐÉO CÓ GÌ" << endl;
                    gPlayer.setTexture(tWaiting);
                    gPlayer.set_clips(WAITING_ANIMATION_FRAMES);
                }
            } else if (gEvent.type == SDL_MOUSEMOTION && drawingMode)
            {
                
                //Đẩy toạ độ hiện tại của chuột vào vector points
                points.push_back({gEvent.motion.x, gEvent.motion.y});
                cout << gEvent.motion.x << " " << gEvent.motion.y << endl;
            }
        }
        //Xoá màn hình với màu trắng
        SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
        SDL_RenderClear(gRenderer);
        //Vẽ background full màn hình
        gBackground.render(0, 0, NULL);
        //Render skill
        //Vẽ nhân vật
        if (points.size()>1)
        {
            for (size_t i = 1; i < points.size(); ++i)
            {
                SDL_RenderDrawLine(gRenderer, points[i-1].x, points[i-1].y, points[i].x, points[i].y);
            }
        }
        gPlayer.show();
        //spawnEnemy(enemies);
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
