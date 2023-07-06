/*******************************************************************************************
*   Aluno: Arthur Emanuel De Sales Ferreira
*   ABI - Ciencia da Computação
*
*   Projeto de jogo A ghost story
*   Jogo baseado no minigame "Mall of the Spook", do jogo Spooky's House Of Jumpscares
*
*   Feito no Raylib   
*
********************************************************************************************/
#include "raylib.h"

#define MAX_PLAYERS 1 
#define MAX_ENTITIES 7
#define PLAYER_SPD 200.0f
#define MAX_SCORES 7
#define MAX_INPUT_CHARS     15

typedef enum GameScreen { TITLE = 0, GAMEPLAY, GAMEOVER, NEWSCORE, SCORES} GameScreen;
typedef enum MenuOptions { START = 0, HIGHSCORES, EXIT} MenuOptions;

typedef struct player {
    bool attack;
    Vector2 position;
    float speed;
    Rectangle collision;
    Rectangle health;
    int Pscore;
} player;

typedef struct Enemy {
    bool dead;
    Texture2D enemy;
    Vector2 position;
    float speed;
    Rectangle collision;
} Enemy;
static struct Enemy enemy[MAX_ENTITIES] = {0};

typedef enum {
    STORAGE_POSITION_SCORE      = 0,
    STORAGE_POSITION_HISCORE1    = 1,
    STORAGE_POSITION_HISCORE2    = 2,
    STORAGE_POSITION_HISCORE3    = 3,
    STORAGE_POSITION_HISCORE4    = 4,
    STORAGE_POSITION_HISCORE5    = 5,
    STORAGE_POSITION_HISCORE6    = 6,
    STORAGE_POSITION_HISCORE7    = 7    
} StorageData;

void UpdatePlayer(player *player, float delta);

bool playerEnemyCollision;
float gamespeed;
int framesCounter = 0;


