#include "EnemyObject.h"
#include "BaseObject.h"

EnemyObject::EnemyObject(int x0, int y0, std::vector <char> skills)
{
    xpos = x0;
    ypos = y0;
    skillQueue = skills;
    isDying = false;
    isHurt = false;
}

EnemyObject::~EnemyObject()
{
    free();
}

Uint32 lastSpawnTime = 0;
void spawnEnemy(std::vector <EnemyObject>& enemies)
{
    if (SDL_GetTicks() > lastSpawnTime + 2000)
    {
        int y = rand() % 571 + 1;
        int x = (rand() % 2 == 0) ? 0 : 1130;
        std::vector<char> skills = generateRandomSkill();
        EnemyObject newEnemy(x, y, skills);
        enemies.push_back(newEnemy);
        std::cout << "Spawn : " << x << " " << y << " Skill : ";
        for (char c : skills)
        {
            std::cout << c << " ";
        }
        std::cout << std::endl;
        lastSpawnTime = SDL_GetTicks();
    }
}
int frame0_ = 0;
Uint32 last_frame_time0_ = 0;
SDL_Rect clip0[20];
void EnemyObject::show(int& x, int& y, SDL_Texture* enemyTexture, std::vector <SDL_Texture*> skillTexture, SDL_Texture* tex)
{
    SDL_Texture* texture = NULL;
    SDL_Rect renderQuad = {x, y, 150, 150};
    int x0 = x + 40;
    if (!isDying && !isHurt)
    {
        SDL_RenderCopy(gRenderer, enemyTexture, NULL, &renderQuad);
    }
    for (char skill : skillQueue)
    {
        if (skill == '-')
        {
            texture = skillTexture[SYMBOL_HORIZONTAL];
        } else if (skill == 'v')
        {
            texture = skillTexture[SYMBOl_SUNKEN];
        } else if (skill == '|')
        {
            texture = skillTexture[SYMBOL_VERTICAL];
        }
        if (texture != NULL)
        {
            SDL_Rect renderQuad = {x0, y, 24, 24};
            SDL_RenderCopy(gRenderer, texture, NULL, &renderQuad);
        }
        x0+=25;
    }
    if (isHurt)
    {
        for (int j = 0; j < 5; j++)
        {
            clip0[j].x = j * 150;
            clip0[j].y = 0;
            clip0[j].w = 150;
            clip0[j].h = 150;
        }
        SDL_Rect renderQuad = {x, y, 150, 150};
        SDL_RenderCopy(gRenderer, tex, &clip0[frame0_], &renderQuad);
        Uint32 current_time = SDL_GetTicks();
        if (current_time > last_frame_time0_ + 100)
        {
            if (frame0_ < 5)
                frame0_++;
            else
            {
                isHurt = false;
                frame0_ = 0;
                last_frame_time0_ = 0;
            }
            last_frame_time0_ = current_time;
        }
        return; // không render enemy bình thường nếu đang hurt
    }
    if (x < (SCREEN_WIDTH)/2)
    {
        x+=2;
    } else
    {
        x-=2;
    }
    if (y < (SCREEN_HEIGHT)/2)
    {
        y+=1;
    } else
    {
        y-=1;
    }
    
}
std::vector <char> generateRandomSkill() {
    std::vector <char> skillSymbols = {'-', '|', 'v'};
    std::vector <char> result;
    int numSkill = rand() % 4 + 1;
    for(int i = 0; i < numSkill; ++i)
    {
        int randomIndex = rand() % skillSymbols.size();
        result.push_back(skillSymbols[randomIndex]);
    }
    return result;
}

void attack(char skill, std::vector <EnemyObject>& enemies)
{
    for (int i = 0; i < enemies.size(); i++)
    {
        if (!enemies[i].skillQueue.empty())
        {
            if (enemies[i].skillQueue[0] == skill)
            {
                enemies[i].isHurt = true;
                enemies[i].skillQueue.erase(enemies[i].skillQueue.begin());
                std::cout << "đã tấn công quái " << i << std::endl;
            }
        }
    }
}
int frame_ = 0;
Uint32 last_frame_time_ = 0;
SDL_Rect clip[20];
void enemyLive(std::vector <EnemyObject>& enemies, Mix_Chunk* dead, SDL_Texture* tex)
{
    
    for (int i = 0; i < enemies.size(); i++)
    {
        if (enemies[i].skillQueue.empty() && !enemies[i].isHurt)
        {
            enemies[i].isDying = true;
            for (int j = 0; j < 8; j++)
            {
                clip[j].x = j * 150;
                clip[j].y = 0;
                clip[j].w = 150;
                clip[j].h = 150;
            }
            SDL_Rect renderQuad = {enemies[i].xpos, enemies[i].ypos, 150, 150};
            SDL_RenderCopy(gRenderer, tex, &clip[frame_], &renderQuad);
            Uint32 current_time = SDL_GetTicks();
            if (current_time > last_frame_time_ + 100)
            {
                if (frame_ < 8)
                {
                    frame_ += 1;
                } else
                {
                    Mix_PlayChannel(-1, dead, 0);
                    frame_ = 0;
                    last_frame_time_ = 0;
                    enemies.erase(enemies.begin());
                }
                last_frame_time_ = current_time;
            }
        }
    }
}
