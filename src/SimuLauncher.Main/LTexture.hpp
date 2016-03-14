#ifndef SIMULAUNCHER_MAIN_LTEXTURE_HPP_
#define SIMULAUNCHER_MAIN_LTEXTURE_HPP_

#include <SDL2/SDL_blendmode.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_ttf.h>
#include <string>

#include "../Log/LoggerFactory.hpp"

//Texture wrapper class
class LTexture
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref LTexture.
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("LTexture");
		return instance;
	}

	//The actual hardware texture
	SDL_Texture* mTexture;

	SDL_Surface* mImage;

	//Image dimensions
	int mWidth_;
	int mHeight_;

public:
	//Initializes variables
	LTexture();

	//Deallocates memory
	~LTexture();

	bool load(SDL_Renderer* renderer, std::string path);

	//Loads svg image at specified path
	bool loadFromSVG(SDL_Renderer* renderer, std::string path);

	//Loads image at specified path
	bool loadFromFile(SDL_Renderer* renderer, std::string path);

	//Creates image from font string
	bool loadFromRenderedText(SDL_Renderer* renderer, TTF_Font *gFont, std::string textureText,
			SDL_Color textColor);

	//Deallocates texture
	void free();

	//Set color modulation
	void setColor(Uint8 red, Uint8 green, Uint8 blue);

	//Set blending
	void setBlendMode(SDL_BlendMode blending);

	//Set alpha modulation
	void setAlpha(Uint8 alpha);

	//Renders texture at given point
	void render(SDL_Renderer* gRenderer, int x, int y, SDL_Rect* clip = NULL, double angle = 0.0,
			SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

	//Gets image dimensions
	int getWidth();
	int getHeight();

};

#endif /* SIMULAUNCHER_MAIN_LTEXTURE_HPP_ */
