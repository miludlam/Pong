#include "Game.h"

const int thickness = 15;
const float paddleH = 100.0f;

Game::Game() : 
	mWindow(nullptr), 
	mRenderer(nullptr), 
	mTicksCount(0),
	mIsRunning(true),
	mPaddle1Dir(0),
	mPaddle2Dir(0)
{}

bool Game::Initialize() {
	// Initialize SDL
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);

	if (sdlResult != 0) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	// Create SDL window
	mWindow = SDL_CreateWindow(
		"Game Programming in C++ (Chapter 1)", // Window title
		100,  // Top left x-coordinate of window
		100,  // Top left y-coordinate of window
		1024, // Width of window
		768,  // Height of window
		0	  // Flags (0 for no flags set)
	);

	if (!mWindow) {
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	// Create SDL renderer
	mRenderer = SDL_CreateRenderer(
		mWindow, // Window to create renderer for
		-1,		 // Usually -1
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);

	if (!mRenderer) {
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
	}

	// Initialize paddle/ball positions (center points)
	mPaddle1Pos.x = 10.0f;
	mPaddle1Pos.y = 768.0f / 2.0f;
	mPaddle2Pos.x = 1024.0 - 10.0f;
	mPaddle2Pos.y = 768.0f / 2.0f;
	mBallPos.x = 1024.0f / 2.0f;
	mBallPos.y = 768.0f / 2.0f;
	// Initialize ball velocity
	mBallVel.x = -200.0f;
	mBallVel.y = 235.0f;

	// All done!
	return true;
}

void Game::RunLoop() {
	while (mIsRunning) {
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::ProcessInput() {
	SDL_Event event;
	
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:	// If quit event is queued, end game
			mIsRunning = false;
			break;
		}
	}

	// Get state of keyboard
	const Uint8* state = SDL_GetKeyboardState(NULL);
	// If escape is pressed, also end game
	if (state[SDL_SCANCODE_ESCAPE]) {
		mIsRunning = false;
	}

	// Paddle one inputs
	mPaddle1Dir = 0;
	if (state[SDL_SCANCODE_W]) {
		mPaddle1Dir -= 1;
	}
	if (state[SDL_SCANCODE_S]) {
		mPaddle1Dir += 1;
	}
	// Paddle two inputs
	mPaddle2Dir = 0;
	if (state[SDL_SCANCODE_I]) {
		mPaddle2Dir -= 1;
	}
	if (state[SDL_SCANCODE_K]) {
		mPaddle2Dir += 1;
	}


}

void Game::UpdateGame() {
	// Wait until 16ms has elapsed since last frame (60fps)
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
		;

	// Delta time is the difference in ticks from last frame
	// (converted to seconds)
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
	
	// Clamp maximum delta time value
	if (deltaTime > 0.05f) {
		deltaTime = 0.05f;
	}

	// Update tick counts (for next frame)
	mTicksCount = SDL_GetTicks();

	// Update first paddle position
	if (mPaddle1Dir != 0) {
		mPaddle1Pos.y += mPaddle1Dir * 300.0f * deltaTime;
		// Make sure paddle doesn't move off screen!
		if (mPaddle1Pos.y < (paddleH / 2.0f + thickness)) {
			mPaddle1Pos.y = paddleH / 2.0f + thickness;
		}
		else if (mPaddle1Pos.y > (768.0f - paddleH / 2.0f - thickness)) {
			mPaddle1Pos.y = 768.0f - paddleH / 2.0f - thickness;
		}
	}
	// Update second paddle position
	if (mPaddle2Dir != 0) {
		mPaddle2Pos.y += mPaddle2Dir * 300.0f * deltaTime;
		// Make sure paddle doesn't move off screen!
		if (mPaddle2Pos.y < (paddleH / 2.0f + thickness)) {
			mPaddle2Pos.y = paddleH / 2.0f + thickness;
		}
		else if (mPaddle2Pos.y > (768.0f - paddleH / 2.0f - thickness)) {
			mPaddle2Pos.y = 768.0f - paddleH / 2.0f - thickness;
		}
	}

	// Update ball position
	mBallPos.x += mBallVel.x * deltaTime;
	mBallPos.y += mBallVel.y * deltaTime;

	// Check for collision with paddles
	float diff1 = mBallPos.y - mPaddle1Pos.y;
	float diff2 = mBallPos.y - mPaddle2Pos.y;
	// Take absolute value of differences
	diff1 = (diff1 > 0.0f) ? diff1 : -diff1;
	diff2 = (diff2 > 0.0f) ? diff2 : -diff2;
	if (
		// Our y-difference is small enough
		diff1 <= paddleH / 2.0f &&
		// Ball is at the correct x-position
		mBallPos.x <= 25.0f && mBallPos.x >= 20.0f &&
		// The ball is moving to the left
		mBallVel.x < 0.0f)
	{
		mBallVel.x *= -1.0f;
	}
	else if (
		// Our y-difference is small enough
		diff2 <= paddleH / 2.0f &&
		// Ball is at the correct x-position
		mBallPos.x <= 1024.0f - 20.0f && mBallPos.x >= 1024.0f - 25.0f &&
		// The ball is moving to the right
		mBallVel.x > 0.0f)
	{
		mBallVel.x *= -1.0f;
	}
	// Check ball has gone off screen
	else if (mBallPos.x <= 0.0f || mBallPos.x >= 1024.0f) {
		mIsRunning = false;
	}
	// Check for collision with top wall
	else if (mBallPos.y <= thickness && mBallVel.y < 0.0f) {
		mBallVel.y *= -1.0f;
	}
	// Check for collision with bottom wall
	else if (mBallPos.y >= 768.0f - thickness && mBallVel.y > 0.0f) {
		mBallVel.y *= -1.0f;
	}
}

void Game::GenerateOutput() {
	// Specify backdrop color (blue)
	SDL_SetRenderDrawColor(
		mRenderer,
		0,	 // R
		0,	 // G
		255, // B
		255	 // A
	);
	
	// Clear back buffer to current draw color
	SDL_RenderClear(mRenderer);
	
	// Set wall color
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
	
	// Create wall
	SDL_Rect wall{
		0,		  // X
		0,		  // Y
		1024,	  // Width
		thickness // Height
	};
	// Draw top wall
	SDL_RenderFillRect(mRenderer, &wall);
	
	// Draw bottom wall
	wall.y = 768 - thickness;
	SDL_RenderFillRect(mRenderer, &wall);

	// Draw paddle 1
	SDL_Rect paddle{
		static_cast<int>(mPaddle1Pos.x - thickness / 2),
		static_cast<int>(mPaddle1Pos.y - paddleH / 2),
		thickness,
		static_cast<int>(paddleH)
	};
	SDL_RenderFillRect(mRenderer, &paddle);

	// Draw paddle 2
	paddle.x = static_cast<int>(mPaddle2Pos.x - thickness / 2);
	paddle.y = static_cast<int>(mPaddle2Pos.y - paddleH / 2);
	SDL_RenderFillRect(mRenderer, &paddle);

	// Draw ball
	SDL_Rect ball{
		static_cast<int>(mBallPos.x - thickness / 2),
		static_cast<int>(mBallPos.y - thickness / 2),
		thickness,
		thickness
	};
	SDL_RenderFillRect(mRenderer, &ball);

	// Swap front and back buffers
	SDL_RenderPresent(mRenderer);
}

void Game::Shutdown() {
	SDL_DestroyWindow(mWindow);
	SDL_DestroyRenderer(mRenderer);
	SDL_Quit();
}