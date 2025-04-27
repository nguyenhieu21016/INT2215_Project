#include "EnemyObject.h"

EnemyObject::EnemyObject(int x0, int y0, std::vector <char> skills)
{
    xpos = x0;
    ypos = y0;
    skillQueue = skills;
}

EnemyObject::~EnemyObject()
{
}
Uint32 lastSpawnTime = 0;
void spawnEnemy(std::vector <EnemyObject> enemies)
{
    if (SDL_GetTicks() > lastSpawnTime + 3000)
    {
        int x = rand() % SCREEN_WIDTH;
        int y = rand() % SCREEN_HEIGHT;
        std::vector<char> skills = generateRandomSkill();
        enemies.push_back(EnemyObject(x, y, skills));
        std::cout << x << " " << y << std::endl;
        lastSpawnTime = SDL_GetTicks();
    }
}
std::vector <char> generateRandomSkill() {
    std::vector <char> skillSymbols = {'-', '|', 'v'};
    std::vector <char> result;
    int numSkill = rand() % 3;
    for(int i = 0; i < numSkill; ++i)
    {
        int randomIndex = rand() % skillSymbols.size();
        result.push_back(skillSymbols[randomIndex]);
    }
    return result;
}

