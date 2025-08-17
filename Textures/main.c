#define SDL_MAIN_USE_CALLBACKS 1

#include <stdlib.h>
#include <time.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_render.h>

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static Uint64 last_time = 0;

#define PI 3.1415926535
#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 1000

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]){

  bool quit = false;
  SDL_Window *window = SDL_CreateWindow("Triangle Example", WINDOW_WIDTH, WINDOW_HEIGHT, 0);
  
  SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);

  SDL_Texture *texture = NULL;

  texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, WINDOW_WIDTH, WINDOW_HEIGHT);
  
  SDL_SetRenderTarget(renderer, texture);

  srand(time(NULL));
  
  for(int x = 0; x < WINDOW_WIDTH; x++){
    for(int y = 0; y < WINDOW_WIDTH; y++){
      int c = rand() % 255;
      SDL_SetRenderDrawColor(renderer, c, c, c, 255);
      SDL_RenderPoint(renderer, x, y);
      }
  }
  
  SDL_SetRenderTarget(renderer, NULL);
  
  while (!quit) {
   SDL_Event ev;
   while (SDL_PollEvent(&ev) != 0) {
      switch(ev.type) {
        case SDL_EVENT_QUIT:
        quit = true;
        break;
      }
    }
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_RenderTextureAffine(renderer, texture, NULL, NULL, NULL, NULL);

    SDL_RenderPresent(renderer);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  
  /*SDL_Init(SDL_INIT_VIDEO);
  
  window = SDL_CreateWindow("Boids", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_BORDERLESS);

  renderer = SDL_CreateRenderer(window, NULL);
  SDL_RenderClear(renderer);
  SDL_RenderPresent(renderer);
  
  if (window == NULL) {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create window: %s\n", SDL_GetError());
    return SDL_APP_FAILURE;
    }*/
  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event){
  switch(event -> type) {
  case SDL_EVENT_QUIT : {
    return SDL_APP_SUCCESS;
    break;
  }
  case SDL_EVENT_KEY_DOWN : {
    SDL_Keycode k = event -> key.key;
    if(k == SDLK_ESCAPE) return SDL_APP_SUCCESS;
  }
  }
  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate){
  return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
  SDL_DestroyWindow(window);
  SDL_free(appstate);
}
