#include "SDLHelper.h"
#include <iomanip>

// *********************************************** SDL_TTF *********************************************** //

void SDLHelper::SDL_ErrorHandler(const string &message, const string &sdlError)
{
    SDL_Log("%s failed: %s\n", message.c_str(), sdlError.c_str());
    TTF_Quit();
    SDL_Quit();
    exit(1);
}

TTF_Font* SDLHelper::Font(int size)
{
    TTF_Font* font = TTF_OpenFont("Roboto.ttf", size);
    if (!font) {
        SDL_ErrorHandler("TTF_OpenFont", TTF_GetError());
    }
    return font;
}

SDL_Texture* SDLHelper::getTextTexture(SDL_Renderer* renderer, const string &text, int fontSize, SDL_Color fgColor)
{
    TTF_Font* font = Font(fontSize);
    SDL_Surface* textSurface = TTF_RenderText_Blended(font, text.c_str(), fgColor);
    if (!textSurface) {
        TTF_CloseFont(font);
        SDL_ErrorHandler("TTF_RenderText", TTF_GetError());
    }

    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);
    TTF_CloseFont(font);

    if (!textTexture) {
        SDL_ErrorHandler("SDL_CreateTextureFromSurface", SDL_GetError());
    }

    SDL_SetTextureBlendMode(textTexture, SDL_BLENDMODE_BLEND);
    return textTexture;
}

void SDLHelper::SDL_RenderText(SDL_Renderer* renderer, const string &text, int fontSize, SDL_Color fgColor, SDL_Rect textRect)
{
    SDL_Texture* textTexture = getTextTexture(renderer, text, fontSize, fgColor);
    SDL_QueryTexture(textTexture, nullptr, nullptr, &textRect.w, &textRect.h);
    SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);
    SDL_DestroyTexture(textTexture);
}


void SDLHelper::SDL_RenderTextRotated(SDL_Renderer* renderer, const string &text, int fontSize, SDL_Color fgColor, SDL_Rect textRect, double angle)
{
    SDL_Texture* textTexture = getTextTexture(renderer, text, fontSize, fgColor);
    if (!textTexture) return;

    int w, h;
    SDL_QueryTexture(textTexture, nullptr, nullptr, &w, &h);
    textRect.w = w;
    textRect.h = h;

    SDL_RenderCopyEx(renderer, textTexture, nullptr, &textRect, angle, nullptr, SDL_FLIP_NONE);
    SDL_DestroyTexture(textTexture);
}

TTF_Font* SDLHelper::clockFont = nullptr;

void SDLHelper::InitClockFont(const std::string &fontPath, int fontSize) {
    if (clockFont) return;
    clockFont = TTF_OpenFont(fontPath.c_str(), fontSize);
    if (!clockFont) SDL_Log("Failed to load clock font: %s", TTF_GetError());
}

void SDLHelper::CloseClockFont() {
    if (clockFont) {
        TTF_CloseFont(clockFont);
        clockFont = nullptr;
    }
}

