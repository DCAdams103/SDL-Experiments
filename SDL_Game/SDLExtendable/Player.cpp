#include <SDL.h>
#include <stdio.h>
#include <string>
#include "Source.h"
#include "Player.h"

using namespace std;

SDL_Texture* Player::playerTexture = NULL;
SDL_Texture* Player::playerRight = NULL;
SDL_Texture* Player::playerLeft = NULL;

Player::Player()
{
	// Load textures
	Player::playerLeft = Source::loadTexture(playerLeftPath);
	Player::playerRight = Source::loadTexture(playerRightPath);
	playerTexture = playerRight;

}

// Sets player texture based on direction moving
void Player::setTexture(const Uint8* keystates)
{
	if (keystates[SDL_SCANCODE_D])
		Player::playerTexture = Player::playerRight;
	else if (keystates[SDL_SCANCODE_A])
		Player::playerTexture = Player::playerLeft;

}

// Tells Main which texture to show based off player direction
SDL_Texture* Player::getTexture()
{
	return playerTexture;
}
