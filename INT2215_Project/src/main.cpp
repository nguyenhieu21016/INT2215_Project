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
BaseObject gHurt;
BaseObject gLightning;
BaseObject gHorizontal;
BaseObject gEnemyLeft;
BaseObject gEnemyRight;
BaseObject sLightning;
BaseObject sHorizontal;
BaseObject sSunken;
BaseObject sVertical;
BaseObject sEnemyDieRight;
BaseObject sEnemyDieLeft;
BaseObject sEnemyHurtRight;
BaseObject sEnemyHurtLeft;
SDL_Texture* tsSunken;
SDL_Texture* tsVertical;
SDL_Texture* tsLightning;
SDL_Texture* tsHorizontal;
SDL_Texture* tWaiting;
SDL_Texture* tDrawing;
SDL_Texture* tHurt;
SDL_Texture* tSunken;
SDL_Texture* tVertical;
SDL_Texture* tLightning;
SDL_Texture* tHorizontal;
SDL_Texture* tEnemyLeft;
SDL_Texture* tEnemyRight;
SDL_Texture* tEnemyDỉeRight;
SDL_Texture* tEnemyDỉeLeft;
SDL_Texture* tEnemyHurtRight;
SDL_Texture* tEnemyHurtLeft;
SDL_Texture* digitTextures[10];
std::vector <Point> points;
std::vector <EnemyObject> enemies;
std::vector <SDL_Texture*> skillTexture;
int score = 0;

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
    { &gHurt, "assets/hurt.png", &tHurt},
    { &gHorizontal, "assets/horizontal.png", &tHorizontal},
    { &gEnemyLeft, "assets/ghost_normal_left.png", &tEnemyLeft},
    { &gEnemyRight, "assets/ghost_normal_right.png", &tEnemyRight},
    { &sSunken, "assets/symbols/sSunken.png", &tsSunken},
    { &sVertical, "assets/symbols/sVertical.png", &tsVertical},
    { &sLightning, "assets/symbols/sLightning.png", &tsLightning},
    { &sHorizontal, "assets/symbols/sHorizontal.png", &tsHorizontal},
    { &sEnemyDieRight, "assets/ghost_die_right.png", &tEnemyDỉeRight},
    { &sEnemyDieLeft, "assets/ghost_die_left.png", &tEnemyDỉeLeft},
    { &sEnemyHurtRight, "assets/ghost_hurt_right.png", &tEnemyHurtRight},
    { &sEnemyHurtLeft, "assets/ghost_hurt_left.png", &tEnemyHurtLeft}
};

