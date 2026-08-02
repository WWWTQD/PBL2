#include "SDLInterface.h"
#define WINDOW_WIDTH 1400
#define WINDOW_HEIGHT 700

int main(int argc, char* argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
       cerr << "SDL_Init loi: " << SDL_GetError() << endl;
       return 0;
   }

   if (TTF_Init() != 0) {
       cerr << "TTF_Init loi: " << TTF_GetError() << endl;
       SDL_Quit();
       return 0;
   }

    SDL_Window* window = SDL_CreateWindow(
       "PBL2 Project",
       SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
       WINDOW_WIDTH, WINDOW_HEIGHT,
       SDL_WINDOW_SHOWN
   );

   if (!window) {
       cerr << "SDL_CreateWindow loi: " << SDL_GetError() << endl;
       TTF_Quit();
       SDL_Quit();
       return 0;
   }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
   if (!renderer) {
       cerr << "SDL_CreateRenderer loi: " << SDL_GetError() << endl;
       SDL_DestroyWindow(window);
       TTF_Quit();
       SDL_Quit();
       return 0;
   }

    TTF_Font* font = TTF_OpenFont("Roboto.ttf", 24);
   if (!font) {
       cerr << "Loi load font: " << TTF_GetError() << endl;
       SDL_DestroyRenderer(renderer);
       SDL_DestroyWindow(window);
       TTF_Quit();
       SDL_Quit();
       return 0;
   }

   bool cityMapGenerated = false;
   bool isRunning = true;
   SDL_Event event;
   AddMode addMode = ADD_NONE;
   EdgeMode edgeMode = EDGE_NONE;
   CityMap cityMap;
   SDLInterface sdlInterface;

    while (isRunning) {
        sdlInterface.HandleEvents(event, isRunning, cityMapGenerated,
                          addMode, edgeMode,
                          renderer, font, cityMap);

       SDL_Delay(16);
   }

    TTF_CloseFont(font);
   SDL_DestroyRenderer(renderer);
   SDL_DestroyWindow(window);
   TTF_Quit();
   SDL_Quit();
   return 0;
}
