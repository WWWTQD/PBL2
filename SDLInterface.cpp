#include "SDLInterface.h"
#include "SDLHelper.h"
#include "CityMap.h"
#include "SDL.h"
#include <sstream>
#include <fstream>
#include <algorithm>

SDLInterface::SDLInterface() {
    SDL_Button btnArray[34] = {
        {610, 550, 160, 60, "START", 30, {0xA2,0xA5,0xB1,0xFF}, {0,0,0,0xFF}, 1},   //0
        {360, 550, 280, 60, "THUC_AN", 28, {0xFE,0x99,0x00,0xFF}, {0,0,0,0xFF}, 2}, //1
        {740, 550, 280, 60, "VAT_PHAM", 28, {0xFE,0x99,0x00,0xFF}, {0,0,0,0xFF}, 2},//2
        {1150,662,90,35,"RUN",30,{0xA2,0xA5,0xB1,0xFF},{0,0,0,0xFF},6},             //3
        {1080,650,95,35,"RESET",30,{0xA2,0xA5,0xB1,0xFF},{0,0,0,0xFF},3},           //4
        {20,650,170,35,"Customer",30,{0xFE,0x99,0x00,0xFF},{0,0,0,0xFF},3},         //5
        {200,650,170,35,"Warehouse",30,{0xFE,0x99,0x00,0xFF},{0,0,0,0xFF},3},       //6
        {380,650,170,35,"Driver",30,{0xFE,0x99,0x00,0xFF},{0,0,0,0xFF},3},          //7
        {560,650,170,35,"Edge",30,{0xFE,0x99,0x00,0xFF},{0,0,0,0xFF},3},            //8
        {990,650,70,35,"LIST",30,{0xA2,0xA5,0xB1,0xFF},{0,0,0,0xFF},3},             //9
        {880,662,75,35,"MAP",30,{0xA2,0xA5,0xB1,0xFF},{0,0,0,0xFF},4},              //10
        {1250,662,90,35,"BACK",30,{0xA2,0xA5,0xB1,0xFF},{0,0,0,0xFF},4},            //11
        {1250,662,90,35,"BACK",30,{0xA2,0xA5,0xB1,0xFF},{0,0,0,0xFF},5},            //12
        {1100,662,135,35,"RANDOM",30,{0xA2,0xA5,0xB1,0xFF},{0,0,0,0xFF},4},         //13
        {1250,662,90,35,"BACK",30,{0xA2,0xA5,0xB1,0xFF},{0,0,0,0xFF},8},            //14
        {685,662,180,35, "ALGORITHM",30,{0xA2,0xA5,0xB1,0xFF},{0,0,0,0xFF},4},      //15
        {1250,662,90,35,"BACK",30,{0xA2,0xA5,0xB1,0xFF},{0,0,0,0xFF},6},            //16
        {150,380,320,40,"RANDOM",30,{0xA2,0xA5,0xB1,0xFF},{0,0,0,0xFF},6},          //17
        {150,470,320,40,"GREEDY",30,{0xA2,0xA5,0xB1,0xFF},{0,0,0,0xFF},6},          //18
        {150,560,320,40,"HUNGARIAN",30,{0xA2,0xA5,0xB1,0xFF},{0,0,0,0xFF},6},       //19
        {550,380,320,40,"K-MEANS",30,{0xA2,0xA5,0xB1,0xFF},{0,0,0,0xFF},6},         //20
        {550,470,320,40,"CLUSTERTSP",30,{0xA2,0xA5,0xB1,0xFF},{0,0,0,0xFF},6},             //21
        {550,560,320,40,"HUNGARIAN&K-MEANS",30,{0xA2,0xA5,0xB1,0xFF},{0,0,0,0xFF},6},           //22
        {950,380,320,40,"NEAREST NEIGHBOR",30,{0xA2,0xA5,0xB1,0xFF},{0,0,0,0xFF},6},//23
        {950,470,320,40,"PREFER CUSTOMER",30,{0xA2,0xA5,0xB1,0xFF},{0,0,0,0xFF},6}, //24
        {950,560,320,40,"PREFER WAREHOUSE",30,{0xA2,0xA5,0xB1,0xFF},{0,0,0,0xFF},6},//25
        {2000,380,160,40,"HELLO4",30,{0xA2,0xA5,0xB1,0xFF},{0,0,0,0xFF},6},         //26
        {2000,470,160,40,"HELLO5",30,{0xA2,0xA5,0xB1,0xFF},{0,0,0,0xFF},6},         //27
        {2000,560,160,40,"HELLO6",30,{0xA2,0xA5,0xB1,0xFF},{0,0,0,0xFF},6},         //28
        {530,662,140,35,"HISTORY",30,{0xA2,0xA5,0xB1,0xFF},{0,0,0,0xFF},4},         //29
        {1135,662,100,35,"RESET",30,{0xA2,0xA5,0xB1,0xFF},{0,0,0,0xFF},7},          //30
        {1250,662,90,35,"BACK",30,{0xA2,0xA5,0xB1,0xFF},{0,0,0,0xFF},7},            //31
        {1015,662,105,35,"PAUSE",30,{0xA2,0xA5,0xB1,0xFF},{0,0,0,0xFF},7},          //32
        {875,662,125,35,"RESUME",30,{0xA2,0xA5,0xB1,0xFF},{0,0,0,0xFF},7},          //33
    };

    buttons.assign(btnArray, btnArray + 34);
    originButtonColor = {100, 100, 100, 255};
}

// ========================================================= BUTTON =========================================================//
bool SDLInterface::SDLButton_isContain(const SDL_Pos &_p, const SDL_Button &_button) {
    return _p.x >= _button.x && _p.x <= _button.x + _button.w &&
           _p.y >= _button.y && _p.y <= _button.y + _button.h;
}

void SDLInterface::SDLButton_draw(SDL_Renderer *renderer, const SDL_Button &_button) {
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, _button.bgColor.r, _button.bgColor.g, _button.bgColor.b, _button.bgColor.a);
    SDL_Rect rect{_button.x,_button.y,_button.w,_button.h};
    SDL_RenderFillRect(renderer,&rect);

    SDL_SetRenderDrawColor(renderer,_button.fgColor.r,_button.fgColor.g,_button.fgColor.b,_button.fgColor.a);
    SDL_Texture* textTexture = SDLHelper::getTextTexture(renderer, _button.text, _button.fontSize, _button.fgColor);
    int textRectW = rect.w-6, textRectH = rect.h-6;
    SDL_QueryTexture(textTexture, nullptr,nullptr,&textRectW,&textRectH);
    SDL_Rect textRect = {rect.x+(rect.w-textRectW)/2, rect.y+(rect.h-textRectH)/2, textRectW, textRectH};
    SDL_RenderCopy(renderer, textTexture,nullptr,&textRect);
    SDL_DestroyTexture(textTexture);
}

