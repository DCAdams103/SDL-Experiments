#ifndef _PLAYER_H
#define _PLAYER_H

#include <string>

using namespace std;

class Player
{

public:
	static void setTexture(const Uint8* keystates);

	static SDL_Texture* getTexture();
	static SDL_Texture* playerTexture;
	static SDL_Texture* playerRight;
	static SDL_Texture* playerLeft;

	const std::string playerRightPath = "Assets/Player/PlayerRight.png";
	const std::string playerLeftPath = "Assets/Player/PlayerLeft.png";
	Player();

private:
	


};

#endif
