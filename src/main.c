#include <SDL2/SDL.h>
#include <stdlib.h>
#include "hot.h"

#define WIDTH 800
#define HEIGHT 800

void
InitSDL()
{
  if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
    printf("error: %s\n", SDL_GetError());
    exit(1);
  }
}

SDL_Window *
InitWindow(int width, int height, char *name)
{
  SDL_Window *win = SDL_CreateWindow(name,
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    width,
    height,
    SDL_WINDOW_SHOWN);

  if(win == NULL) {
    printf("error: %s\n", SDL_GetError());
    exit(1);
  }

  return win;
}

SDL_Renderer *
InitRenderer(SDL_Window *win, bool vsync)
{
  uint32_t flags = SDL_RENDERER_ACCELERATED;
  if(vsync) {
    flags |= SDL_RENDERER_PRESENTVSYNC;
  }
  SDL_Renderer *renderer = SDL_CreateRenderer(win, -1, flags);
  if(renderer == NULL) {
    printf("error: %s\n", SDL_GetError());
    exit(1);
  }

  return renderer;
}

int
main()
{
  InitSDL();

  SDL_Window *win = InitWindow(WIDTH, HEIGHT, "Hot Reload C");

  SDL_Renderer *renderer = InitRenderer(win, false);

  SDL_Texture *texture = SDL_CreateTexture(renderer,
    SDL_PIXELFORMAT_RGBA8888,
    SDL_TEXTUREACCESS_STREAMING,
    WIDTH,
    HEIGHT);
  if(texture == NULL) {
    return 1;
  }

  void *rawPixels;
  int pitch;

  bool quit = false;

  SDL_Event e;

  hot_video video;
  video.width = WIDTH;
  video.height = HEIGHT;

  while(!quit) {
    while(SDL_PollEvent(&e)) {
      if(e.type == SDL_KEYDOWN) {
        switch(e.key.keysym.sym) {
          case SDLK_ESCAPE: {
            quit = true;
          } break;
        }
      }

      if(e.type == SDL_QUIT) {
        quit = true;
      }
    }

    SDL_LockTexture(texture, NULL, &video.memory, &video.pitch);

    Render(&video);

    SDL_UnlockTexture(texture);

    SDL_RenderCopy(renderer, texture, NULL, NULL);

    SDL_RenderPresent(renderer);

    SDL_Delay(16);
  }

  return 0;
}
