// GameAssets.cpp: Khởi tạo và quản lý tài nguyên toàn cục của game
#include "GameAssets.h"

// === Các đối tượng hiển thị (BaseObject / MainObject) ===
MainObject gPlayer;
BaseObject gBackground;
BaseObject gGameOverBackground;
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
BaseObject gBoss;
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

// === Các nút bấm (ButtonObject) ===
ButtonObject pauseButton(10, 660);
ButtonObject continueButton(150, 660);
ButtonObject continueOverButton(350, 600);
ButtonObject quitOverButton(700, 600);
ButtonObject startButton((SCREEN_WIDTH - 300) / 2, (SCREEN_HEIGHT - 60) / 2);
ButtonObject quitButton((SCREEN_WIDTH - 300) / 2, (SCREEN_HEIGHT - 60) / 2 + 90);
ButtonObject selectButton(0, 0);
ButtonObject selectCornerButton(0, 0);
ButtonObject volumeOffButton(200, 600);
ButtonObject volumeOnButton(260, 600);

// === Các texture dùng trong game ===
SDL_Texture* sunkenSymbolTexture    = nullptr;
SDL_Texture* verticalSymbolTexture   = nullptr;
SDL_Texture* healthSymbolTexture     = nullptr;
SDL_Texture* lightningSymbolTexture  = nullptr;
SDL_Texture* horizontalSymbolTexture = nullptr;
SDL_Texture* waitingTexture          = nullptr;
SDL_Texture* drawingTexture          = nullptr;
SDL_Texture* hurtTexture             = nullptr;
SDL_Texture* sunkenTexture           = nullptr;
SDL_Texture* verticalTexture         = nullptr;
SDL_Texture* lightningTexture        = nullptr;
SDL_Texture* horizontalTexture       = nullptr;
SDL_Texture* enemyLeftTexture        = nullptr;
SDL_Texture* enemyRightTexture       = nullptr;
SDL_Texture* bossTexture             = nullptr;
SDL_Texture* enemyDieRightTexture    = nullptr;
SDL_Texture* enemyDieLeftTexture     = nullptr;
SDL_Texture* enemyHurtRightTexture   = nullptr;
SDL_Texture* enemyHurtLeftTexture    = nullptr;
SDL_Texture* digitTextures[10]       = { nullptr };