// ========================================================= LOGO =========================================================//
void SDLInterface::DrawDLCarrierLogo(SDL_Renderer* renderer, Uint32 startTime, int centerX, int centerY)
{
   if (startTime == 0) return; // chưa khởi tạo

   Uint32 elapsed = SDL_GetTicks() - startTime;

   // fade-in trong 1000 ms
   float t = std::min(1.0f, static_cast<float>(elapsed) / 1000.0f);
   auto baseAlpha = static_cast<Uint8>(t * 200); // alpha tối đa 200 (không quá mạnh)
   if (baseAlpha > 200) baseAlpha = 200;

   // pulse nhẹ (sử dụng sin) để tăng/giảm size
   float pulse = 1.0f + 0.02f * sin(elapsed / 200.0f); // +/-2%

   // thiết lập blend mode
   SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

   //  // vị trí trung tâm
   // int centerX = WINDOW_WIDTH / 2.5;
   // int centerY = WINDOW_HEIGHT / 2.7;

    // Vẽ biểu tượng hộp (vuông bo góc nhẹ) bên trái chữ
   auto boxSize = static_cast<int>(120 * pulse);
   int boxX = centerX - 300;
   int boxY = centerY - boxSize / 2;

    // màu mờ vàng/xanh dương pha (nhạt)
   SDL_Color iconColor = {0, 200, 220, baseAlpha}; // xanh dương pha xanh lá, alpha = baseAlpha
   SDL_SetRenderDrawColor(renderer, iconColor.r, iconColor.g, iconColor.b, iconColor.a);

    SDL_Rect box{ boxX, boxY, boxSize, boxSize };
   SDL_RenderFillRect(renderer, &box);

    // vẽ "nắp" hộp (giả 3D) - một đường trắng nhẹ để tạo chi tiết
   SDL_SetRenderDrawColor(renderer, 255, 255, 255, baseAlpha /4);
   SDL_Rect lid{ boxX + boxSize / 8, boxY + boxSize / 8, boxSize * 3 / 4, boxSize / 6 };
   SDL_RenderFillRect(renderer, &lid);

    // Vẽ mũi tên nằm bên phải hộp (một tam giác đơn)
   SDL_SetRenderDrawColor(renderer, iconColor.r, iconColor.g, iconColor.b, iconColor.a);
   SDL_Point arrow[4];
   int ax = boxX + boxSize + 20;
   int ay = centerY;
   int aw = static_cast<int>(80 * pulse);
   int ah = static_cast<int>(40 * pulse);
   arrow[0] = { ax, ay };
   arrow[1] = { ax + aw, ay - ah };
   arrow[2] = { ax + aw, ay + ah };
   arrow[3] = { ax, ay };
   SDL_RenderDrawLines(renderer, arrow, 4);
   // Fill arrow (simple fill by drawing lines inside)
   for (int i = -ah; i <= ah; i++) {
       SDL_RenderDrawLine(renderer, ax, ay + i, ax + aw - abs(i), ay + i);
   }

    // Vẽ chữ "DLCarrier" (mờ) ở giữa, sau icon
   string logoText = "DLCarrier";
   int fontSize = static_cast<int>(160 * pulse);
   SDL_Color textColor = {0, 255, 200, 255}; // xanh dương nhạt với alpha

    SDL_Texture* logoTexture = SDLHelper::getTextTexture(renderer, logoText, fontSize, textColor);
   if (logoTexture) {
       int w, h;
       SDL_QueryTexture(logoTexture, nullptr, nullptr, &w, &h);

       SDL_Rect dst;
       dst.w = w;
       dst.h = h;
       dst.x = ax + aw - 40; // đặt sau mũi tên
       dst.y = centerY - h / 2 + 70;

       // Slight rotation for watermark effect: small negative angle (e.g. -8 deg)
       double angle = -8.0;
       SDL_SetTextureAlphaMod(logoTexture, baseAlpha);
       SDL_RenderCopyEx(renderer, logoTexture, nullptr, &dst, angle, nullptr, SDL_FLIP_NONE);
       SDL_DestroyTexture(logoTexture);

       // Draw the diagonal dashed line under the logo text
       int dashWidth = w;  // Start with the same width as the text
       SDL_SetRenderDrawColor(renderer, 30, 144, 255, baseAlpha / 2); // Light blue dashed line

       // Angle for the dashed line (adjust the angle here)
       double lineAngle =-15.0;  // Change this value to control the line's tilt

       // The dashed line will start below the bottom of the text
       int startX = dst.x;               // Starting X based on the text position
       int startY = dst.y + h / 2 + 10;  // Starting Y is just below the text (adjust for gap)

       for (int i = 0; i < 15; ++i) { // Increase the number of dashes
           int dashHeight = 10; // Thicker dashes
           int dashX = startX + (i * dashWidth / 15); // Position dashes progressively
           int dashY = startY + i * dashHeight; // Starting point under the text
           // Calculate the endpoint of each dash with an angle
           int dashEndX = dashX + static_cast<int>(dashWidth * 0.85 * cos(lineAngle * M_PI / 180));
           int dashEndY = dashY + static_cast<int>(dashWidth * 0.85 * sin(lineAngle * M_PI / 180));
           SDL_RenderDrawLine(renderer, dashX, dashY, dashEndX, dashEndY);
           dashWidth = static_cast<int>(dashWidth * 0.85); // Shrink the width of each dash
       }
   }
}

