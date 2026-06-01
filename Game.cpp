#include "stdafx.h"
#include "Game.h"

Game::Game()
{
	Reset();
}

void Game::Reset()
{
	Console::SetWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	Console::CursorVisible(false);
	paddle.width = 12;
	paddle.height = 2;
	paddle.x_position = 32;
	paddle.y_position = 30;

	ball.visage = 'O';
	ball.color = ConsoleColor::Cyan;
	ResetBall();

	// TODO #2 - Add this brick and 4 more bricks to the vector
	bricks.resize(numberOfBricks);
	int brickWidth = 10;
	int brickGap = 2;
	int startPos = 5;
	int nextBrick = brickWidth + brickGap;

	for (int i = 0; i < numberOfBricks; i++) {
		//int brickOffset = 
		bricks[i].width = brickWidth;
		bricks[i].height = 2;
		bricks[i].x_position = startPos + (i * nextBrick);
		bricks[i].y_position = 5;
		bricks[i].doubleThick = true;
		bricks[i].color = ConsoleColor::DarkCyan;
	}
}

void Game::ResetBall()
{
	ball.x_position = paddle.x_position + paddle.width / 2;
	ball.y_position = paddle.y_position - 1;
	ball.x_velocity = rand() % 2 ? 1 : -1;
	ball.y_velocity = -1;
	ball.moving = false;
}

bool Game::Update()
{
	if (GetAsyncKeyState(VK_ESCAPE) & 0x1)
		return false;

	if (GetAsyncKeyState(VK_RIGHT) && paddle.x_position < WINDOW_WIDTH - paddle.width)
		paddle.x_position += 2;

	if (GetAsyncKeyState(VK_LEFT) && paddle.x_position > 0)
		paddle.x_position -= 2;

	if (GetAsyncKeyState(VK_SPACE) & 0x1)
		ball.moving = !ball.moving;

	if (GetAsyncKeyState('R') & 0x1)
		Reset();

	ball.Update();
	CheckCollision();
	return true;
}

//  All rendering, including text, should occur in the Render function
void Game::Render() const
{
	Console::Lock(true);
	Console::Clear();
	
	paddle.Draw();
	ball.Draw();

	// TODO #3 - Update render to render all bricks
	for (int i = 0; i < bricks.size(); i++)
		bricks[i].Draw();

	int centerHeight = Console::WindowHeight() / 2;
	int centerWidth = Console::WindowWidth() / 2;
	int offset = 10;

	if (bricks.empty()) {
		Console::WordWrap(centerWidth - offset, centerHeight, 20, "You win! Press R to play again.");
	} else if (ball.y_position >= Console::WindowHeight()) {
		Console::WordWrap(centerWidth - offset, centerHeight, 20, "You lose! Press R to play again.");
	}

	Console::Lock(false);
}

void Game::CheckCollision()
{
	// TODO #4 - Update collision to check all bricks
	for (std::vector<Box>::iterator iter = bricks.begin(); iter != bricks.end(); ) {
		if (iter->Contains(ball.x_position + ball.x_velocity, ball.y_position + ball.y_velocity)) {
			iter->color = ConsoleColor(iter->color - 1);
			ball.y_velocity *= -1;

			// TODO #5 - If the ball hits the same brick 3 times (color == black), remove it from the vector
			if (iter->color == ConsoleColor::Black)
				iter = bricks.erase(iter);
			else
				iter++;

			break;
		} else
			iter++;
	}
	// TODO #6 - If no bricks remain, pause ball and display (render) victory text with R to reset
	if (bricks.empty()) {
		ball.x_velocity = 0;
		ball.y_velocity = 0;
	}
	if (paddle.Contains(ball.x_position + ball.x_velocity, ball.y_velocity + ball.y_position))
	{
		ball.y_velocity *= -1;
	}

	// TODO #7 - If ball touches bottom of window, pause ball and display (render) defeat text with R to reset
	if (ball.y_position >= Console::WindowHeight()) {
		ball.x_velocity = 0;
		ball.y_velocity = 0;

		ball.y_position = Console::WindowHeight();
	}
}