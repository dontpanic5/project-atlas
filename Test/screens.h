#ifndef SCREENS_H
#define SCREENS_H

typedef enum GameScreen { TITLE = 0, GAMEPLAY, GAMEOVER } GameScreen;

extern GameScreen currentScreen;

#ifdef __cplusplus
extern "C" {            // Prevents name mangling of functions
#endif

//----------------------------------------------------------------------------------
// Title Screen Functions Declaration
//----------------------------------------------------------------------------------
void InitTitleScreen(void);
void UpdateTitleScreen(void);
void DrawTitleScreen(void);
void UnloadTitleScreen(void);
int FinishTitleScreen(void);

//----------------------------------------------------------------------------------
// Gameplay Screen Functions Declaration
//----------------------------------------------------------------------------------
void InitGameplayScreen(void);
void UpdateGameplayScreen(void);
void DrawGameplayScreen(void);
void UnloadGameplayScreen(void);
int FinishGameplayScreen(void);

//----------------------------------------------------------------------------------
// GameOver Screen Functions Declaration
//----------------------------------------------------------------------------------
void InitGameOverScreen(void);
void UpdateGameOverScreen(void);
void DrawGameOverScreen(void);
void UnloadGameOverScreen(void);
int FinishGameOverScreen(void);
	
#ifdef __cplusplus
}
#endif

#endif // !SCREENS_H