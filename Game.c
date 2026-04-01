#include "raylib.h"
#include "rcamera.h"
#include "math.h"

#define PLAYER_SIZE 100

int main(void){
    
    int screenHeight = 600;
    int screenWidth = 800;
    InitWindow(screenWidth, screenHeight, "raylib example - Game Project");
    SetTargetFPS(60);
    
    
    //Player Variables
    int playerWidth = 100;
    int playerHeight = 100;
    float playerSpeed = 150.0f;
    Rectangle player = { 400, 400, PLAYER_SIZE, PLAYER_SIZE };
    int lives = 3;
    
    //Block Variables
    
    Rectangle block[50];
    float blockSpeed = 150.0f;
    float spawnTimer = 0.0f;
    float spawnInterval = 2.5f;
    bool blockActive[50];
    int blockHeight = 100;
    int blockWidth = 200;
    for (int i = 0; i < 50; i++) {
        blockActive[i] = 0;
    }
    float zigzagspeed[50];
    enum BlockType { BLOCK_FAST, BLOCK_BIG, BLOCK_ZIGZAG, BLOCK_NORMAL };
    enum BlockType blockType[50];
    
    //Score
    int score = 0;
    float scoreTimer = 0.0f;
    float difficultyMult = 1.0f;
    int nextDifficultyInterval = 10;

    
    
    //Game
    
    enum GameState { MENU, PLAYING, GAME_OVER };
    enum GameState gameState = MENU;
    
    
    
    while (!WindowShouldClose()) {
        
        //dt is time since last frame
        float dt = GetFrameTime();
        
        
        //Movement keys
        
        if (IsKeyDown(KEY_D)) {
            player.x += 7.5f;
        }
        if (IsKeyDown(KEY_A)){
            player.x -= 7.5f;
        }
        
        if (player.x < 0 ) {
            player.x = 0;
        }
        if (player.x + playerWidth > screenWidth) {
            player.x = screenWidth - playerWidth;
        }
        if (player.y < 0) {
            player.y = 0;
        }
        if (player.y + playerHeight > screenHeight) {
            player.y = screenHeight - playerHeight;
        }
        
        // Start Game
        if(gameState == MENU && IsKeyPressed(KEY_SPACE)) {
            gameState = PLAYING;
        }
        
        // Increasing Difficulty
               
        if (score == nextDifficultyInterval) {
            difficultyMult += 0.1;
            spawnInterval = spawnInterval / difficultyMult;
            nextDifficultyInterval += 5;
        }
        
        
       
        //Spawning Blocks    
        
        spawnTimer += dt;
        
        if (gameState == PLAYING && spawnTimer > spawnInterval) {
            
            spawnTimer = 0.0f;        
            
            for (int i = 0; i < 50; i++) {
                if (!blockActive[i]) {
                    
                    blockType[i] = GetRandomValue(0, 3);
                    if (blockType[i] == BLOCK_ZIGZAG) {
                        zigzagspeed[i] = 150.0f;
                    } else {
                        zigzagspeed[i] = 0.0f;
                    }
  
                    if (blockType[i] == BLOCK_BIG) {
                        block[i].width = blockWidth * 1.5f;
                        block[i].height = blockHeight * 1.5f;
                    }
                    else {
                        block[i].width = blockWidth;
                        block[i].height = blockHeight;
                        }
  
                    block[i].x = GetRandomValue(0, screenWidth - block[i].width);
                    block[i].y = -block[i].height;
                    

                    bool spaceFree = true;
                    for (int j = 0; j < 50; j++) {
                        if (blockActive[j] == true && j != i) {
                            if (block[i].x < block[j].x + block[j].width + 25 && block[i].x + block[i].width + 25 > block[j].x) {
                                spaceFree = false;
                            }
                        }
                    }
                                
                    if (spaceFree == true){
                            blockActive[i] = true;
                            break;
                        }
                }
                
            }
        }
                            
                    
                    
                
            
        
                    
                
           
        
        
        
        //Moving blocks
        
        for (int i = 0; i < 50; i++) {
            if (blockActive[i] == true) {
                    switch (blockType[i]) {
                        
                        case BLOCK_FAST:
                            block[i].y += (blockSpeed * 1.5) * dt;
                            break;
                        
                        case BLOCK_BIG:
                            block[i].y += (blockSpeed * 0.7f) * dt;
                            break;
                        
                        case BLOCK_ZIGZAG:
                            block[i].x += zigzagspeed[i] * dt;
                            block[i].y += blockSpeed * dt;
                            break;
                        
                        case BLOCK_NORMAL: 
                            block[i].y += blockSpeed * dt;
                            break;
                    }
            }
        }
        
        //Remove blocks when off screen
        
        for (int i = 0; i < 50; i++) {
            if (blockActive[i] == true && block[i].y > screenHeight) {
                blockActive[i] = false;
                score += 1;
            }
        }
        
        
        
        
        //Collision detection
        
        for (int i = 0; i < 50; i++) {
            if (blockActive[i] == true && block[i].y < player.y + playerHeight
            && block[i].y + block[i].height > player.y && block[i].x < player.x + playerWidth
            && block[i].x + block[i].width > player.x) {
                lives = lives - 1;
                blockActive[i] = false;
                if (lives == 0) {
                    gameState = GAME_OVER;
                }
            }
            
        }
        
        for (int i = 0; i < 50; i++) {
            if (blockType[i] == BLOCK_ZIGZAG && blockActive[i]) {
                if (block[i].x + block[i].width > screenWidth) {
                    zigzagspeed[i] = -zigzagspeed[i];
                }
                if (block[i].x < 0) {
                    zigzagspeed[i] = -zigzagspeed[i];
                }
            }
        }
        
        if (gameState == GAME_OVER) {
            for (int i = 0; i < 50; i++) {
                blockActive[i] = false;
                spawnInterval = 2.5f;
            }                
            if (IsKeyDown(KEY_SPACE)) {
                score = 0;
                lives = 3;
                gameState = PLAYING;
                
            }
            
        }
        
        //Score update
        
        
       
       BeginDrawing();
       
        ClearBackground(LIGHTGRAY);
       
        DrawRectangle(player.x, player.y, playerWidth, playerHeight, RED);
        
        for (int i = 0; i < 50; i++) {
            if (blockActive[i] == true) {
                DrawRectangle(block[i].x, block[i].y, block[i].width, block[i].height, BLUE);
            }
        }
        
        if (gameState == GAME_OVER) {          
           DrawText("Game over!!", 200, 200, 50, BLACK );
           DrawText("Press SPACE to restart the game!", 200, 300, 25, BLACK);
        }
        
        if (gameState == MENU) {
            DrawText("Dodge The Blocks!!", 100, 150, 25, BLACK);
            DrawText("Controls-", 100, 200, 25, BLACK);
            DrawText("A = Left", 100, 250, 25, BLACK);
            DrawText("D = Right", 100, 300, 25, BLACK);
            DrawText("ESC = Quit Game", 100 , 350, 25, BLACK);
            DrawText("Click Space To Start!", 100, 400, 25, BLACK);
        }
        
        DrawText(TextFormat("Score: %i", score), 20, 20, 25, BLACK);
        DrawText(TextFormat("Lives: %i", lives), 20, 60, 25, BLACK);
            
       EndDrawing();
            
        
    }
}

       
       
       
    
    
    
    