SDL_Texture* SDLHelper::getClockTextTexture(SDL_Renderer* renderer, const std::string &text, SDL_Color fgColor) {
    if (!clockFont) return nullptr;
    SDL_Surface* surface = TTF_RenderUTF8_Blended(clockFont, text.c_str(), fgColor);
    if (!surface) return nullptr;
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

void SDLHelper::RenderClockAndTotalDistance(SDL_Renderer* renderer, Uint32 elapsedMs, float totalDistance, SDL_Color fgColor, SDL_Rect &rect) {
    if (!clockFont) return;

    int minutes = elapsedMs / 60000;
    int seconds = (elapsedMs % 60000) / 1000;
    int centis  = (elapsedMs % 1000) / 10;

    std::ostringstream ss;
    ss << std::setfill('0') << std::setw(2) << minutes << ":"
       << std::setfill('0') << std::setw(2) << seconds << ":"
       << std::setfill('0') << std::setw(2) << centis
       << " | Total distance: " << std::fixed << std::setprecision(1) << totalDistance;

    std::string text = ss.str();
    SDL_Texture* texture = getClockTextTexture(renderer, text, fgColor);
    if (!texture) return;

    SDL_QueryTexture(texture, nullptr, nullptr, &rect.w, &rect.h);
    SDL_RenderCopy(renderer, texture, nullptr, &rect);
    SDL_DestroyTexture(texture);
}

void SDLHelper::RenderDriverDistance(SDL_Renderer* renderer, float driverDistance, int x, int y, SDL_Color fgColor) {
    if (!clockFont) return;

    std::ostringstream ss;
    ss << std::fixed << std::setprecision(1) << driverDistance;

    SDL_Texture* texture = getClockTextTexture(renderer, ss.str(), fgColor);
    if (!texture) return;

    SDL_Rect rect = { x, y, 0, 0 };
    SDL_QueryTexture(texture, nullptr, nullptr, &rect.w, &rect.h);
    SDL_RenderCopy(renderer, texture, nullptr, &rect);
    SDL_DestroyTexture(texture);
}
// ************************************************* SDL_Hinhtron ******************************************** //

int SDLHelper::SDL_RenderDrawCircle(SDL_Renderer *renderer,const int x,const int y,const int radius)
{
    int offsetX = 0;
    int offsetY = radius;
    int d = radius - 1;
    int status = 0;

    while (offsetY >= offsetX)
    {
        status += SDL_RenderDrawPoint(renderer, x + offsetX, y + offsetY);
        status += SDL_RenderDrawPoint(renderer, x + offsetY, y + offsetX);
        status += SDL_RenderDrawPoint(renderer, x - offsetX, y + offsetY);
        status += SDL_RenderDrawPoint(renderer, x - offsetY, y + offsetX);
        status += SDL_RenderDrawPoint(renderer, x + offsetX, y - offsetY);
        status += SDL_RenderDrawPoint(renderer, x + offsetY, y - offsetX);
        status += SDL_RenderDrawPoint(renderer, x - offsetX, y - offsetY);
        status += SDL_RenderDrawPoint(renderer, x - offsetY, y - offsetX);

        if (status < 0) {
            status = -1;
            break;
        }

        if (d >= 2*offsetX) {
            d -= 2*offsetX + 1;
            offsetX +=1;
        }
        else if (d < 2 * (radius - offsetY)) {
            d += 2 * offsetY - 1;
            offsetY -= 1;
        }
        else {
            d += 2 * (offsetY - offsetX - 1);
            offsetY -= 1;
            offsetX += 1;
        }
    }
    return status;
}

int SDLHelper::SDL_RenderFillCircle(SDL_Renderer *renderer,const int x,const int y,const int radius)
{
    int offsetX = 0;
    int offsetY = radius;
    int d = radius - 1;
    int status = 0;

    while (offsetY >= offsetX)
    {
        status += SDL_RenderDrawLine(renderer, x - offsetY, y + offsetX, x + offsetY, y + offsetX);
        status += SDL_RenderDrawLine(renderer, x - offsetX, y + offsetY, x + offsetX, y + offsetY);
        status += SDL_RenderDrawLine(renderer, x - offsetX, y - offsetY, x + offsetX, y - offsetY);
        status += SDL_RenderDrawLine(renderer, x - offsetY, y - offsetX, x + offsetY, y - offsetX);

        if (status < 0) {
            status = -1;
            break;
        }

        if (d >= 2*offsetX) {
            d -= 2*offsetX + 1;
            offsetX +=1;
        }
        else if (d < 2 * (radius - offsetY)) {
            d += 2 * offsetY - 1;
            offsetY -= 1;
        }
        else {
            d += 2 * (offsetY - offsetX - 1);
            offsetY -= 1;
            offsetX += 1;
        }
    }

    return status;
}

bool SDLHelper::SDLNode_isContain(const SDL_Pos _p, const Node _n)
{
    int dx = _p.x - _n.pos.x;
    int dy = _p.y - _n.pos.y;
    return (dx * dx + dy * dy <= 20 * 20);
}

void SDLHelper::SDL_DrawNode(SDL_Renderer *renderer, const SDL_Pos _p, int _id, SDL_Color color, bool flag)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillCircle(renderer, _p.x, _p.y, 20);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawCircle(renderer, _p.x, _p.y, 20);

    char buffer[20];
    sprintf(buffer, "%d", _id);
    SDL_Texture *textTexture = getTextTexture(renderer, buffer, 25, {0,0,0,255});

    int w, h;
    SDL_QueryTexture(textTexture, nullptr, nullptr, &w, &h);
    SDL_Rect r = {_p.x - w/2, _p.y - h/2, w, h};

    SDL_RenderCopy(renderer, textTexture, nullptr, &r);
    SDL_DestroyTexture(textTexture);

    if (flag)
        SDL_RenderPresent(renderer);
}