bool loadMedia()
{
    SDL_Color textColor = {255, 255, 255};
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
    gFont = TTF_OpenFont( "assets/fonts/m6x11.ttf", 28 );
        if( gFont == NULL )
        {
            printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
            success = false;
        }
    for (int i = 0; i < 10; i++)
    {
        std::string digitStr = std::to_string(i);
        SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, digitStr.c_str(), textColor );
            if( textSurface == NULL )
            {
                printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
            }
            else
            {
                digitTextures[i] = SDL_CreateTextureFromSurface( gRenderer, textSurface );
                SDL_FreeSurface(textSurface);
            }
    }
    skillTexture.push_back(tsSunken);
    skillTexture.push_back(tsLightning);
    skillTexture.push_back(tsVertical);
    skillTexture.push_back(tsHorizontal);
    return success;
}
int main(int argc, char* argv[])
{
    ImpTimer fps_timer;
    if (!InitData()) return -1;
    if (!loadMedia()) return -1;
    Mix_Music* bgm = Mix_LoadMUS("assets/bgm.mp3");
    Mix_Chunk* hit = Mix_LoadWAV("assets/hit.mp3");
    Mix_Chunk* dead = Mix_LoadWAV("assets/dead.mp3");
    Mix_VolumeChunk(hit, MIX_MAX_VOLUME / 4);
    Mix_VolumeChunk(dead, MIX_MAX_VOLUME);

    if (!bgm) {
        std::cout << "Không load được nhạc nền: " << Mix_GetError() << std::endl;
        return -1;
    }
    Mix_PlayMusic(bgm, -1);
    bool drawingMode = false;
    bool isRunning = true;
    gPlayer.setWaitingTexture(tWaiting);
    gPlayer.setTexture(tWaiting);
    gPlayer.set_clips(WAITING_ANIMATION_FRAMES);
    while (isRunning)
    {
        fps_timer.start();
        while (SDL_PollEvent(&gEvent))
        {
            if (gEvent.type == SDL_QUIT)
            {
                isRunning = false;
            }
            else if (gEvent.type == SDL_MOUSEBUTTONDOWN)
            {
                points.clear();
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
                    attack('-', enemies);
                    Mix_PlayChannel(-1, hit, 0);
                } else if (isHeartLine(points))
                {
                    gPlayer.loadFromFile("assets/heart.png");
                    gPlayer.set_clips(HEART_ANIMATION_FRAMES);
                    gPlayer.skill();
                    Mix_PlayChannel(-1, hit, 0);
                    cout << "ĐƯỜNG TRÁI TIM" << endl;
                } else if (isVLine(points))
                {
                    gPlayer.setTexture(tSunken);
                    gPlayer.set_clips(SUNKEN_ANIMATION_FRAMES);
                    gPlayer.skill();
                    attack('v', enemies);
                    Mix_PlayChannel(-1, hit, 0);
                    cout << "CHỮ V" << endl;
                } else if (isVerticalLine(points))
                {
                    gPlayer.setTexture(tVertical);
                    gPlayer.set_clips(VERTICAL_ANIMATION_FRAMES);
                    gPlayer.skill();
                    Mix_PlayChannel(-1, hit, 0);
                    attack('|', enemies);
                    cout << "ĐƯỜNG DỌC" << endl;
                } else if (isLightningLine(points))
                {
                    gPlayer.setTexture(tLightning);
                    gPlayer.set_clips(LIGHTNING_ANIMATION_FRAMES);
                    gPlayer.skill();
                    Mix_PlayChannel(-1, hit, 0);
                    cout << "ĐƯỜNG SÉT" << endl;
                } else
                {
                    cout << "ĐÉO CÓ GÌ" << endl;
                    gPlayer.setTexture(tWaiting);
                    gPlayer.set_clips(WAITING_ANIMATION_FRAMES);
                }
            } else if (gEvent.type == SDL_MOUSEMOTION && drawingMode)
            {
                
                points.push_back({gEvent.motion.x, gEvent.motion.y});
                cout << gEvent.motion.x << " " << gEvent.motion.y << endl;
            }
        }
        SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
        SDL_RenderClear(gRenderer);
        gBackground.render(0, 0, NULL);
        if (points.size()>1)
        {
            for (size_t i = 1; i < points.size(); ++i)
            {
                SDL_RenderDrawLine(gRenderer, points[i-1].x+1, points[i-1].y, points[i].x+1, points[i].y);
                SDL_RenderDrawLine(gRenderer, points[i-1].x+2, points[i-1].y, points[i].x+2, points[i].y);
                SDL_RenderDrawLine(gRenderer, points[i-1].x, points[i-1].y+1, points[i].x, points[i].y+1);
                SDL_RenderDrawLine(gRenderer, points[i-1].x, points[i-1].y+2, points[i].x, points[i].y+2);
                SDL_RenderDrawLine(gRenderer, points[i-1].x-1, points[i-1].y, points[i].x-1, points[i].y);
                SDL_RenderDrawLine(gRenderer, points[i-1].x-2, points[i-1].y, points[i].x-2, points[i].y);
                SDL_RenderDrawLine(gRenderer, points[i-1].x, points[i-1].y-1, points[i].x, points[i].y-1);
                SDL_RenderDrawLine(gRenderer, points[i-1].x, points[i-1].y-2, points[i].x, points[i].y-2);
                SDL_RenderDrawLine(gRenderer, points[i-1].x, points[i-1].y, points[i].x, points[i].y);
                SDL_RenderDrawLine(gRenderer, points[i-1].x+1, points[i-1].y+1, points[i].x+1, points[i].y+1);
                SDL_RenderDrawLine(gRenderer, points[i-1].x-1, points[i-1].y-1, points[i].x-1, points[i].y-1);
                SDL_RenderDrawLine(gRenderer, points[i-1].x+1, points[i-1].y-1, points[i].x+1, points[i].y-1);
                SDL_RenderDrawLine(gRenderer, points[i-1].x-1, points[i-1].y+1, points[i].x-1, points[i].y+1);

            }
        }
        spawnEnemy(enemies);
        for (EnemyObject& enemy : enemies)
        {
            SDL_Rect playerRect = { (SCREEN_WIDTH - gPlayer.getWidth())/2, (SCREEN_HEIGHT - gPlayer.getHeight())/2, gPlayer.getWidth()-90, gPlayer.getHeight()};
            SDL_Rect enemyRect = { enemy.xpos, enemy.ypos, 60, 150 };

            if (checkCollision(playerRect, enemyRect))
            {
                enemy.skillQueue.clear();
                gPlayer.setTexture(tHurt);
                gPlayer.set_clips(HURT_ANIMATION_FRAMES);
                gPlayer.skill();
            }
        }
        enemyLive(enemies, dead, tEnemyDỉeRight, tEnemyDỉeLeft, score);
        renderScore(gRenderer, score, 50, 30, digitTextures);
        for (EnemyObject& enemy : enemies)
        {
            if (enemy.xpos < (SCREEN_WIDTH - FRAME_CHARACTER_WIDTH)/2)
            {
                enemy.show(enemy.xpos, enemy.ypos, tEnemyLeft, skillTexture, tEnemyHurtRight, tEnemyHurtLeft);
            } else
            {
                enemy.show(enemy.xpos, enemy.ypos, tEnemyRight, skillTexture, tEnemyHurtRight, tEnemyHurtLeft);
            }
            
        }
        gPlayer.show();
        SDL_RenderPresent(gRenderer);
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
    gBackground.free();
    gPlayer.free();
    Mix_FreeChunk(hit);
    Mix_FreeMusic(bgm);
    Mix_CloseAudio();
    return 0;
}
