#define SDL_MAIN_USE_CALLBACKS 1
#include <stdio.h>
#include <stdlib.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <math.h>
#include <time.h>

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static Uint64 last_time = 0;

#define PI 3.1415926535
#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 1000

typedef struct {
  float x;
  float y;
  float z;
} vect;

typedef struct {
  int x;
  int y;
  int z;
} side;

typedef struct {
  int n_v;
  int n_f;
  vect *v;
  side *s;
} obj;

obj *read_obj(char *file_name, float zoom){
  FILE *f = fopen(file_name, "r");
  if(f == NULL){printf("Eurreur lors de fopen\n");}
  int n_v = 0;
  int n_f = 0;
  char c;
  float x, y, z;
  while(fscanf(f, "%c %f %f %f\n", &c, &x, &y, &z) != EOF){
    c == 'v' ? n_v++ : n_f++;
  }
  vect *v = malloc(sizeof(vect)*n_v);
  side *s = malloc(sizeof(side)*n_f);
  n_v = 0;
  n_f = 0;
  fclose(f);
  f = fopen(file_name, "r");  
  while(fscanf(f, "%c %f %f %f\n", &c, &x, &y, &z) != EOF){
    if(c == 'v'){
      v[n_v].x = zoom*x;
      v[n_v].y = zoom*y;
      v[n_v].z = zoom*z;
      n_v++;
    }
    else{
      s[n_f].x = (int) (x-1);
      s[n_f].y = (int) (y-1);
      s[n_f].z = (int) (z-1);
      n_f++;
    }
  }
  obj *res = malloc(sizeof(obj));
  res -> n_v = n_v;
  res -> n_f = n_f;
  res -> v = v;
  res -> s = s;
  fclose(f);
  return res;
}

void draw_side(SDL_Renderer *renderer, vect i, vect j, vect k){
  SDL_RenderLine(renderer, i.x + WINDOW_WIDTH/2, i.z + WINDOW_HEIGHT/2, j.x + WINDOW_WIDTH/2, j.z + WINDOW_HEIGHT/2);
  SDL_RenderLine(renderer, j.x + WINDOW_WIDTH/2, j.z + WINDOW_HEIGHT/2, k.x + WINDOW_WIDTH/2, k.z + WINDOW_HEIGHT/2);
  SDL_RenderLine(renderer, k.x + WINDOW_WIDTH/2, k.z + WINDOW_HEIGHT/2, i.x + WINDOW_WIDTH/2, i.z + WINDOW_HEIGHT/2);
  //SDL_RenderPoint(renderer, i.x + WINDOW_WIDTH/2, i.z + WINDOW_HEIGHT/2);
  //SDL_RenderPoint(renderer, j.x + WINDOW_WIDTH/2, j.z + WINDOW_HEIGHT/2);
  //SDL_RenderPoint(renderer, k.x + WINDOW_WIDTH/2, k.z + WINDOW_HEIGHT/2);
}

void draw_obj(SDL_Renderer *renderer, obj *o){
  for(int i=0; i < o -> n_f; i++){
    draw_side(renderer, o -> v[o->s[i].x], o -> v[o->s[i].y], o -> v[o->s[i].z]);
  }
}


vect rot_x(vect v, float teta){
  float c = cosf(teta);
  float s = sinf(teta);
  vect res = {v.x, c*v.y-s*v.z, s*v.y+c*v.z};
  return res;
}

vect rot_y(vect v, float teta){
  float c = cosf(teta);
  float s = sinf(teta);
  vect res = {c*v.x+s*v.z, v.y, -s*v.x+c*v.z};
  return res;
}

vect rot_z(vect v, float teta){
  float c = cosf(teta);
  float s = sinf(teta);
  vect res = {c*v.x-s*v.y, s*v.x+c*v.y, v.z};
  return res;
}

void cube_rot(obj *o, float r_x, float r_y, float r_z){
  for(int i=0; i<o->n_v; i++){
    o->v[i] = rot_x(o->v[i], r_x);
    o->v[i] = rot_y(o->v[i], r_y);
    o->v[i] = rot_z(o->v[i], r_z);
  }
}


/*int main(int argc, char *argv[]){
  obj *tmp = read_obj("teddy.obj");
  printf("v = %d\nf = %d\n",tmp -> n_v, tmp -> n_f);
  for(int i = 0; i < (tmp -> n_v); i++){
    printf("x = %f, y = %f, z = %f\n", tmp -> v[i].x, tmp -> v[i].y, tmp -> v[i].z);
  }
  return 0;
}*/


SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]){

  srand(time(NULL));
  SDL_Init(SDL_INIT_VIDEO);
  
  window = SDL_CreateWindow("Obj wiever", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_BORDERLESS);
  
  renderer = SDL_CreateRenderer(window, NULL);
  SDL_RenderClear(renderer);
  SDL_RenderPresent(renderer);

  obj *res = read_obj(argv[1], atof(argv[2]));
  *appstate = res;
  
  if (window == NULL) {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create window: %s\n", SDL_GetError());
    return SDL_APP_FAILURE;
  }
  SDL_SetWindowRelativeMouseMode(window, true);
  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event){
  obj *as = appstate;
  switch(event -> type) {
  case SDL_EVENT_QUIT : {
    return SDL_APP_SUCCESS;
    break;
  }
  case SDL_EVENT_KEY_DOWN : {
    SDL_Keycode k = event -> key.key;
    if(k == SDLK_ESCAPE) return SDL_APP_SUCCESS;
  }
  case SDL_EVENT_MOUSE_MOTION : {
    float x_rel = event -> motion.xrel;
    float y_rel = event -> motion.yrel;
    printf("x_rel = %f\ny_rel = %f\n", x_rel, y_rel);
    cube_rot(as, y_rel/500, 0, x_rel/500); 
  }
  }
  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate){
  obj *o = appstate;
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);  /* black, full alpha */                                                         
  SDL_RenderClear(renderer);
  
  SDL_SetRenderDrawColor(renderer, 255, 255, 0, SDL_ALPHA_OPAQUE);
  draw_obj(renderer, o);
  SDL_RenderPresent(renderer);
  return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
  SDL_DestroyWindow(window);
  SDL_free(appstate);
}

