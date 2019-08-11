#include <iostream>
#include <SDL.h>

const int ANIMATION_FRAMES = 15;
SDL_Rect gSpriteClips[ANIMATION_FRAMES];
LTexture gSpriteSheetTexture;

bool loadMedia();