int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 768;
    const int screenHeight = 640;

    InitWindow(screenWidth, screenHeight, "GhostStory");

    GameScreen currentScreen = TITLE;
    MenuOptions currentOption = START;
    
    //Textures
    Texture2D menu = LoadTexture("textures/Menu.png");
    Texture2D knife = LoadTexture("textures/pointer.png");
    Texture2D midground = LoadTexture("textures/foreground.png");
    Texture2D foreground = LoadTexture("textures/backgroundCloser.png");
    Texture2D Spooky = LoadTexture("textures/Ghost.png");
    Texture2D HealthBarEmpty = LoadTexture("textures/HealthBarEmpty.png");
    Texture2D HealthBarFrame = LoadTexture("textures/HealthBarFrame.png");
    Texture2D Enemy = LoadTexture("textures/Enemy1.png");
    Texture2D DeadEnemy = LoadTexture("textures/DeadEnemy.png");
    Texture2D GameOver = LoadTexture("textures/GameOver.png");

    Font scoreFont = LoadFont("fonts/alpha_beta.png");
    
    float scrollingMid = 0.0f;
    float scrollingFore = 0.0f;
    gamespeed = 1.0f;
    int score = 0;
    
    int HighScore1 = 0;
    int HighScore2 = 0;
    int HighScore3 = 0;
    int HighScore4 = 0;
    int HighScore5 = 0;
    int HighScore6 = 0;
    int HighScore7 = 0;
    
    char name[MAX_INPUT_CHARS + 1] = "Enter Name\0";      // NOTE: One extra space required for null terminator char '\0'
    int letterCount = 9;

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
    player player = {0};
    player.position = (Vector2){134, 442};
    player.speed = 0;
    
    player.health = (Rectangle){60, 78, 282, 29};
    player.attack = false;
    
    for (int i =0; i < MAX_ENTITIES; i++)
    {        
        enemy[i].position = (Vector2){GetRandomValue(768, 2768), GetRandomValue(356, 506)};
        enemy[i].collision = (Rectangle){enemy[i].position.x, enemy[i].position.y, 70.0f, 134.0f};
        enemy[i].speed = 1.0f;
        enemy[i].dead = false;        
    }    
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        switch(currentScreen)
        {
            case TITLE:
            {               
                // TODO: Update TITLE screen variables here!
                HideCursor();                
                switch(currentOption)
                    {
                      case START:
                      {                                               
                        if (IsKeyPressed(KEY_ENTER))
                        {
                         currentScreen = GAMEPLAY;   
                        }
                        if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))
                        {
                            currentOption = HIGHSCORES;                                      
                        }
                        if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))
                        {
                            currentOption = EXIT;                                      
                        }
                      }break;
                      case HIGHSCORES:
                      {
                         if (IsKeyPressed(KEY_ENTER))
                        {
                         currentScreen = SCORES;   
                        }
                        if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))
                        {                            
                            currentOption = EXIT;                                       
                        }
                        if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))
                        {
                            currentOption = START;                                      
                        }
                      }break;                      
                      case EXIT:
                      {
                        if (IsKeyPressed(KEY_ENTER))
                        {
                          CloseWindow();                            
                        }
                        if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))
                        {                            
                            currentOption = START;                                       
                        }
                        if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))
                        {
                            currentOption = HIGHSCORES;                                      
                        }
                      }break;
                    }                
            } break;
            case GAMEPLAY:
            {
                // TODO: Update GAMEPLAY screen variables here!
                HideCursor();
                float deltaTime = GetFrameTime();
                UpdatePlayer(&player, deltaTime);
                player.health.width -= (enemy->speed/4)*gamespeed; 
                if (IsKeyPressed(KEY_SPACE))
                {
                    player.attack = true; 
                } else {
                    player.attack = false;
                }
                
                for (int i = 0; i < MAX_ENTITIES; i++)
                {
                    enemy[i].position.x -= enemy[i].speed*gamespeed;
                    player.collision = (Rectangle){player.position.x,player.position.y, Spooky.width, 64.0f};
                    if (player.attack == true && CheckCollisionRecs(enemy[i].collision, player.collision))
                    {
                        enemy[i].dead = true;
                        player.health.width += 60;           
                        gamespeed += 0.2f;                           
                    }
                    if (enemy[i].position.x == -70) 
                    {
                        enemy[i].dead = false;
                        enemy[i].position.x = GetRandomValue(768, 800);
                        enemy[i].position.y = GetRandomValue(356, 506);
                    }                                       
                }               
                
                int currentScore = framesCounter;                 
                framesCounter++;
                
                scrollingMid -= 0.5f*gamespeed;
                scrollingFore -= 1.0f*gamespeed;

                if (scrollingMid <= -midground.width) scrollingMid = 0;
                if (scrollingFore <= -foreground.width) scrollingFore = 0;
                
                if (player.health.width == 0) 
                {
                    SaveStorageValue(STORAGE_POSITION_SCORE, currentScore);
                    currentScore = 0;
                    framesCounter = 0;
                    currentScreen = GAMEOVER;
                    player.health.width = 282;                                        
                }                
            break;    
            } break;
            case GAMEOVER:
            {
                HideCursor();
                framesCounter++;
                if (framesCounter > 120)
                {                    
                    currentScreen = SCORES;                   
                }                                       
            }break;
            case SCORES:
            {
                // TODO: Update ENDING screen variables here!
                score = LoadStorageValue(STORAGE_POSITION_SCORE);
                
                HighScore1 = LoadStorageValue(STORAGE_POSITION_HISCORE1);
                HighScore2 = LoadStorageValue(STORAGE_POSITION_HISCORE2);
                HighScore3 = LoadStorageValue(STORAGE_POSITION_HISCORE3);
                HighScore4 = LoadStorageValue(STORAGE_POSITION_HISCORE4);
                HighScore5 = LoadStorageValue(STORAGE_POSITION_HISCORE5);
                HighScore6 = LoadStorageValue(STORAGE_POSITION_HISCORE6);
                HighScore7 = LoadStorageValue(STORAGE_POSITION_HISCORE7);
                
                int key = GetCharPressed();

                // Check if more characters have been pressed on the same frame
                while (key > 0)
                {
                // NOTE: Only allow keys in range [32..125]
                    if ((key >= 32) && (key <= 125) && (letterCount < MAX_INPUT_CHARS))
                    {
                    name[letterCount] = (char)key;
                    name[letterCount+1] = '\0'; // Add null terminator at the end of the string.
                    letterCount++;
                    }

                    key = GetCharPressed();  // Check next character in the queue
                }

                if (IsKeyPressed(KEY_BACKSPACE))
                {
                    letterCount--;
                    if (letterCount < 0) letterCount = 0;
                    name[letterCount] = '\0';
                }

                if (IsKeyPressed(KEY_ENTER))
                {
                    if (score > HighScore1) SaveStorageValue(STORAGE_POSITION_HISCORE1, score);
                    else if (score < HighScore1 && score > HighScore2) SaveStorageValue(STORAGE_POSITION_HISCORE2, score);
                    else if (score < HighScore2 && score > HighScore3) SaveStorageValue(STORAGE_POSITION_HISCORE3, score);
                    else if (score < HighScore3 && score > HighScore4) SaveStorageValue(STORAGE_POSITION_HISCORE4, score);
                    else if (score < HighScore4 && score > HighScore5) SaveStorageValue(STORAGE_POSITION_HISCORE5, score);
                    else if (score < HighScore5 && score > HighScore6) SaveStorageValue(STORAGE_POSITION_HISCORE6, score);
                    else if (score < HighScore6 && score > HighScore7) SaveStorageValue(STORAGE_POSITION_HISCORE7, score);
                    else if(score < HighScore7) SaveStorageValue(STORAGE_POSITION_SCORE, 0);
                }
                
                if (IsKeyPressed(KEY_DELETE))
                {
                    SaveStorageValue(STORAGE_POSITION_HISCORE1, 0);
                    SaveStorageValue(STORAGE_POSITION_HISCORE2, 0);
                    SaveStorageValue(STORAGE_POSITION_HISCORE3, 0);
                    SaveStorageValue(STORAGE_POSITION_HISCORE4, 0);
                    SaveStorageValue(STORAGE_POSITION_HISCORE5, 0);
                    SaveStorageValue(STORAGE_POSITION_HISCORE6, 0);
                    SaveStorageValue(STORAGE_POSITION_HISCORE7, 0);
                }
                if (IsKeyPressed(KEY_SPACE))
                {
                    currentScreen = TITLE;
                }               
            } break;
            default: break;
        }
        
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(WHITE);

            switch(currentScreen)
            {
                case TITLE:
                {
                    DrawTexture(menu, 0, 0, WHITE);
                    switch(currentOption)
                    {
                      case START:
                      {
                        DrawTexture (knife,191,356, WHITE);                          
                      }break;
                      case HIGHSCORES:
                      {
                        DrawTexture (knife,151,452, WHITE);                        
                      }break;                      
                      case EXIT:
                      {
                          DrawTexture (knife,206,548, WHITE);
                      }break;
                    }
                    // TODO: Draw TITLE screen here!
                                                            

                } break;
                case GAMEPLAY:
                {
                    // TODO: Draw GAMEPLAY screen here!
                    
                    // Draw midground image twice
                    DrawTextureV(midground, (Vector2){scrollingMid,0 }, WHITE);
                    DrawTextureV(midground, (Vector2){midground.width + scrollingMid,0 }, WHITE);

                    // Draw foreground image twice
                    DrawTextureV(foreground, (Vector2){scrollingFore, 0 }, WHITE);
                    DrawTextureV(foreground, (Vector2){midground.width + scrollingFore,0 }, WHITE);

                    if (player.attack == true){
                        DrawTextureRec(Spooky, (Rectangle){118, 0, 118, 120}, player.position, WHITE);                      
                    }else{
                        DrawTextureRec(Spooky, (Rectangle){0, 0, 118, 120}, player.position, WHITE);                        
                    }

                    DrawTexture(HealthBarEmpty, 18, 28, WHITE);
                    DrawRectangleRec(player.health, RED);
                    DrawTexture(HealthBarFrame, 18, 28, WHITE);
                    for (int i = 0; i < MAX_ENTITIES; i++)
                    {
                        DrawTextureV(Enemy, enemy[i].position, WHITE);
                        if (enemy[i].dead == true)
                        {
                            DrawTextureV(DeadEnemy, (Vector2){enemy[i].position.x, enemy[i].position.y+54}, WHITE);                           
                        }                        
                    }


                } break;
                case GAMEOVER:
                {
                    DrawTexture(GameOver, 0, 0, WHITE);                                                       
                }break;
                case SCORES:
                {
                    // TODO: Draw ENDING screen here!
                    DrawTexture(GameOver, 0, 0, WHITE);
                    
                    DrawTextEx(scoreFont,TextFormat("%s - %i", name, score), (Vector2){380, 320 - 50}, 40.0f, 1.0f, YELLOW);
                    
                    DrawTextEx(scoreFont,TextFormat("SPOOKY - %i", HighScore1), (Vector2){380, 320}, 40.0f, 1.0f, WHITE);
                    DrawTextEx(scoreFont,TextFormat("SPOOKY - %i", HighScore2), (Vector2){380, 320+40}, 40.0f, 1.0f, WHITE);
                    DrawTextEx(scoreFont,TextFormat("SPOOKY - %i", HighScore3), (Vector2){380, 320+80}, 40.0f, 1.0f, WHITE);
                    DrawTextEx(scoreFont,TextFormat("SPOOKY - %i", HighScore4), (Vector2){380, 320+120}, 40.0f, 1.0f, WHITE);
                    DrawTextEx(scoreFont,TextFormat("SPOOKY - %i", HighScore5), (Vector2){380, 320+160}, 40.0f, 1.0f, WHITE);
                    DrawTextEx(scoreFont,TextFormat("SPOOKY - %i", HighScore6), (Vector2){380, 320+200}, 40.0f, 1.0f, WHITE);
                    DrawTextEx(scoreFont,TextFormat("SPOOKY - %i", HighScore7), (Vector2){380, 320+240}, 40.0f, 1.0f, WHITE);

                    
                } break;
                default: break;
            }
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadTexture(midground);   // Unload midground texture
    UnloadTexture(foreground);  // Unload foreground texture

    CloseWindow();              // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
void UpdatePlayer(player *player, float delta)
{    
    if (IsKeyDown(KEY_LEFT)||IsKeyDown(KEY_A)) player->position.x -= PLAYER_SPD*delta;
    if (IsKeyDown(KEY_RIGHT)||IsKeyDown(KEY_D)) player->position.x += PLAYER_SPD*delta;
    if (IsKeyDown(KEY_UP)||IsKeyDown(KEY_W)) player->position.y -= PLAYER_SPD*delta;
    if (IsKeyDown(KEY_DOWN)||IsKeyDown(KEY_S)) player->position.y += PLAYER_SPD*delta;
     
}    