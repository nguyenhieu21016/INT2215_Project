#include "EnemyObject.h"
#include "BaseObject.h"

EnemyObject::EnemyObject(int x0, int y0, std::vector <char> skills)
{
    xpos = x0;
    ypos = y0;
    skillQueue = skills;
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
void EnemyObject::show(int& x, int& y, SDL_Texture* enemyTexture, std::vector <SDL_Texture*> skillTexture)
{
    if (x < (SCREEN_WIDTH - mWidth)/2)
    {
        x+=2;
    } else
    {
        x-=2;
    }
    if (y < (SCREEN_HEIGHT - mHeight)/2)
    {
        y+=2;
    } else
    {
        y-=2;
    }
    SDL_Texture* texture = NULL;
    SDL_Rect renderQuad = {x, y, 150, 150};
    int x0 = x + 40;
    SDL_RenderCopy(gRenderer, enemyTexture, NULL, &renderQuad);
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
                enemies[i].skillQueue.erase(enemies[i].skillQueue.begin());
                std::cout << "đã tấn công quái " << i << std::endl;
            }
        }
    }
}
void enemyLive(std::vector <EnemyObject>& enemies)
{
    for (int i = 0; i < enemies.size(); i++)
    {
        if (enemies[i].skillQueue.empty())
        {
            enemies.erase(enemies.begin()+i);
        }
    }
}

