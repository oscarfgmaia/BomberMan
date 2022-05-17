#include <raylib.h>
#include <list>
#include <string>
#include <vector>
	//------------------------------------------------------------------------------------------
	typedef enum GameScreen { LOGO = 0, TITLE, GAMEPLAY, ENDING } GameScreen;
	const int kTileSize = 32;
	
	class Bomb {
	public:
		Bomb() {

		}
		Bomb(Color color, Vector2 position) {
			this->color = color;
			this->bomb_position = position;
		}
		float timer = 60;
		Color color;
		Vector2 bomb_position;

		bool shouldExplode() {
			if (timer <= 0) {
				return true;
			}
			else {
				return false;
			}
		}
	};

	class Player {
	public:
		Vector2 position;
		Player() {

		}
		Player(Vector2 position) {
			this->position = position;
		};

		void MoveUp() {
			position.y -= 1;
		}
		void MoveDown() {
			position.y += 1;
		}
		void MoveRight() {
			position.x += 1;
		}
		void MoveLeft() {
			position.x -= 1;
		}
		Vector2 GetPosition() {
			return position;
		}
};

	class Render {
	public:
		Render() {

		}
		void RenderPlayer(Player& player,Color color) {
			auto sprite = Rectangle{ player.position.x * kTileSize, player.position.y * kTileSize, kTileSize,kTileSize };
			DrawRectangleRec(sprite, color);
		}
		void RenderBomb(Bomb& bomb) {
			DrawCircle(bomb.bomb_position.x * kTileSize, bomb.bomb_position.y * kTileSize, 16, bomb.color);
		}
		
	};
	class Game {
	public:
		Game() {

		}
		Player player1;
		Player player2;
		Render render;
		std::list<Bomb> bombList;
		std::string bombSize;
		void GameUpdate() {
			//---------DELTA TIME ---------
			float delta = GetFrameTime();
			
				for (Bomb& bombs : bombList) {
					bombs.timer -= 1;
				}
				auto erased = std::erase_if(bombList, [](Bomb& bomb) {bomb.timer <= 0; });
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
				bombList.push_back(Bomb(RED,player1.GetPosition()));
			}

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
		}
		void DrawGame() {
			render.RenderPlayer(player1, BLACK);
			render.RenderPlayer(player2, PURPLE);

			for (Bomb& bombs : bombList)
			{
				render.RenderBomb(bombs);
				DrawText(TextFormat("Y: %2.0f", bombs.timer), 300, 360, 20, RED);
				bombSize = std::to_string(bombList.size());
				DrawText(bombSize.c_str(), 300, 380, 20, RED);
			}
		}
	};

int main(void) {
	const int screenWidth = 800;
	const int screenHeight = 450;
	GameScreen current_screen = LOGO;
	
	InitWindow(screenWidth, screenHeight, "BomberMan");
	SetTargetFPS(60);
	Game game;
	int framesCounter = 0;
	while (!WindowShouldClose()) {

		switch (current_screen)
		{
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
		//UPDATE
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
			}
			break;
		default:
			break;
		}
		
		//DRAWING
		//------------------------------------------------------------------------------------------
		ClearBackground(RAYWHITE);
		switch (current_screen)
		{
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