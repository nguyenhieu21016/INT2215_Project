#include "GameBase.h"
#include "BaseObject.h"
#include "MainObject.h"
#include "SkillObject.h"
#include "EnemyObject.h"
#include "ButtonObject.h"

using namespace std;
MainObject gPlayer;
BaseObject gBackground;
BaseObject gMenu;
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
BaseObject sHealth;
BaseObject sVertical;
BaseObject sEnemyDieRight;
BaseObject gMenuBackground;
BaseObject sEnemyDieLeft;
BaseObject sEnemyHurtRight;
BaseObject sEnemyHurtLeft;
ButtonObject bPause(50, 650);
ButtonObject bStart((SCREEN_WIDTH - 300) / 2, (SCREEN_HEIGHT - 60) / 2);
ButtonObject bQuit((SCREEN_WIDTH - 300) / 2, (SCREEN_HEIGHT - 60) / 2 + 90);
ButtonObject bSelect(0,0);
ButtonObject bSelectCorner(0,0);
SDL_Texture* tsSunken;
SDL_Texture* tsVertical;
SDL_Texture* tsHealth;
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
std::vector <SDL_Texture*> hp;
std::vector <EnemyObject> enemies;
std::vector <SDL_Texture*> skillTexture;
int score = 0;
int lastScoreCheckpoint = 0;
int ultimate = 0;
struct LoadAsset {
    BaseObject* object;
    const char* path;
    SDL_Texture** texturePtr;
};
std::vector <LoadAsset> assets = {
    { &gBackground, "assets/background.png", NULL},
    { &gMenu, "assets/menu_title.png", NULL},
    { &gMenuBackground, "assets/backgroundmenu.png", NULL},
    { &bPause, "assets/pause_button.png", NULL},
    { &bStart, "assets/start_button.png", NULL},
    { &bSelect, "assets/select_button.png", NULL},
    { &bSelectCorner, "assets/select_corner.png", NULL},
    { &bQuit, "assets/quit_button.png", NULL},
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
    { &sHealth, "assets/symbols/sHealth.png", &tsHealth},
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
    gFont = TTF_OpenFont( "assets/fonts/m6x11.ttf", 34 );
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
    for (int i = 0; i < 5;  i++)
    {
        hp.push_back(tsHealth);
    }
    return success;
}
int main(int argc, char* argv[])
{
    ImpTimer fps_timer;
    if (!InitData()) return -1;
    if (!loadMedia()) return -1;
    Mix_Music* bgm = Mix_LoadMUS("assets/bgm.mp3");
    Mix_Chunk* pause_bgm = Mix_LoadWAV("assets/pause_bgm.mp3");
    Mix_Chunk* title = Mix_LoadWAV("assets/audio/title.mp3");
    Mix_Chunk* hit = Mix_LoadWAV("assets/hit.mp3");
    Mix_Chunk* select = Mix_LoadWAV("assets/audio/select.mp3");
    Mix_Chunk* selected = Mix_LoadWAV("assets/audio/selected.mp3");
    Mix_Chunk* dead = Mix_LoadWAV("assets/dead.mp3");
    Mix_VolumeChunk(hit, MIX_MAX_VOLUME / 4);
    Mix_VolumeChunk(dead, MIX_MAX_VOLUME);

    if (!bgm) {
        std::cout << "Không load được nhạc nền: " << Mix_GetError() << std::endl;
        return -1;
    }
    bool drawingMode = false;
    bool isRunning = false;
    bool isInMenu = true;
    bool isPaused = false;
    gPlayer.setWaitingTexture(tWaiting);
    gPlayer.setTexture(tWaiting);
    gPlayer.set_clips(WAITING_ANIMATION_FRAMES);
    if (isInMenu)
    {
        isRunning = false;
        Mix_PauseMusic();
        Mix_PlayChannel(-1, title, -1);
    }
    bool selectup = false;
    bool selectdown = false;
    while (isInMenu)
    {
        while (SDL_PollEvent(&gEvent))
        {
            bStart.handleEvent(&gEvent);
            if (gEvent.type == SDL_QUIT)
            {
                isRunning = false;
                isInMenu = false;
            } else if (gEvent.key.keysym.sym == SDLK_RETURN)
            {
                isRunning = true;
                isInMenu = false;
            } else if (gEvent.type == SDL_KEYDOWN && gEvent.key.keysym.sym == SDLK_UP)
            {
                Mix_PlayChannel(-1, select, 0);
                selectup = true;
                selectdown = false;
            } else if (gEvent.type == SDL_KEYDOWN && gEvent.key.keysym.sym == SDLK_DOWN)
            {
                Mix_PlayChannel(-1, select, 0);
                selectup = false;
                selectdown = true;
            }
        }
        SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
        SDL_RenderClear(gRenderer);
        if (bStart.wasClicked())
        {
            
            isRunning = true;
            isInMenu = false;
        }
        if (bQuit.wasClicked())
        {
            SDL_Quit();
        }

        gMenu.render(0, 0, NULL);
        bStart.render((SCREEN_WIDTH - 300) / 2, (SCREEN_HEIGHT - 60) / 2, NULL);
        if (selectup)
        {
            bSelect.render((SCREEN_WIDTH - 300) / 2 - 60, (SCREEN_HEIGHT - 60) / 2, NULL);
            bSelectCorner.render((SCREEN_WIDTH - 400) / 2 , (SCREEN_HEIGHT - 80) / 2, NULL);
        }
        if (selectdown)
        {
            bSelect.render((SCREEN_WIDTH - 300) / 2 - 60, (SCREEN_HEIGHT - 60) / 2 + 90, NULL);
            bSelectCorner.render((SCREEN_WIDTH - 400) / 2 , (SCREEN_HEIGHT - 80) / 2 + 90, NULL);
        }
        bQuit.render((SCREEN_WIDTH - 300) / 2, (SCREEN_HEIGHT - 60) / 2 + 90, NULL);
        SDL_RenderPresent(gRenderer);
    }
    if (isRunning)
    {
        Mix_HaltChannel(-1);
        Mix_PlayChannel(-1, selected, 0);
        Mix_PlayMusic(bgm, -1);
    }
    while (isRunning)
    {
        if (score >=1000 && score / 1000 > lastScoreCheckpoint)
        {
            ultimate += 1;
            lastScoreCheckpoint = score / 1000;
        }
        fps_timer.start();
        if (bPause.wasClicked())
        {
            if (!isPaused)
            {
                isPaused = true;
                Mix_PauseMusic();
                Mix_PlayChannel(-1, pause_bgm, -1);
            }
        }
        while (SDL_PollEvent(&gEvent))
        {
            if (gEvent.type == SDL_QUIT)
            {
                isRunning = false;
            } else if (gEvent.type == SDL_MOUSEBUTTONDOWN)
            {
                if (!isPaused)
                {
                    points.clear();
                    drawingMode = true;
                    gPlayer.setTexture(tDrawing);
                    gPlayer.set_clips(DRAWING_ANIMATION_FRAMES);
                    gPlayer.loop();
                }
            } else if (gEvent.type == SDL_MOUSEBUTTONUP)
            {
                if (!isPaused)
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
                    }  else if (isVLine(points))
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
                        if (ultimate>0)
                        {
                            ultimate-=1;
                            attack('L', enemies);
                        }
                        cout << "ĐƯỜNG SÉT" << endl;
                    } else
                    {
                        cout << "ĐÉO CÓ GÌ" << endl;
                        gPlayer.setTexture(tWaiting);
                        gPlayer.set_clips(WAITING_ANIMATION_FRAMES);
                    }
                }
            } else if (gEvent.type == SDL_MOUSEMOTION && drawingMode)
            {
                if (!isPaused)
                {
                    points.push_back({gEvent.motion.x, gEvent.motion.y});
                    cout << gEvent.motion.x << " " << gEvent.motion.y << endl;
                }
            } else if (gEvent.type == SDL_KEYDOWN)
            {
                if (gEvent.key.keysym.sym == SDLK_p)
                {
                    if (!isPaused)
                    {
                        isPaused = true;
                        Mix_PauseMusic();
                    }
                }
                if (gEvent.key.keysym.sym == SDLK_r)
                {
                    isPaused = false;
                    drawingMode = false;
                    points.clear();
                    gPlayer.setTexture(tWaiting);
                    gPlayer.set_clips(WAITING_ANIMATION_FRAMES);
                    Mix_HaltChannel(-1);
                    Mix_ResumeMusic();
                }
            }
            bPause.handleEvent(&gEvent);
        }
        SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
        SDL_RenderClear(gRenderer);
        if (!isPaused)
        {
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
                
                if (!enemy.hasCollided && checkCollision(playerRect, enemyRect))
                {
                    enemy.hasCollided = true;
                    enemy.skillQueue.clear();
                    gPlayer.setTexture(tHurt);
                    gPlayer.set_clips(HURT_ANIMATION_FRAMES);
                    gPlayer.hurt();
                    hp.erase(hp.begin());
                }
            }
            enemyLive(enemies, dead, tEnemyDỉeRight, tEnemyDỉeLeft, score);
            renderScore(gRenderer, score, 122, 35, digitTextures);
            renderScore(gRenderer, ultimate, 165, 73, digitTextures);
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
            renderHP(hp);
            bPause.render (50, 650, NULL);
        } else
        {
            gMenuBackground.render(0, 0, NULL);
            renderHP(hp);
            renderScore(gRenderer, score, 50, 30, digitTextures);
        }
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