// ========================================================= RENDER PRESET =========================================================//
void SDLInterface::SDLRenderPreset(SDL_Renderer *renderer) {
   SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
   SDL_RenderClear(renderer);

   if (currentPageId == 2) {
       DrawDLCarrierLogo(renderer, logoStartTime, WINDOW_WIDTH / 2.5, WINDOW_HEIGHT / 2.7);
   }
    if (currentPageId == 6) {
        DrawDLCarrierLogo(renderer, logoStartTime, WINDOW_WIDTH / 2.2, WINDOW_HEIGHT / 5.5);
    }
   if (currentPageId == 1) {
       SDL_SetRenderDrawColor(renderer, 0, 255, 200, 255);
       SDL_Rect containerRect{50, 40, 1300, 620};
       SDL_RenderDrawRect(renderer, &containerRect);

       SDL_Rect PBL_TITLE_rect{containerRect.x + 310, containerRect.y + 30, 1, 1};
       SDLHelper::SDL_RenderText(renderer, "PBL2: DO AN CO SO LAP TRINH", 48, {255, 255, 255, 255}, PBL_TITLE_rect);

       SDL_Rect PBL_PROJECT_NAME1_rect{containerRect.x + 190, PBL_TITLE_rect.y + 80, 0, 0};
       SDLHelper::SDL_RenderText(renderer, "DE TAI: MO PHONG VA TOI UU HOA HANH TRINH CUA SHIPPER", 32, {0, 255, 200, 255}, PBL_PROJECT_NAME1_rect);

       SDL_Rect PBL_PROJECT_NAME2_rect{containerRect.x + 150, PBL_PROJECT_NAME1_rect.y + 50, 0, 0};
       SDLHelper::SDL_RenderText(renderer, "CONG NGHE KET HOP GIAO THUC AN DUA TREN GIAI THUAT DO THI", 32, {0, 255, 200, 255}, PBL_PROJECT_NAME2_rect);

       SDL_Rect SV_THUC_HIEN_rect{containerRect.x + 100, PBL_PROJECT_NAME2_rect.y + 100, 0, 0};
       SDLHelper::SDL_RenderText(renderer, "SINH VIEN THUC HIEN", 36, {255, 255, 255, 255}, SV_THUC_HIEN_rect);

       SDL_Rect SV_1_rect{SV_THUC_HIEN_rect.x, SV_THUC_HIEN_rect.y + 60, 0, 0};
       SDLHelper::SDL_RenderText(renderer, "Truong Quang Dat - 102240304", 28, {200, 200, 200, 255}, SV_1_rect);

       SDL_Rect SV_2_rect{SV_THUC_HIEN_rect.x, SV_1_rect.y + 40, 0, 0};
       SDLHelper::SDL_RenderText(renderer, "Nguyen Hai Long - 102240318", 28, {200, 200, 200, 255}, SV_2_rect);

       SDL_Rect GV_HUONG_DAN_rect{containerRect.x + containerRect.w - 510, PBL_PROJECT_NAME2_rect.y + 100, 0, 0};
       SDLHelper::SDL_RenderText(renderer, "GIANG VIEN HUONG DAN", 36, {255, 255, 255, 255}, GV_HUONG_DAN_rect);

       SDL_Rect GV_rect{GV_HUONG_DAN_rect.x + 40, GV_HUONG_DAN_rect.y + 60, 0, 0};
       SDLHelper::SDL_RenderText(renderer, "TS. PHAM MINH TUAN", 28, {200, 200, 200, 255}, GV_rect);
   }
   if (currentPageId == 2) {
       SDL_Rect WELCOME_rect{200, 80, 0, 0};
       SDLHelper::SDL_RenderText(renderer, "Chao mung ban den voi dich vu giao hang cua DLCarrier", 40, {0, 255, 200, 255}, WELCOME_rect);
   }
    if (currentPageId == 6) {
        SDL_SetRenderDrawColor(renderer, 0, 255, 200, 255);
        SDL_Rect containerRect{50, 40, 1300, 620};
        SDL_RenderDrawRect(renderer, &containerRect);

        SDL_SetRenderDrawColor(renderer, 0, 255, 200, 255);
        SDL_RenderDrawLine(renderer, 910, 370, 910, 650);

        SDL_Rect WELCOME_rect{190, 230, 0, 0};
        SDLHelper::SDL_RenderText(renderer, "ALGORITHM", 45, {0, 255, 200, 255}, WELCOME_rect);
    }
    for (const auto& button : buttons) {
        if (button.pageId == currentPageId) {
            SDLButton_draw(renderer, button);
        }
    }
    SDL_RenderPresent(renderer);
}

// ========================================================= MESSAGE BOX =========================================================//
void SDLInterface::ShowMessageBox(const char *message, SDL_Window *window) {
    SDL_MessageBoxButtonData buttons[] = {
        { SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 0, "Ok" }
    };
    SDL_MessageBoxData messageboxdata = {
        SDL_MESSAGEBOX_INFORMATION,
        window,
        "Caution",
        message,
        SDL_arraysize(buttons),
        buttons,
        nullptr
    };
    int buttonId;
    if(SDL_ShowMessageBox(&messageboxdata,&buttonId)<0)
        SDLHelper::SDL_ErrorHandler("SDL_ShowMessageBox", SDL_GetError());
}

void SDLInterface::ShowMap(SDL_Renderer* renderer, TTF_Font* font, CityMap& cityMap)
{
    tableRows.clear();

    if (tableScrollY < 0)
        tableScrollY = 0;

    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
    SDL_RenderClear(renderer);

    SDL_Rect tableRect{0, 0, 1400, 700};
    SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
    SDL_RenderFillRect(renderer, &tableRect);

    SDL_Rect headerRect{0, 0, 1400, 50};
    SDL_SetRenderDrawColor(renderer, 60, 60, 60, 255);
    SDL_RenderFillRect(renderer, &headerRect);

    SDLHelper::SDL_RenderText(renderer, "MAP DATA (INTERSECTIONS + EDGES)",
        36, {220,220,220,255}, SDL_Rect{20, 10, 0, 0});

    SDL_Rect clipArea{40, 80, 1300, 580};
    SDL_RenderSetClipRect(renderer, &clipArea);

    int rowY = clipArea.y - tableScrollY;
    int rowHeight = 36;
    int spacing = 4;
    totalHeight = 0;

    auto DrawRow = [&](string col1, string col2, string col3, string col4, bool header=false)
    {
        SDL_Rect rowRect{clipArea.x, rowY, clipArea.w, rowHeight};

        bool visible =
            !(rowY + rowHeight < clipArea.y ||
              rowY > clipArea.y + clipArea.h);

        if (visible)
        {
            SDL_Color textColor = header ? SDL_Color{220,220,220,255} : SDL_Color{180,180,180,255};
            SDL_Color bgColor   = header ? SDL_Color{80,80,80,255}  : SDL_Color{30,30,30,255};

            SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
            SDL_RenderFillRect(renderer, &rowRect);

            int col1X = clipArea.x + 10;
            int col2X = clipArea.x + 250;
            int col3X = clipArea.x + 450;
            int col4X = clipArea.x + 650;

            SDLHelper::SDL_RenderText(renderer, col1, 24, textColor, SDL_Rect{col1X, rowY + 6, 0, 0});
            SDLHelper::SDL_RenderText(renderer, col2, 24, textColor, SDL_Rect{col2X, rowY + 6, 0, 0});
            SDLHelper::SDL_RenderText(renderer, col3, 24, textColor, SDL_Rect{col3X, rowY + 6, 0, 0});
            SDLHelper::SDL_RenderText(renderer, col4, 24, textColor, SDL_Rect{col4X, rowY + 6, 0, 0});
        }

        rowY += rowHeight + spacing;
        totalHeight += rowHeight + spacing;
    };

    // ====== INTERSECTIONS ======
    DrawRow("INTERSECTION ID", "X", "Y", "-", true);

    for (auto& inter : cityMap.intersections)
    {
        DrawRow(
            to_string(inter.id),
            to_string(inter.x),
            to_string(inter.y),
            "-"
        );
    }

    // ====== EDGES ======
    DrawRow("EDGE ID", "FROM", "TO", "LENGTH", true);

    for (auto& e : cityMap.edges)
    {
        DrawRow(
            to_string(e.id),
            to_string(e.from + 1),
            to_string(e.to + 1),
            to_string(e.length)
        );
    }

    SDL_RenderSetClipRect(renderer, NULL);

    // ===== Scrollbar =====
    tableMaxScroll = max(0, totalHeight - clipArea.h);

    SDL_Rect scrollBg{clipArea.x + clipArea.w + 5, clipArea.y, 20, clipArea.h};
    SDL_SetRenderDrawColor(renderer, 70, 70, 70, 255);
    SDL_RenderFillRect(renderer, &scrollBg);

    if (tableMaxScroll > 0)
    {
        float ratio = (float)clipArea.h / totalHeight;
        int handleH = max(40, (int)(scrollBg.h * ratio));
        int handleY = scrollBg.y + ((float)tableScrollY / tableMaxScroll) * (scrollBg.h - handleH);

        SDL_SetRenderDrawColor(renderer, 120, 120, 120, 255);
        SDL_Rect handle{scrollBg.x, handleY, scrollBg.w, handleH};
        SDL_RenderFillRect(renderer, &handle);
    }
    tableMaxScroll = max(0, totalHeight - clipArea.h);

    if (tableScrollY > tableMaxScroll)
        tableScrollY = tableMaxScroll;
}

