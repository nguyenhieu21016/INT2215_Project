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
void spawnEnemy(std::vector <EnemyObject>& enemies)
{
    if (SDL_GetTicks() > lastSpawnTime + 3000)
    {
        int y = rand() % 571 + 1;
        int x = (rand() % 2 == 0) ? 0 : 1130;
        std::vector<char> skills = generateRandomSkill();
        EnemyObject newEnemy(x, y, skills);
        enemies.push_back(newEnemy);
        std::cout << "Spawn : " << x << " " << y << std::endl;
        lastSpawnTime = SDL_GetTicks();

    }
}
void EnemyObject::show(int x, int y, SDL_Texture* enemyTexture)
{
    SDL_Rect renderQuad = {x, y, 150, 150};
    SDL_RenderCopy(gRenderer, enemyTexture, NULL, &renderQuad);
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

