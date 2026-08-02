#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

using namespace std;

typedef struct {
    int x;
    int y;
} SDL_Pos;

struct Node {
    int id;
    SDL_Pos pos;
};

class SDLHelper {
    static SDL_Texture* getClockTextTexture(SDL_Renderer* renderer, const std::string &text, SDL_Color fgColor);
public:
    static void SDL_ErrorHandler(const string &message, const string &sdlError);
    static TTF_Font* Font(int size);
    static SDL_Texture* getTextTexture(SDL_Renderer* renderer, const string &text, int fontSize, SDL_Color fgColor);
    static void SDL_RenderText(SDL_Renderer* renderer, const string &text, int fontSize, SDL_Color fgColor, SDL_Rect textRect);
    static void SDL_RenderTextRotated(SDL_Renderer* renderer, const string &text, int fontSize, SDL_Color fgColor, SDL_Rect textRect, double angle);

    static TTF_Font* clockFont;
    static void InitClockFont(const std::string &fontPath, int fontSize);
    static void CloseClockFont();
    static void RenderClockAndTotalDistance(SDL_Renderer* renderer, Uint32 elapsedMs, float totalDistance, SDL_Color fgColor, SDL_Rect &rect);
    static void RenderDriverDistance(SDL_Renderer* renderer, float driverDistance, int x, int y, SDL_Color fgColor);

    static int SDL_RenderDrawCircle(SDL_Renderer *renderer, int x, int y, int radius);
    static int SDL_RenderFillCircle(SDL_Renderer *renderer, int x, int y, int radius);

    static bool SDLNode_isContain(SDL_Pos _p, Node _n);
    static void SDL_DrawNode(SDL_Renderer *renderer, SDL_Pos _p, int _id, SDL_Color color, bool flag);
};
