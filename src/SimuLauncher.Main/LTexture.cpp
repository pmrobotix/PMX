#include "LTexture.hpp"

#include <SDL2/SDL_error.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_surface.h>
#include <cstdio>
#include <cstdlib>
#include <iostream>

#include "../Common/Utils/File.hpp"
#include "../Log/Logger.hpp"
#include "SDLTool.hpp"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "Nanosvg/stb_image_write.h"
#define NANOSVG_IMPLEMENTATION
#include "Nanosvg/nanosvg.h"
#define NANOSVGRAST_IMPLEMENTATION
#include "Nanosvg/nanosvgrast.h"

LTexture::LTexture()
{
	//Initialize
	mTexture = NULL;
	mImage = NULL;
	mWidth_ = 0;
	mHeight_ = 0;
}

LTexture::~LTexture()
{
	//Deallocate
	free();
}

bool LTexture::load(SDL_Renderer* renderer, std::string path)
{
	if (getExt(path) == "svg")
	{
		return this->loadFromSVG(renderer, path);
	}
	else
	{
		return this->loadFromFile(renderer, path);
	}
}

bool LTexture::loadFromSVG(SDL_Renderer* renderer, std::string path)
{
	SDLTool::checkThread(__PRETTY_FUNCTION__);

	//Get rid of preexisting texture
	free();

	//using nanosvg
	NSVGimage *image = NULL;
	NSVGrasterizer *rast = NULL;
	unsigned char* img = NULL;

	const char* filename = path.c_str();

	//printf("parsing %s\n", filename);
	image = nsvgParseFromFile(filename, "px", 96.0f);
	if (image == NULL)
	{
		logger().error() << "Could not open SVG image." << logs::end;
		goto error;
	}

	//Get image dimensions
	mWidth_ = (int) image->width;
	mHeight_ = (int) image->height;

	rast = nsvgCreateRasterizer();
	if (rast == NULL)
	{
		logger().error() << "Could not init rasterizer." << logs::end;
		goto error;
	}

	img = (unsigned char*) malloc(mWidth_ * mHeight_ * 4);
	if (img == NULL)
	{
		logger().error() << "Could not alloc image buffer." << logs::end;
		goto error;
	}

	//printf("rasterizing image %d x %d\n", w, h);
	nsvgRasterize(rast, image, 0, 0, 1, img, mWidth_, mHeight_, mWidth_ * 4);

	//write PNG image in a file
	//printf("writing svg.png\n");
	//stbi_write_png("svg.png", mWidth, mHeight, 4, img, mWidthw * 4);

	mImage = SDL_CreateRGBSurfaceFrom(img, //pointer to the pixels
			mWidth_, //Width
			mHeight_, //Height
			32, //Depth (bits per pixel)
			mWidth_ * 4, //Pitch (width*depth_in_bytes, in this case)
			0x000000FF, //Red mask
			0x0000FF00, //Green mask
			0x00FF0000, //Blue mask
			0xFF000000); //Alpha mask (there is an alpha in this format)

	mTexture = SDL_CreateTextureFromSurface(renderer, mImage);

	//Return success
	return mTexture != NULL;

	error: nsvgDeleteRasterizer(rast);
	nsvgDelete(image);
	return 0;
}

bool LTexture::loadFromFile(SDL_Renderer* renderer, std::string path)
{
	SDLTool::checkThread(__PRETTY_FUNCTION__);

	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(),
		IMG_GetError());
		exit(-1);
	}
	else
	{
		//Color key image
		SDL_SetColorKey(loadedSurface, true, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
		if (newTexture == NULL)
		{
			logger().error() << "Unable to create texture from " << path.c_str() << "! SDL Error: "
					<< SDL_GetError() << logs::end;
		}
		else
		{
			//Get image dimensions
			mWidth_ = loadedSurface->w;
			mHeight_ = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}

bool LTexture::loadFromRenderedText(SDL_Renderer* renderer, TTF_Font *gFont,
		std::string textureText, SDL_Color textColor)
{
	SDLTool::checkThread(__PRETTY_FUNCTION__);
	//Get rid of preexisting texture
	free();

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, textureText.c_str(), textColor);
	if (textSurface == NULL)
	{
		logger().error() << "Unable to render text surface! SDL_ttf Error: " << TTF_GetError()
				<< logs::end;
	}
	else
	{
		//Create texture from surface pixels
		mTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
		if (mTexture == NULL)
		{
			logger().error() << "Unable to create texture from rendered text! SDL Error: "
					<< SDL_GetError() << logs::end;
		}
		else
		{
			//Get image dimensions
			mWidth_ = textSurface->w;
			mHeight_ = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface(textSurface);
	}

	//Return success
	return mTexture != NULL;
}

void LTexture::free()
{
	SDLTool::checkThread(__PRETTY_FUNCTION__);
	SDL_FreeSurface(mImage);

	//Free texture if it exists
	if (mTexture != NULL)
	{
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
		mWidth_ = 0;
		mHeight_ = 0;
	}
}

void LTexture::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
	SDLTool::checkThread(__PRETTY_FUNCTION__);
	//Modulate texture rgb
	SDL_SetTextureColorMod(mTexture, red, green, blue);
}

void LTexture::setBlendMode(SDL_BlendMode blending)
{
	SDLTool::checkThread(__PRETTY_FUNCTION__);
	//Set blending function
	SDL_SetTextureBlendMode(mTexture, blending);
}

void LTexture::setAlpha(Uint8 alpha)
{
	SDLTool::checkThread(__PRETTY_FUNCTION__);
	//Modulate texture alpha
	SDL_SetTextureAlphaMod(mTexture, alpha);
}

void LTexture::render(SDL_Renderer* gRenderer, int x, int y, SDL_Rect* clip, double angle,
		SDL_Point* center, SDL_RendererFlip flip)
{
	SDLTool::checkThread(__PRETTY_FUNCTION__);
	//Set rendering space and render to screen
	SDL_Rect renderQuad =
	{ x, y, mWidth_, mHeight_ };

	//Set clip rendering dimensions
	if (clip != NULL)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopyEx(gRenderer, mTexture, clip, &renderQuad, angle, center, flip);
}

int LTexture::getWidth()
{
	return mWidth_;
}

int LTexture::getHeight()
{
	return mHeight_;
}

