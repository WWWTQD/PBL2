#pragma once
#include "SDLHelper.h"
#include "SDLInterface.h"
#include "CityMap.h"
#define WINDOW_WIDTH 1400
#define WINDOW_HEIGHT 700

using namespace std;

struct SDL_Button {
    int x;
    int y;
    int w;
    int h;
    string text;
    int fontSize;
    SDL_Color bgColor;
    SDL_Color fgColor;
    int pageId;
};
struct AlgorithmRow {
    string divideAlgo;
    string moveAlgo;
    string time;
    string dist;
    string balance;
};

extern int activeAlgorithm1;
extern int activeAlgorithm2;

class SDLInterface {
public:
    struct TableRow {
        int id;
        std::string typeStr;
        SDL_Rect deleteRect;
    };

    vector<TableRow> tableRows;
    vector<SDL_Button> buttons;
    SDL_Color originButtonColor = {0xA2, 0xA5, 0xB1, 0xFF};
    Uint32 logoStartTime = 0;
    int currentPageId = 1;
    int totalHeight = 0;
    int scrollBarX = 1345;
    int scrollBarY = 80;
    int scrollBarW = 20;
    int scrollBarH = 580;
    int tableScrollY = 0;
    int tableMaxScroll = 0;
    bool isDraggingScrollbar = false;
    int dragOffsetY = 0;
    bool dataLoaded = false;
    bool customerRandomized = false;
    int selectedTop = -1;
    int selectedBottom = -1;
    int activeAlgorithm1 = ALGORITHM1_NONE;
    int activeAlgorithm2 = ALGORITHM2_NONE;
    bool needResetLogistics = false;
    bool logisticsStarted = false;
    bool logisticsFinished = false;

    SDLInterface();

    // Button
    static bool SDLButton_isContain(const SDL_Pos &_p, const SDL_Button &_button);
    static void SDLButton_draw(SDL_Renderer *renderer, const SDL_Button &_button);
    void addButton(const SDL_Button &button) { buttons.push_back(button); }

    // Logo
    void setLogoStartTime(Uint32 startTime) { logoStartTime = startTime; }
    static void DrawDLCarrierLogo(SDL_Renderer* renderer, Uint32, int centerX, int centerY);

    // Render preset
    void setCurrentPage(int pageId) { currentPageId = pageId; }
    int getCurrentPage() const { return currentPageId; }
    void SDLRenderPreset(SDL_Renderer *renderer);

    // MessageBox
    static void ShowMessageBox(const char *message, SDL_Window *window);

    // Table
    bool LoadFromFile(const string& filename);
    void ShowMap(SDL_Renderer* renderer, TTF_Font* font, CityMap& cityMap);
    void ShowData(SDL_Renderer* renderer, TTF_Font* font, CityMap& cityMap, vector<Driver>& drivers, vector<Customer>& customers, vector<Warehouse>& warehouses);
    vector<AlgorithmRow> LoadAlgorithmHistory(const string& filename);
    void ShowAlgorithm(SDL_Renderer* renderer);

    void HandleEvents(SDL_Event& event, bool& isRunning, bool& cityMapGenerated, AddMode& addMode, EdgeMode& edgeMode, SDL_Renderer* renderer, TTF_Font* font, CityMap& cityMap);
};
