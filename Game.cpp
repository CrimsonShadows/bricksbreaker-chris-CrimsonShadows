#include "stdafx.h"
#include "Game.h"
#include <thread>
#include <chrono>


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

	bricks.clear(); //clear prev brick 

	for (int i = 0; i < 5; i++) {
		Box brick;
		brick.width = 10;
		brick.height = 2;
		brick.x_position = i * 15; //was 0
		brick.y_position = 5;
		brick.doubleThick = true;
		brick.color = ConsoleColor::DarkGreen;
		bricks.push_back(brick);
		
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

//  All rendering should be done between the locks in this function
void Game::Render() const
{
	Console::Lock(true);
	Console::Clear();
	
	paddle.Draw();
	ball.Draw();

	// TODO #3 - Update render to render all bricks
	for (const Box& brick : bricks) {
		brick.Draw();
	}

	Console::Lock(false);
}

void Game::CheckCollision()
{
	// TODO #4 - Update collision to check all bricks

	for (Box& brick : bricks) {
		
		if (brick.color == ConsoleColor::DarkMagenta) {
			brick.color = ConsoleColor::Gray;

		}

		if (brick.color == ConsoleColor::Yellow) {
			brick.color = ConsoleColor::DarkMagenta;
			
		}


		if (brick.Contains(ball.x_position + ball.x_velocity, ball.y_position + ball.y_velocity))
		{
			brick.color = ConsoleColor(brick.color - 1);
			ball.y_velocity *= -1;

		

			if (brick.color == ConsoleColor::DarkYellow) {
				brick.color = Black;
				brick.width = 0;
				
			}
			// TODO #5 - If the ball hits the same brick 3 times (color == black), remove it from the vector
			else if (brick.color == ConsoleColor::Black) {

				brick.color = ConsoleColor::DarkBlue;

				if (brick.color == ConsoleColor::DarkBlue) {

					brick.color = ConsoleColor::DarkRed;

					if (brick.color == ConsoleColor::DarkRed) {

						brick.color = ConsoleColor::Yellow;
				    }
			    }
		    }
		}
	}

	// TODO #6 - If no bricks remain, pause ball and display victory text with R to reset
	
	bool WinCon = false;
	for (const Box& brick : bricks) {
		if (brick.width > 0) {
			WinCon = true;
			break;
		}
	}

	if (!WinCon) {

		ball.moving = false; //stops ball
		Console::SetCursorPosition(WINDOW_WIDTH / 2 - 10, WINDOW_HEIGHT / 2);
		Console::ForegroundColor(ConsoleColor::Green);
		std::cout << "You Won!";

		std::this_thread::sleep_for(std::chrono::seconds(1));

		Console::ForegroundColor(ConsoleColor::Green);
		std::cout << " Press 'R' to reset game";
		std::this_thread::sleep_for(std::chrono::seconds(1)); 

	}

	if (paddle.Contains(ball.x_position + ball.x_velocity, ball.y_velocity + ball.y_position))
	{
		ball.y_velocity *= -1;
	}

	// TODO #7 - If ball touches bottom of window, pause ball and display defeat text with R to reset
	if (ball.y_position + ball.y_velocity >= WINDOW_HEIGHT) {
		ball.moving = false; //stops ball
		Console::SetCursorPosition(WINDOW_WIDTH / 2 - 10, WINDOW_HEIGHT / 2);
		Console::ForegroundColor(ConsoleColor::Red);
		std::cout << "You Lost!";
		std::this_thread::sleep_for(std::chrono::seconds(1));

		
		Console::SetCursorPosition(WINDOW_WIDTH / 2 - 15, WINDOW_HEIGHT / 2 + 1);
		Console::ForegroundColor(ConsoleColor::Red);
		std::cout << "Press 'R' to reset game";
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}