// ========================================================= TABLE =========================================================//
void SDLInterface::ShowData(SDL_Renderer* renderer, TTF_Font* font,
    CityMap& cityMap,
    vector<Driver>& drivers,
    vector<Customer>& customers,
    vector<Warehouse>& warehouses)
{
    tableRows.clear();

    if (tableScrollY < 0) tableScrollY = 0;

    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
    SDL_RenderClear(renderer);

    SDL_Rect tableRect{0, 0, 1400, 700};
    SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
    SDL_RenderFillRect(renderer, &tableRect);

    SDL_Rect headerRect{0, 0, 1400, 50};
    SDL_SetRenderDrawColor(renderer, 60, 60, 60, 255);
    SDL_RenderFillRect(renderer, &headerRect);

    SDLHelper::SDL_RenderText(renderer, "DATA TABLE", 36,
        {200,200,200,255}, SDL_Rect{20, 10, 0, 0});

    SDL_Rect clipArea{40, 80, 1300, 580};
    SDL_RenderSetClipRect(renderer, &clipArea);

    int rowY = clipArea.y - tableScrollY;
    int rowHeight = 36;
    int spacing = 4;
    totalHeight = 0;

    // reset click zones mỗi frame
    tableRows.clear();

    auto DrawRow = [&](int id, string typeStr, string col1, string col2, string col3, string col4, bool header=false)
    {
        SDL_Rect rowRect{clipArea.x, rowY, clipArea.w, rowHeight};

        bool visible =
            !(rowY + rowHeight < clipArea.y ||
              rowY > clipArea.y + clipArea.h);

        if (visible)
        {
            SDL_Color textColor = header ? SDL_Color{220,220,220,255} : SDL_Color{180,180,180,255};
            SDL_Color bgColor   = header ? SDL_Color{80,80,80,255}  : SDL_Color{30,30,30,255};

            SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
            SDL_RenderFillRect(renderer, &rowRect);

            int col1X = clipArea.x + 10;
            int col2X = clipArea.x + 250;
            int col3X = clipArea.x + 350;
            int col4X = clipArea.x + 450;
            int delX  = clipArea.x + 1100;

            SDLHelper::SDL_RenderText(renderer, col1, 24, textColor, SDL_Rect{col1X, rowY+6, 0, 0});
            SDLHelper::SDL_RenderText(renderer, col2, 24, textColor, SDL_Rect{col2X, rowY+6, 0, 0});
            SDLHelper::SDL_RenderText(renderer, col3, 24, textColor, SDL_Rect{col3X, rowY+6, 0, 0});
            SDLHelper::SDL_RenderText(renderer, col4, 24, textColor, SDL_Rect{col4X, rowY+6, 0, 0});

            if(!header && !typeStr.empty())
            {
                SDL_Rect delRect{delX, rowY + 4, 100, 30};

                SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
                SDL_RenderFillRect(renderer, &delRect);

                SDLHelper::SDL_RenderText(renderer, "[Delete]", 24,
                    {255,100,100,255},
                    SDL_Rect{delRect.x + 10, delRect.y + 2, 0, 0});

                tableRows.push_back({id, typeStr, delRect});
            }
        }

        rowY += rowHeight + spacing;
        totalHeight += rowHeight + spacing;
    };

    // ====== HELPERS ======
    auto GetDriverCol4 = [&](const Driver& d) -> string {
        if (d.customerIds.empty()) return "None";
        string res;
        for (size_t i = 0; i < d.customerIds.size(); ++i) {
            if (i > 0) res += " | ";
            res += "C" + to_string(d.customerIds[i]) + "(W" + to_string(d.warehouseIds[i]) + ")";
        }
        return res;
    };

    auto GetCustomerCol4 = [&](const Customer& c) -> string {
        if (c.warehouseIds.empty()) return "None";
        string res;
        for (size_t i = 0; i < c.warehouseIds.size(); ++i) {
            if (i > 0) res += " | ";
            res += "W" + to_string(c.warehouseIds[i]);
        }
        return res;
    };

    auto GetWarehouseCol4 = [&](const Warehouse& w) -> string {
        if (w.driverIds.empty()) return "None";
        string res;
        for (size_t i = 0; i < w.driverIds.size(); ++i) {
            int did = w.driverIds[i];
            const Driver* dptr = nullptr;
            for (const auto& d : drivers)
                if (d.id == did) { dptr = &d; break; }

            if (!dptr) continue;

            for (size_t j = 0; j < dptr->customerIds.size(); ++j) {
                if (dptr->warehouseIds[j] == w.id) {
                    if (!res.empty()) res += " | ";
                    res += "C" + to_string(dptr->customerIds[j]) + "(D" + to_string(did) + ")";
                }
            }
        }
        if (res.empty()) res = "None";
        return res;
    };

    // ====== DRAW DATA ======
    DrawRow(-1, "CUSTOMER", "CUSTOMER ID", "X", "Y", "WAREHOUSE", true);
    for (auto& c : customers)
        DrawRow(c.id, "CUSTOMER", to_string(c.id), to_string(c.x), to_string(c.y), GetCustomerCol4(c));

    DrawRow(-1, "WAREHOUSE", "WAREHOUSE ID", "X", "Y", "CUSTOMER(DRIVER)", true);
    for (auto& w : warehouses)
        DrawRow(w.id, "WAREHOUSE", to_string(w.id), to_string(w.x), to_string(w.y), GetWarehouseCol4(w));

    DrawRow(-1, "DRIVER", "DRIVER ID", "X", "Y", "CUSTOMER(WAREHOUSE)", true);
    for (auto& d : drivers)
        DrawRow(d.id, "DRIVER", to_string(d.id), to_string(d.x), to_string(d.y), GetDriverCol4(d));

    SDL_RenderSetClipRect(renderer, nullptr);

    // ====== Scrollbar ======
    tableMaxScroll = max(0, totalHeight - clipArea.h);

    SDL_Rect scrollBg{clipArea.x + clipArea.w + 5, clipArea.y, 20, clipArea.h};
    SDL_SetRenderDrawColor(renderer, 70, 70, 70, 255);
    SDL_RenderFillRect(renderer, &scrollBg);

    if (tableMaxScroll > 0)
    {
        float ratio = (float)clipArea.h / totalHeight;
        int handleH = max(40, (int)(scrollBg.h * ratio));
        int handleY = scrollBg.y + ((float)tableScrollY / tableMaxScroll) * (scrollBg.h - handleH);

        SDL_SetRenderDrawColor(renderer, 120, 120, 120, 255);
        SDL_Rect handle{scrollBg.x, handleY, scrollBg.w, handleH};
        SDL_RenderFillRect(renderer, &handle);
    }
    tableMaxScroll = max(0, totalHeight - clipArea.h);
    if (tableScrollY > tableMaxScroll)
        tableScrollY = tableMaxScroll;
}

