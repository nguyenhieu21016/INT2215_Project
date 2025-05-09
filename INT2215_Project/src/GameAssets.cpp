#include "GameAssets.h"

// BaseObject
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

// Button
ButtonObject bPause(10, 660);
ButtonObject bContinue(150, 660);
ButtonObject bContinueOverButton(350, 600);
ButtonObject bQuitOverButton(700, 600);
ButtonObject bStart((SCREEN_WIDTH - 300) / 2, (SCREEN_HEIGHT - 60) / 2);
ButtonObject bQuit((SCREEN_WIDTH - 300) / 2, (SCREEN_HEIGHT - 60) / 2 + 90);
ButtonObject bSelect(0,0);
ButtonObject bSelectCorner(0,0);

// Texture
SDL_Texture* tsSunken = nullptr;
SDL_Texture* tsVertical = nullptr;
SDL_Texture* tsHealth = nullptr;
SDL_Texture* tsLightning = nullptr;
SDL_Texture* tsHorizontal = nullptr;
SDL_Texture* tWaiting = nullptr;
SDL_Texture* tDrawing = nullptr;
SDL_Texture* tHurt = nullptr;
SDL_Texture* tSunken = nullptr;
SDL_Texture* tVertical = nullptr;
SDL_Texture* tLightning = nullptr;
SDL_Texture* tHorizontal = nullptr;
SDL_Texture* tEnemyLeft = nullptr;
SDL_Texture* tEnemyRight = nullptr;
SDL_Texture* tEnemyDỉeRight = nullptr;
SDL_Texture* tEnemyDỉeLeft = nullptr;
SDL_Texture* tEnemyHurtRight = nullptr;
SDL_Texture* tEnemyHurtLeft = nullptr;
SDL_Texture* digitTextures[10] = { nullptr };
