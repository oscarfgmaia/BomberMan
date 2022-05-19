#include <raylib.h>
#include <algorithm>
#include <list>
#include <string>
#include <vector>

//------------------------------------------------------------------------------------------
typedef enum GameScreen { LOGO = 0, TITLE, GAMEPLAY, ENDING } GameScreen;
const int kTileSize = 64;

class Bomb {
public:
    Bomb() {}
    Bomb(Color color, Vector2 position) {
        this->color = color;
        this->position = position;
    }
    float timer = 60;
    Color color;
    Vector2 position;
};

class Obstacle {
public:
    Color color = BROWN;
    Vector2 position;
    bool collision = true;
    Obstacle() {

    }
    Obstacle(float x, float y) {
        this->position.x = position.x;
        this->position.y = position.y;
    }
};

class Player {
public:
    Texture2D characterSprite = LoadTexture("textures/character.png");
    Rectangle characterRec;
    Vector2 position;
    Player() {
        characterRec.height = characterSprite.height / 4;
        characterRec.width= characterSprite.width/ 8;
        characterRec.x = 0;
        characterRec.y = 0;
    }
    Player(Vector2 position) { this->position = position; };

    void MoveUp() { position.y -= 1; }
    void MoveDown() { position.y += 1; }
    void MoveRight() { position.x += 1; }
    void MoveLeft() { position.x -= 1; }
    Vector2 GetPositionFixedForSprite() { 
        Vector2 temp = position;
        temp.x = temp.x * 64;
        temp.y = temp.y * 64;
        return temp; 
    }
    Vector2 GetPosition() {
        return position;
    }
};

class Render {
public:
    Render() {}
    void RenderPlayer(Player& player, Color color) {
        DrawTextureRec(player.characterSprite, player.characterRec, player.GetPositionFixedForSprite(), WHITE);
        DrawText(std::to_string(player.position.x).c_str(), 300, 380, 20, RED);
        DrawText(std::to_string(player.position.y).c_str(), 300, 400, 20, RED);
    }
    void RenderBomb(Bomb& bomb) {
        DrawCircle(bomb.position.x * kTileSize, bomb.position.y * kTileSize, 32, bomb.color);
    }
    void RenderObstacles(Obstacle& obstacle) {
        DrawRectangle(obstacle.position.x * kTileSize, obstacle.position.y * kTileSize, 64, 64, obstacle.color);
    }
};

class Map {
public:
    std::string mapName;
    Obstacle obstacles;
    Map(std::string name) {
        this->mapName = name;
        std::list<Obstacle> obstacles;
        if (GetScreenWidth() % 2 == 0) {
            for(int i = 0; i < GetScreenWidth()/64; i++) {
                obstacles.push_back(Obstacle((float)i,0));
            }
        }
    }
};

class Game {
public:
    Game() {}
    Player player1;
    //Player player2;
    std::list<Obstacle> *obstacles;
    Render render;
    std::list<Bomb> bombList;

    void GameUpdate() {
        //---------DELTA TIME ---------
        float delta = GetFrameTime();

        for (Bomb& bombs : bombList) {
            bombs.timer -= 1;
        }
        std::erase_if(bombList, [](Bomb& bomb) { return bomb.timer <= 0; });
    }

    void InputHandle() {
        //---------PLAYER 1 INPUTS-------------
        if (IsKeyPressed(KEY_UP)) {
            player1.MoveUp();
        }
        if (IsKeyPressed(KEY_DOWN)) {
            player1.MoveDown();
        }
        if (IsKeyPressed(KEY_RIGHT)) {
            player1.MoveRight();
        }
        if (IsKeyPressed(KEY_LEFT)) {
            player1.MoveLeft();
        }
        if (IsKeyPressed(KEY_RIGHT_SHIFT)) {
            bombList.push_back(Bomb(RED, player1.GetPosition()));
        }
        /*
        //---------PLAYER 2 INPUTS-------------
        if (IsKeyPressed(KEY_W)) {
            player2.MoveUp();
        }
        if (IsKeyPressed(KEY_S)) {
            player2.MoveDown();
        }
        if (IsKeyPressed(KEY_D)) {
            player2.MoveRight();
        }
        if (IsKeyPressed(KEY_A)) {
            player2.MoveLeft();
        }
        if (IsKeyPressed(KEY_Q)) {
            bombList.push_back(Bomb(BLUE, player2.GetPosition()));
        }
        if (IsKeyPressed(KEY_E)) {
            bombList.pop_front();
        }
        */
    }
    void DrawGame() {
        render.RenderPlayer(player1, BLACK);
      //  render.RenderPlayer(player2, PURPLE);

        for (Bomb& bombs : bombList) {
            render.RenderBomb(bombs);
            DrawText(TextFormat("Y: %2.0f", bombs.timer), 300, 360, 20, RED);
            DrawText(std::to_string(bombList.size()).c_str(), 300, 380, 20, RED);
        }
    }
};

int main(void) {
    const int screenWidth = 1600;
    const int screenHeight = 960;
    GameScreen current_screen = LOGO;

    InitWindow(screenWidth, screenHeight, "BomberMan");
    SetTargetFPS(60);
    Game game;
    Map map("first");
    game.player1.position.x = 24;
    game.player1.position.y = 7;
    //game.player2.position.x = 0;
   // game.player2.position.y = 7;
    int framesCounter = 0;
    while (!WindowShouldClose()) {
        switch (current_screen) {
        case LOGO:
            ++framesCounter;
            if (framesCounter > 60) {
                framesCounter = 0;
                current_screen = TITLE;
            }
            break;
        case TITLE:
            if (IsKeyPressed(KEY_ENTER)) {
                current_screen = GAMEPLAY;
            }
            break;

        case GAMEPLAY:
            // UPDATE
            //------------------------------------------------------------------------------------------

            game.GameUpdate();

            //---------GAME INPUTS-------------
            game.InputHandle();
            //----GAME KEYS-------------/

            if (IsKeyPressed(KEY_ENTER)) {
                current_screen = ENDING;
            }
            break;

        case ENDING:
            if (IsKeyPressed(KEY_ENTER)) {
                current_screen = LOGO;
                game.player1.position.x = 24;
                game.player1.position.y = 7;
               // game.player2.position.x = 0;
               // game.player2.position.y = 7;
            }
            break;
        default:
            break;
        }

        // DRAWING
        //------------------------------------------------------------------------------------------
        ClearBackground(RAYWHITE);
        switch (current_screen) {
        case LOGO:
            DrawText("LOGO HERE", 50, 50, 20, RED);
            break;
        case TITLE:
            DrawText("TITLE HERE", 50, 50, 20, RED);
            break;
        case GAMEPLAY:
            DrawText("GAME PLAY HERE", 50, 50, 20, RED);
            game.DrawGame();
            break;
        case ENDING:
            DrawText("GAME OVER HERE", 50, 50, 20, RED);
            break;
        default:
            break;
        }

        //------------------------------------------------------------------------------------------
        EndDrawing();
        //------------------------------------------------------------------------------------------
    }
    CloseWindow();

    return 0;
}