vector<AlgorithmRow> SDLInterface::LoadAlgorithmHistory(const string& filename)
{
    vector<AlgorithmRow> rows;
    ifstream fin(filename);
    if (!fin.is_open()) return rows;

    string line;
    while (getline(fin, line))
    {
        if (line.empty()) continue;

        stringstream ss(line);
        AlgorithmRow r;
        string tmp;

        getline(ss, r.divideAlgo, '|');
        getline(ss, r.moveAlgo,   '|');
        getline(ss, r.time,       '|');
        getline(ss, r.dist,       '|');
        getline(ss, tmp); // balance=0.xx

        // trim space
        auto trim = [](string& s) {
            while (!s.empty() && s.front() == ' ') s.erase(s.begin());
            while (!s.empty() && s.back()  == ' ') s.pop_back();
        };

        trim(r.divideAlgo);
        trim(r.moveAlgo);
        trim(r.time);
        trim(r.dist);

        if (tmp.find("balance=") != string::npos)
            r.balance = tmp.substr(tmp.find("=") + 1);
        else
            r.balance = tmp;

        trim(r.balance);

        rows.push_back(r);
    }
    return rows;
}
void SDLInterface::ShowAlgorithm(SDL_Renderer* renderer)
{
    tableRows.clear(); // Reset bảng nếu cần scroll
    if (tableScrollY < 0) tableScrollY = 0;

    // ===== Background =====
    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
    SDL_RenderClear(renderer);

    SDL_Rect tableRect{0, 0, 1400, 700};
    SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
    SDL_RenderFillRect(renderer, &tableRect);

    SDL_Rect headerRect{0, 0, 1400, 50};
    SDL_SetRenderDrawColor(renderer, 60, 60, 60, 255);
    SDL_RenderFillRect(renderer, &headerRect);

    SDLHelper::SDL_RenderText(renderer, "ALGORITHM HISTORY", 36,
        {220,220,220,255}, SDL_Rect{20, 10, 0, 0});

    // ===== Clip area =====
    SDL_Rect clipArea{40, 80, 1300, 580};
    SDL_RenderSetClipRect(renderer, &clipArea);

    int rowY = clipArea.y - tableScrollY;
    int rowHeight = 36;
    int spacing = 4;
    totalHeight = 0;

    vector<AlgorithmRow> data = LoadAlgorithmHistory("Algorithm.txt");

    auto DrawRow = [&](AlgorithmRow r, bool header=false)
    {
        SDL_Rect rowRect{clipArea.x, rowY, clipArea.w, rowHeight};
        bool visible = !(rowY + rowHeight < clipArea.y || rowY > clipArea.y + clipArea.h);

        if (visible)
        {
            SDL_Color textColor = header ? SDL_Color{220,220,220,255} : SDL_Color{180,180,180,255};
            SDL_Color bgColor   = header ? SDL_Color{80,80,80,255}  : SDL_Color{30,30,30,255};

            SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
            SDL_RenderFillRect(renderer, &rowRect);

            int colX[] = {clipArea.x + 10, clipArea.x + 250, clipArea.x + 500, clipArea.x + 700, clipArea.x + 900};
            SDLHelper::SDL_RenderText(renderer, r.divideAlgo, 24, textColor, SDL_Rect{colX[0], rowY+6, 0, 0});
            SDLHelper::SDL_RenderText(renderer, r.moveAlgo,   24, textColor, SDL_Rect{colX[1] + 150, rowY+6, 0, 0});
            SDLHelper::SDL_RenderText(renderer, r.time,       24, textColor, SDL_Rect{colX[2] + 250, rowY+6, 0, 0});
            SDLHelper::SDL_RenderText(renderer, r.dist,       24, textColor, SDL_Rect{colX[3] + 250, rowY+6, 0, 0});
            SDLHelper::SDL_RenderText(renderer, r.balance,    24, textColor, SDL_Rect{colX[4] + 230, rowY+6, 0, 0});
        }

        rowY += rowHeight + spacing;
        totalHeight += rowHeight + spacing;
    };

    // ===== Header =====
    DrawRow({"ALGORITHM DIVIDE","ALGORITHM MOVE","TIME","DIST","%BALANCE"}, true);

    // ===== Data rows =====
    for (auto& r : data)
        DrawRow(r);

    SDL_RenderSetClipRect(renderer, nullptr);

    // ===== Scrollbar =====
    tableMaxScroll = std::max(0, totalHeight - clipArea.h);

    SDL_Rect scrollBg{clipArea.x + clipArea.w + 5, clipArea.y, 20, clipArea.h};
    SDL_SetRenderDrawColor(renderer, 70, 70, 70, 255);
    SDL_RenderFillRect(renderer, &scrollBg);

    if (tableMaxScroll > 0)
    {
        float ratio = (float)clipArea.h / totalHeight;
        int handleH = std::max(40, (int)(scrollBg.h * ratio));
        int handleY = scrollBg.y + ((float)tableScrollY / tableMaxScroll) * (scrollBg.h - handleH);

        SDL_SetRenderDrawColor(renderer, 120, 120, 120, 255);
        SDL_Rect handle{scrollBg.x, handleY, scrollBg.w, handleH};
        SDL_RenderFillRect(renderer, &handle);
    }

    if (tableScrollY > tableMaxScroll)
        tableScrollY = tableMaxScroll;

}



