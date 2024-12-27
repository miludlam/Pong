#pragma once
#include "SDL.h"

// Vector2 struct to store x/y coordinates
struct Vector2 {
	float x;
	float y;
};

class Game {
public:
	Game();
	// Initialize the game
	bool Initialize();
	// Runs the game loop until the game is over
	void RunLoop();
	// Shutdown the game
	void Shutdown();
private:
	// Helper functions for the game loop
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();

	// Window created by SDL
	SDL_Window* mWindow;
	// Renderer member variable
	SDL_Renderer* mRenderer;
	// Tick counter for game loop
	Uint32 mTicksCount;
	// Game should continue to run
	bool mIsRunning;
	
	// First paddle
	// Direction of paddle
	int mPaddle1Dir;
	// Paddle position
	Vector2 mPaddle1Pos;
	// Second paddle
	// Direction of paddle
	int mPaddle2Dir;
	// Paddle position
	Vector2 mPaddle2Pos;

	// Ball position
	Vector2 mBallPos;
	// Ball velocity
	Vector2 mBallVel;
};