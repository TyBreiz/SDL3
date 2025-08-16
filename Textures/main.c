#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static Uint64 last_time = 0;

#define PI 3.1415926535
#define WINDOW_WIDTH 100
#define WINDOW_HEIGHT 100

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]){

  SDL_Init(SDL_INIT_VIDEO);
  
  window = SDL_CreateWindow("Boids", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_BORDERLESS);

  renderer = SDL_CreateRenderer(window, NULL);
  SDL_RenderClear(renderer);
  SDL_RenderPresent(renderer);
  
  if (window == NULL) {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create window: %s\n", SDL_GetError());
    return SDL_APP_FAILURE;
  }
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