void SDLInterface::HandleEvents(SDL_Event& event,
                                bool& isRunning,
                                bool& cityMapGenerated,
                                AddMode& addMode,
                                EdgeMode& edgeMode,
                                SDL_Renderer* renderer,
                                TTF_Font* font,
                                CityMap& cityMap)
{
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                isRunning = false;
                break;

            case SDL_MOUSEBUTTONDOWN: {
                SDL_Pos p;
                SDL_GetMouseState(&p.x, &p.y);

                // ===== Page switching =====
                if (currentPageId == 1 && SDLButton_isContain(p, buttons[0])) { // START
                    currentPageId = 2;
                    logoStartTime = SDL_GetTicks();
                }
                if (currentPageId == 2 && SDLButton_isContain(p, buttons[1])) { // THUC_AN
                    currentPageId = 3;
                    cityMapGenerated = false;
                }
                if (currentPageId == 2 && SDLButton_isContain(p, buttons[2])) { // VAT_PHAM
                    currentPageId = 3;
                    cityMapGenerated = false;
                }
                if (currentPageId == 3 && SDLButton_isContain(p, buttons[9])) { // LIST
                    currentPageId = 4;
                }
                if (currentPageId == 4 && SDLButton_isContain(p, buttons[10])) { // MAP
                    currentPageId = 5;
                }
                if (currentPageId == 4 && SDLButton_isContain(p, buttons[11])) { // BACK
                    currentPageId = 3;
                    cityMapGenerated = false;
                }
                if (currentPageId == 5 && SDLButton_isContain(p, buttons[12])) { // BACK
                    currentPageId = 4;
                    cityMapGenerated = false;
                }
                if (currentPageId == 8 && SDLButton_isContain(p, buttons[14])) { // BACK
                    currentPageId = 4;
                    cityMapGenerated = false;
                }
                if (currentPageId == 4 && SDLButton_isContain(p, buttons[15])) { // ALGORITHM
                    currentPageId = 6;
                    logoStartTime = SDL_GetTicks();
                    cityMapGenerated = false;
                }
                if (currentPageId == 6 && SDLButton_isContain(p, buttons[16])) { // BACK
                    currentPageId = 4;
                }
                if (currentPageId == 4 && SDLButton_isContain(p, buttons[29])) { // HISTORY
                    currentPageId = 8;
                }
                if (currentPageId == 4 && SDLButton_isContain(p, buttons[13])) // RANDOM CUSTOMER
                {
                    Customer::RandomAssignToWarehouses(cityMap.customerObjs, cityMap.warehouseObjs);

                    cityMap.SaveData(cityMap.customerObjs, cityMap.driverObjs, cityMap.warehouseObjs, "Data.txt");
                    cityMap.SaveCityData("Citymap.txt");
                    dataLoaded = true;
                    ofstream("Algorithm.txt", ios::trunc).close();
                    return;
                }
                if (currentPageId == 6 && event.button.button == SDL_BUTTON_LEFT) {
                    for (size_t i = 17; i <= 28 && i < buttons.size(); i++)
                    {
                        if (buttons[i].pageId != 6) continue;
                        if (!SDLButton_isContain(p, buttons[i])) continue;

                        // ================= GROUP 1: ASSIGN =================
                        if (i >= 17 && i <= 22)
                        {
                            activeAlgorithm1 = i;
                            cityMap.ResetAllDriversAndLogistics();

                            switch (i)
                            {
                                case 17:
                                    cityMap.selectedAssignName = "Random";
                                    Warehouse::AssignByRandom(cityMap.warehouseObjs, cityMap.customerObjs, cityMap.driverObjs);
                                    break;

                                case 18:
                                    cityMap.selectedAssignName = "Greedy";
                                    Warehouse::AssignByGreedy(cityMap.warehouseObjs, cityMap.customerObjs, cityMap.driverObjs, cityMap);
                                    break;

                                case 19:
                                    cityMap.selectedAssignName = "Hungarian";
                                    Warehouse::AssignByHungarian(cityMap.warehouseObjs, cityMap.customerObjs, cityMap.driverObjs, cityMap);
                                    break;

                                case 20:
                                    cityMap.selectedAssignName = "K-Means";
                                    Warehouse::AssignByKMeans(cityMap.warehouseObjs, cityMap.customerObjs, cityMap.driverObjs);
                                    break;

                                case 21:
                                    cityMap.selectedAssignName = "Cluster&TSP";
                                    Warehouse::AssignByClusterTSP(cityMap.warehouseObjs, cityMap.customerObjs, cityMap.driverObjs, cityMap);
                                    break;

                                case 22:
                                    cityMap.selectedAssignName = "Hungarian&K-Means";
                                    Warehouse::AssignByHungarianKMeans(cityMap.warehouseObjs, cityMap.customerObjs, cityMap.driverObjs, cityMap);
                                    break;
                            }

                            // ===== Driver assign + save =====
                            Driver::RandomAssignToCustomerWarehouse(cityMap.driverObjs, cityMap.customerObjs, cityMap.warehouseObjs);
                            cityMap.SaveData( cityMap.customerObjs, cityMap.driverObjs, cityMap.warehouseObjs, "Data.txt");
                            cityMap.SaveCityData("Citymap.txt");
                        }
                        else if (i >= 23 && i <= 25)
                        {
                            activeAlgorithm2 = i;
                        }
                    }
                    // ================= RUN =================
                    if (SDLButton_isContain(p, buttons[3])) // RUN
                    {
                        if (activeAlgorithm1 != -1 && activeAlgorithm2 != -1)
                        {
                            currentPageId = 7;
                            logoStartTime = SDL_GetTicks();

                            cityMap.algorithmSaved = false;
                            cityMap.logisticsRunning  = true;
                            cityMap.logisticsPaused   = false;
                            cityMap.logisticsFinished = false;

                            needResetLogistics = true;
                        }
                    }
                }
                if (currentPageId == 3) {
                    // Customer
                    if (SDLButton_isContain(p, buttons[5])) {
                        if (addMode == ADD_CUSTOMER) {
                            addMode = ADD_NONE;
                            buttons[5].bgColor = {0xFE,0x99,0x00,0xFF};
                        } else {
                            addMode = ADD_CUSTOMER;
                            buttons[5].bgColor = {255,100,100,255};
                            buttons[6].bgColor = buttons[7].bgColor = {0xFE,0x99,0x00,0xFF};
                        }
                    }
                    // Warehouse
                    else if (SDLButton_isContain(p, buttons[6])) {
                        if (addMode == ADD_WAREHOUSE) {
                            addMode = ADD_NONE;
                            buttons[6].bgColor = {0xFE,0x99,0x00,0xFF};
                        } else {
                            addMode = ADD_WAREHOUSE;
                            buttons[6].bgColor = {255,215,0,255};
                            buttons[5].bgColor = buttons[7].bgColor = {0xFE,0x99,0x00,0xFF};
                        }
                    }
                    // Driver
                    else if (SDLButton_isContain(p, buttons[7])) {
                        if (addMode == ADD_DRIVER) {
                            addMode = ADD_NONE;
                            buttons[7].bgColor = {0xFE,0x99,0x00,0xFF};
                        } else {
                            addMode = ADD_DRIVER;
                            buttons[7].bgColor = {0,200,255,255};
                            buttons[5].bgColor = buttons[6].bgColor = {0xFE,0x99,0x00,0xFF};
                        }
                    }
                    // Edge
                    if (SDLButton_isContain(p, buttons[8])) {
                        edgeMode = (edgeMode == EDGE_ON) ? EDGE_NONE : EDGE_ON;
                        if (edgeMode == EDGE_ON) {
                            switch(addMode) {
                                case ADD_CUSTOMER: buttons[8].bgColor = {255,100,100,255}; break;
                                case ADD_WAREHOUSE: buttons[8].bgColor = {255,215,0,255}; break;
                                case ADD_DRIVER: buttons[8].bgColor = {0,200,255,255}; break;
                                default: buttons[8].bgColor = {255,105,180,255}; break;
                            }
                        } else {
                            buttons[8].bgColor = {0xFE,0x99,0x00,0xFF};
                        }
                    }
                    // Click map
                    if (event.button.button == SDL_BUTTON_LEFT && p.y <= 640) {
                        cityMap.HandleLeftClick(renderer, p, addMode, edgeMode);
                    }
                    // Reset / Clear
                    if (SDLButton_isContain(p, buttons[4])) {
                        cityMap.intersections.clear();
                        cityMap.edges.clear();
                        cityMap.driverObjs.clear();
                        cityMap.customerObjs.clear();
                        cityMap.warehouseObjs.clear();
                        ofstream("Citymap.txt", ios::trunc).close();
                        ofstream("Algorithm.txt", ios::trunc).close();
                        cityMapGenerated = false;
                    }

                }
                bool needRedrawTable = false;
                int deleteTargetId = -1;
                std::string deleteTargetType;

                if ((currentPageId == 4 || currentPageId == 5 || currentPageId == 8) && event.button.button == SDL_BUTTON_LEFT) {
                    int mx = event.button.x;
                    int my = event.button.y;

                    for (size_t i = 0; i < tableRows.size(); ++i) {
                        SDL_Rect delRect = tableRows[i].deleteRect;

                        if (mx >= delRect.x && mx <= delRect.x + delRect.w &&
                            my >= delRect.y && my <= delRect.y + delRect.h) {
                            deleteTargetId = tableRows[i].id;
                            deleteTargetType = tableRows[i].typeStr;
                            break;
                        }
                    }
                }
                if (deleteTargetId != -1) {
                    if (deleteTargetType == "DRIVER") {
                        cityMap.driverObjs.erase(
                            std::remove_if(cityMap.driverObjs.begin(), cityMap.driverObjs.end(),
                                           [&](auto &d) { return d.id == deleteTargetId; }),
                            cityMap.driverObjs.end());
                        for (size_t j = 0; j < cityMap.driverObjs.size(); ++j) cityMap.driverObjs[j].id = j + 1;
                    } else if (deleteTargetType == "CUSTOMER") {
                        cityMap.customerObjs.erase(
                            std::remove_if(cityMap.customerObjs.begin(), cityMap.customerObjs.end(),
                                           [&](auto &c) { return c.id == deleteTargetId; }),
                            cityMap.customerObjs.end());
                        for (size_t j = 0; j < cityMap.customerObjs.size(); ++j) cityMap.customerObjs[j].id = j + 1;
                    } else if (deleteTargetType == "WAREHOUSE") {
                        cityMap.warehouseObjs.erase(
                            std::remove_if(cityMap.warehouseObjs.begin(), cityMap.warehouseObjs.end(),
                                           [&](auto &w) { return w.id == deleteTargetId; }),
                            cityMap.warehouseObjs.end());
                        for (size_t j = 0; j < cityMap.warehouseObjs.size(); ++j) cityMap.warehouseObjs[j].id = j + 1;
                    }

                    cityMap.SaveCityData("Citymap.txt");
                    cityMap.SaveData(cityMap.customerObjs, cityMap.driverObjs, cityMap.warehouseObjs, "Data.txt");

                    needRedrawTable = true;
                    deleteTargetId = -1;
                }

                // Sau event loop
                if (needRedrawTable) {
                    ShowData(renderer, font, cityMap, cityMap.driverObjs, cityMap.customerObjs, cityMap.warehouseObjs);
                    needRedrawTable = false;
                }


                if((currentPageId == 4 || currentPageId == 5 || currentPageId == 8) && tableMaxScroll > 0) {
                    int handleH = max(40, (int)(scrollBarH * (500.0f / totalHeight)));
                    float ratio = (float)tableScrollY / tableMaxScroll;
                    int handleY = scrollBarY + ratio * (scrollBarH - handleH);

                    SDL_Rect handleRect{ scrollBarX, handleY, scrollBarW, handleH };
                    if(p.x >= handleRect.x && p.x <= handleRect.x + handleRect.w &&
                       p.y >= handleRect.y && p.y <= handleRect.y + handleRect.h)
                    {
                        isDraggingScrollbar = true;
                        dragOffsetY = p.y - handleY;
                    }
                }
            } break;
            case SDL_MOUSEBUTTONUP: {
                if(event.button.button == SDL_BUTTON_LEFT)
                    isDraggingScrollbar = false;
                break;
            }
            case SDL_KEYDOWN:
                if ((currentPageId == 4 || currentPageId == 5 || currentPageId == 8) && tableMaxScroll > 0) {
                    const int step = 20; // số px mỗi lần nhấn
                    if (event.key.keysym.sym == SDLK_UP) {
                        tableScrollY -= step;
                        if (tableScrollY < 0) tableScrollY = 0;
                    } else if (event.key.keysym.sym == SDLK_DOWN) {
                        tableScrollY += step;
                        if (tableScrollY > tableMaxScroll) tableScrollY = tableMaxScroll;
                    }
                }
                break;
            case SDL_MOUSEMOTION: {
                SDL_Pos MousePos;
                SDL_GetMouseState(&MousePos.x, &MousePos.y);

                for (size_t i = 0; i < buttons.size(); i++) {
                    if (buttons[i].pageId != currentPageId) continue;

                    SDL_Cursor* cursor;
                    if (SDLButton_isContain(MousePos, buttons[i])) {
                        cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
                        SDL_SetCursor(cursor);
                        // Không hover nếu nút đang bật
                        if (!((i == 5 && addMode == ADD_CUSTOMER) ||
                              (i == 6 && addMode == ADD_WAREHOUSE) ||
                              (i == 7 && addMode == ADD_DRIVER) ||
                              (i == 8 && edgeMode == EDGE_ON))) {
                            buttons[i].bgColor = {0x00,0xFF,0xFF,0xFF};
                        }
                    } else {
                        cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
                        SDL_SetCursor(cursor);
                        if (!((i == 5 && addMode == ADD_CUSTOMER) ||
                              (i == 6 && addMode == ADD_WAREHOUSE) ||
                              (i == 7 && addMode == ADD_DRIVER) ||
                              (i == 8 && edgeMode == EDGE_ON))) {
                            buttons[i].bgColor = originButtonColor;
                        }
                    }
                }

                // ===== Page 6: ALGORITHM buttons =====
                for (size_t i = 17; i <= 25 && i < buttons.size(); i++) {
                    if (buttons[i].pageId != 6) continue;

                    bool isActive = (i == activeAlgorithm1 || i == activeAlgorithm2);

                    SDL_Cursor* cursor;
                    if (SDLButton_isContain(MousePos, buttons[i])) {
                        cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
                        SDL_SetCursor(cursor);

                        // Chỉ đổi màu hover nếu nút chưa active
                        if (!isActive)
                            buttons[i].bgColor = {0x00, 0xFF, 0xFF, 0xFF}; // màu cyan hover
                    } else {
                        cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
                        SDL_SetCursor(cursor);

                        // Chỉ reset màu nếu nút chưa active
                        if (!isActive)
                            buttons[i].bgColor = originButtonColor;
                    }

                    // Luôn giữ màu hồng nếu active
                    if (isActive)
                        buttons[i].bgColor = {255, 105, 180, 255};
                }

                if (isDraggingScrollbar && (currentPageId == 4 || currentPageId == 5 || currentPageId == 8) && tableMaxScroll > 0) {
                    int handleH = max(40, (int)(scrollBarH * (500.0f / totalHeight)));
                    int newHandleY = event.motion.y - dragOffsetY;

                    if (newHandleY < scrollBarY) newHandleY = scrollBarY;
                    if (newHandleY > scrollBarY + scrollBarH - handleH) newHandleY = scrollBarY + scrollBarH - handleH;

                    float ratio = (float)(newHandleY - scrollBarY) / (scrollBarH - handleH);
                    tableScrollY = ratio * tableMaxScroll;
                }
            } break;
        }
    }
    // ===== Render =====
    if (currentPageId == 3) {
        if (!cityMapGenerated) {
            SDL_SetRenderDrawColor(renderer,0,0,0,255);
            SDL_RenderClear(renderer);
            cityMap.GenerateRandomCityMap(renderer, font, WINDOW_WIDTH, WINDOW_HEIGHT);
            cityMapGenerated = true;
        }

        for (auto &e : cityMap.edges)
            if(e.from>=0 && e.to>=0 && e.from<cityMap.intersections.size() && e.to<cityMap.intersections.size())
                cityMap.DrawRoad(renderer, cityMap.intersections[e.from], cityMap.intersections[e.to]);

        for (auto &p : cityMap.intersections)
            cityMap.DrawIntersection(renderer, p);

        for (auto &d : cityMap.driverObjs)
            SDLHelper::SDL_DrawNode(renderer, {d.GetX(),d.GetY()}, d.GetID(), {0,200,255,255}, false);
        for (auto &c : cityMap.customerObjs)
            SDLHelper::SDL_DrawNode(renderer, {c.GetX(),c.GetY()}, c.GetID(), {255,100,100,255}, false);
        for (auto &w : cityMap.warehouseObjs)
            SDLHelper::SDL_DrawNode(renderer, {w.GetX(),w.GetY()}, w.GetID(), {255,215,0,255}, false);

        for(auto &b: buttons)
            if(b.pageId==currentPageId)
                SDLButton_draw(renderer,b);

        SDL_RenderPresent(renderer);
    } else if (currentPageId == 4) {
        // Luôn load lại dữ liệu từ file, để table luôn cập nhật
        cityMap.LoadSavedData(cityMap.customerObjs, cityMap.driverObjs, cityMap.warehouseObjs, "Data.txt");
        dataLoaded = true;

        ShowData(renderer, font, cityMap, cityMap.driverObjs, cityMap.customerObjs, cityMap.warehouseObjs);

        for(auto &b: buttons)
            if(b.pageId==currentPageId)
                SDLButton_draw(renderer,b);

        SDL_RenderPresent(renderer);
    } else if (currentPageId == 5) {
        cityMap.LoadCityData("Citymap.txt");
        ShowMap(renderer, font, cityMap);

        for(auto &b: buttons)
            if(b.pageId==currentPageId)
                SDLButton_draw(renderer,b);

        SDL_RenderPresent(renderer);
    } else if (currentPageId == 7)
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // ===== RESET =====
        if (needResetLogistics) {
            cityMap.ResetAllDriversAndLogistics();
            logisticsStarted = false;
            needResetLogistics = false;
        }
        if (!logisticsStarted) {
            logisticsStarted = true;

            cityMap.LoadCityData("Citymap.txt");
            cityMap.LoadSavedData(
                cityMap.customerObjs,
                cityMap.driverObjs,
                cityMap.warehouseObjs,
                "Data.txt"
            );

            int mode = activeAlgorithm2 - 22;
            cityMap.RunAllDriversLogistics(renderer, mode, this);
        }
        cityMap.RenderAllMap(renderer, nullptr);

        for (auto &b : buttons)
            if (b.pageId == 7)
                SDLButton_draw(renderer, b);

        SDL_RenderPresent(renderer);
    } else if (currentPageId == 8) {
        SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
        SDL_RenderClear(renderer);

        ShowAlgorithm(renderer);

        for(auto &b: buttons)
            if(b.pageId==currentPageId)
                SDLButton_draw(renderer,b);

        SDL_RenderPresent(renderer);
    }
    else {
        SDLRenderPreset(renderer);
    }
}

