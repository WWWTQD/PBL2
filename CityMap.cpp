#include "CityMap.h"
#include "SDLInterface.h"
#include <iomanip>
#include <fstream>
#include <iostream>
#include <algorithm>
using namespace std;

// ========================================================= MAP ELEMENT =========================================================//

int CityMap::randRange(int min, int max) {
    return min + rand() % (max - min + 1);
}

float CityMap::distanceBetween(const Intersection& a, const Intersection& b) {
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    return sqrt(dx*dx + dy*dy);
}

float CityMap::DistancePoint(float x1, float y1, float x2, float y2) const
{
    int startIdx = GetNearestIntersectionIndex(x1, y1);
    int endIdx   = GetNearestIntersectionIndex(x2, y2);

    if (startIdx == -1 || endIdx == -1) return 1e18f;

    vector<float> dist = DijkstraDistancesFrom(startIdx);
    return dist[endIdx];
}

void CityMap::DrawRoad(SDL_Renderer* renderer, const Intersection& a, const Intersection& b) {
    SDL_SetRenderDrawColor(renderer, 70, 130, 180, 255);
    SDL_RenderDrawLine(renderer, a.x, a.y, b.x, b.y);
}

void CityMap::DrawIntersection(SDL_Renderer* renderer, const Intersection& p) {
    SDL_SetRenderDrawColor(renderer, 60, 60, 60, 255);
    SDLHelper::SDL_RenderFillCircle(renderer, p.x, p.y, 5);
}

void DrawThickLine(SDL_Renderer* renderer, int x1, int y1, int x2, int y2, int thickness = 100)
{
    // Vẽ đường dày bằng cách dịch vuông góc nhiều lần
    float dx = x2 - x1;
    float dy = y2 - y1;

    float len = sqrtf(dx*dx + dy*dy);
    if (len == 0) return;

    // Vector vuông góc để tạo độ dày
    float nx = -(dy / len);
    float ny = dx / len;

    int half = thickness / 2;

    for (int i = -half; i <= half; i++)
    {
        int offX = (int)(nx * i);
        int offY = (int)(ny * i);
        SDL_RenderDrawLine(renderer, x1 + offX, y1 + offY, x2 + offX, y2 + offY);
    }
}

// ========================================================= SAVE & LOAD =========================================================//

void CityMap::SaveCityData(const string& filename) {
    ofstream fout(filename, ios::trunc);
    if (!fout.is_open()) {
        cerr << "Không thể mở file để ghi!";
        return;
    }

    fout << "======= [DRIVERS] =======\n" << driverObjs.size() << "\n";
    for (auto& d : driverObjs)
        fout << d.id << " " << d.x << " " << d.y << "\n";

    fout << "\n======= [CUSTOMERS] =======\n" << customerObjs.size() << "\n";
    for (auto& c : customerObjs)
        fout << c.id << " " << c.x << " " << c.y << "\n";

    fout << "\n======= [WAREHOUSES] =======\n" << warehouseObjs.size() << "\n";
    for (auto& w : warehouseObjs)
        fout << w.id << " " << w.x << " " << w.y << "\n";

    fout << "\n======= [INTERSECTIONS] =======\n" << intersections.size() << "\n";
    for (size_t i = 0; i < intersections.size(); ++i)
        fout << i + 1 << " " << intersections[i].x << " " << intersections[i].y << "\n";

    fout << "\n======= [EDGES] =======\n" << edges.size() << "\n";
    for (auto& e : edges)
        fout << e.id << " " << e.from + 1 << " " << e.to + 1 << " " << e.length << "\n";

    fout.close();
    cout << "✅ Đã lưu dữ liệu bản đồ vào file '" << filename << "'!\n";
}

bool CityMap::LoadCityData(const string& filename) {
    ifstream fin(filename);
    if (!fin.is_open()) {
        cerr << "Không thể mở file '" << filename << "'!\n";
        return false;
    }

    driverObjs.clear();
    customerObjs.clear();
    warehouseObjs.clear();
    intersections.clear();
    edges.clear();

    string line;

    while (getline(fin, line)) {
        if (line.find("[DRIVERS]") != string::npos) {
            fin >> ws;
            int n; fin >> n;
            for (int i = 0; i < n; ++i) {
                Driver d(0, 0, 0);
                fin >> d.id >> d.x >> d.y;
                d.startX = d.x;
                d.startY = d.y;
                driverObjs.push_back(d);

            }
        } else if (line.find("[CUSTOMERS]") != string::npos) {
            fin >> ws;
            int n; fin >> n;
            for (int i = 0; i < n; ++i) {
                Customer c(0, 0, 0);
                fin >> c.id >> c.x >> c.y;
                customerObjs.push_back(c);
            }
        } else if (line.find("[WAREHOUSES]") != string::npos) {
            fin >> ws;
            int n; fin >> n;
            for (int i = 0; i < n; ++i) {
                Warehouse w(0, 0, 0);
                fin >> w.id >> w.x >> w.y;
                warehouseObjs.push_back(w);
            }
        } else if (line.find("[INTERSECTIONS]") != string::npos) {
            fin >> ws;
            int n; fin >> n;
            for (int i = 0; i < n; ++i) {
                Intersection inter;
                fin >> inter.id >> inter.x >> inter.y;
                intersections.push_back(inter);
            }
        } else if (line.find("[EDGES]") != string::npos) {
            fin >> ws;
            int n; fin >> n;
            for (int i = 0; i < n; ++i) {
                Edge e;
                fin >> e.id >> e.from >> e.to >> e.length;
                e.from--; e.to--;
                edges.push_back(e);
            }
        }
    }

    fin.close();
    if (intersections.empty() || edges.empty() || warehouseObjs.empty() || customerObjs.empty() || driverObjs.empty()) {
        cout << "⚠ File tồn tại nhưng không có dữ liệu hợp lệ. Sẽ sinh map mới.\n";
        return false;
    }

    cout << "✅ Đã tải dữ liệu bản đồ từ '" << filename << "'!\n";
    return true;
}


void CityMap::SaveData(const vector<Customer>& customers, const vector<Driver>& drivers, const vector<Warehouse>& warehouses, const string& filename)
{
    ofstream fout(filename, ios::trunc);
    if (!fout.is_open()) return;

    fout << "=== [CUSTOMERS] ===\n\n";
    for (const auto& c : customers)
    {
        fout << "Customer " << c.id << " Warehouses: ";
        if (c.warehouseIds.empty()) fout << "None";
        else for (int wid : c.warehouseIds) fout << wid << " ";
        fout << "\n";
    }
    fout << "\n\n";

    fout << "=== [DRIVERS] ===\n\n";
    for (const auto& d : drivers)
    {
        fout << "Driver " << d.id << " -> ";
        if (d.customerIds.empty())
        {
            fout << "No deliveries\n";
            continue;
        }

        for (size_t i = 0; i < d.customerIds.size(); i++)
        {
            int cid = d.customerIds[i];
            int wid = d.warehouseIds[i]; // tương ứng 1:1 với Customer

            if (i != 0) fout << ", ";
            fout << "Customer " << cid << " (Warehouse " << wid << ")";
        }
        fout << "\n";
    }

    fout << "\n\n";

    fout << "=== [WAREHOUSE ASSIGNMENTS] ===\n\n";

    for (const auto& w : warehouses)
    {
        fout << "Warehouse " << w.id << "\n";

        for (int did : w.driverIds)
        {
            fout << "      Driver " << did << " -> Customers: ";

            const Driver* dptr = nullptr;
            for (const auto& d : drivers)
                if (d.id == did) { dptr = &d; break; }
            if (!dptr)
            {
                fout << "ERROR\n";
                continue;
            }

            bool first = true;
            for (size_t i = 0; i < dptr->customerIds.size(); i++)
            {
                if (dptr->warehouseIds[i] == w.id)
                {
                    if (!first) fout << " ";
                    fout << dptr->customerIds[i];
                    first = false;
                }
            }

            if (first) fout << "None";
            fout << "\n";
        }

        fout << "\n";
    }

    fout.close();
}

bool CityMap::LoadSavedData(vector<Customer>& customers,
                            vector<Driver>& drivers,
                            vector<Warehouse>& warehouses,
                            const string& filename)
{
    ifstream f(filename);
    if (!f.is_open()) return false;

    string line;
    enum Sec { NONE, CUS, DRI, WAR } sec = NONE;

    // Không clear toàn bộ collections ở đây — giả sử customers/drivers/warehouses
    // đã có (vị trí x,y được sinh từ map). Ta chỉ cập nhật các vector id liên quan.
    // current warehouse pointer/index khi đọc phần WAREHOUSE ASSIGNMENTS
    Warehouse* curW = nullptr;

    while (getline(f, line))
    {
        // trim line front/back (đơn giản)
        if (line.empty()) continue;

        if (line.find("[CUSTOMERS]") != string::npos) { sec = CUS; curW = nullptr; continue; }
        if (line.find("[DRIVERS]") != string::npos) { sec = DRI; curW = nullptr; continue; }
        if (line.find("[WAREHOUSE ASSIGNMENTS]") != string::npos) { sec = WAR; curW = nullptr; continue; }

        if (sec == CUS)
        {
            if (line.find("Customer") == string::npos) continue;

            int id;
            string tmp;
            stringstream ss(line);
            ss >> tmp >> id >> tmp; // "Customer <id> Warehouses:"

            // tìm customer trong memory
            auto it = find_if(customers.begin(), customers.end(),
                                   [&](const Customer& c){ return c.id == id; });
            if (it == customers.end()) continue; // bỏ qua nếu không tồn tại

            // cập nhật warehouseIds (xoá trước cho đồng bộ với file)
            it->warehouseIds.clear();
            int wid;
            while (ss >> wid) it->warehouseIds.push_back(wid);
        }
        else if (sec == DRI)
        {
            if (line.find("Driver") == string::npos) continue;

            // có thể là "Driver 1 -> No deliveries" hoặc "Driver 2 -> Customer 1 (Warehouse 1), Customer 4 (Warehouse 1)"
            int id;
            string tmp;
            stringstream ss(line);
            ss >> tmp >> id; // "Driver" id

            // tìm driver
            auto it = find_if(drivers.begin(), drivers.end(),
                                   [&](const Driver& d){ return d.id == id; });
            if (it == drivers.end()) continue;

            // reset rồi parse phần sau '->'
            it->customerIds.clear();
            it->warehouseIds.clear();

            size_t pos = line.find("->");
            if (pos == string::npos) continue;

            string rest = line.substr(pos + 2); // sau ->
            // nếu có "No deliveries" thì bỏ qua
            if (rest.find("No deliveries") != string::npos) continue;

            // phân tách theo dấu ',' vì các mục được ngăn cách bằng comma
            stringstream parts(rest);
            string part;
            while (getline(parts, part, ',')) {
                // phần có dạng " Customer 1 (Warehouse 1)" (có thể có khoảng trắng)
                int cid = 0, wid = 0;
                // dùng sscanf an toàn hơn cho format cố định
                if (sscanf(part.c_str(), " Customer %d (Warehouse %d)", &cid, &wid) == 2 ||
                    sscanf(part.c_str(), "Customer %d (Warehouse %d)", &cid, &wid) == 2) {
                    it->customerIds.push_back(cid);
                    it->warehouseIds.push_back(wid);
                }
                // else: format khác -> bỏ qua
            }
        }
        else if (sec == WAR)
        {
            // nếu gặp "Warehouse <id>" -> đặt curW tới warehouse có id đó
            if (line.find("Warehouse") != string::npos && line.find("Driver") == string::npos) {
                int wid = 0;
                if (sscanf(line.c_str(), "Warehouse %d", &wid) == 1) {
                    auto it = find_if(warehouses.begin(), warehouses.end(),
                                           [&](const Warehouse& w){ return w.id == wid; });
                    if (it != warehouses.end()) {
                        curW = &(*it);
                        // clear driverIds & customerIds (cẩn thận) để đồng bộ với file
                        curW->driverIds.clear();
                        curW->customerIds.clear(); // nếu bạn đang lưu customerIds trong warehouse
                    } else {
                        curW = nullptr; // không tồn tại trong memory -> bỏ qua phần sau
                    }
                } else {
                    curW = nullptr;
                }
            }
            // nếu dòng bắt đầu bằng "      Driver <did> -> Customers: ..." thì gán vào curW
            else if (line.find("Driver") != string::npos || line.find("      Driver") != string::npos) {
                if (!curW) continue; // chưa xác định warehouse hợp lệ -> bỏ qua

                // parse driver id
                int did = 0;
                if (sscanf(line.c_str(), "      Driver %d", &did) != 1) {
                    // thử không có khoảng trắng đầu
                    if (sscanf(line.c_str(), "Driver %d", &did) != 1) continue;
                }

                // thêm driver id vào warehouse hiện tại
                curW->driverIds.push_back(did);

                // tiếp tục parse danh sách customers sau "Customers:"
                size_t pos = line.find("Customers:");
                if (pos != string::npos) {
                    string after = line.substr(pos + strlen("Customers:"));
                    stringstream cs(after);
                    int cid;
                    while (cs >> cid) {
                        curW->customerIds.push_back(cid);
                    }
                }
            }
        }
    }

    f.close();
    return true;
}

bool CityMap::LoadCityData1(const string& filename) {
    ifstream fin(filename, ios::in);
    if (!fin.is_open()) {
        cerr << "Cannot open file: " << filename << "\n";
        return false;
    }

    // Clear dữ liệu cũ
    driverObjs.clear();
    customerObjs.clear();
    warehouseObjs.clear();
    intersections.clear();
    edges.clear();

    // Gợi ý cho vector tránh realloc nhiều lần
    driverObjs.reserve(200);
    customerObjs.reserve(200);
    warehouseObjs.reserve(200);
    intersections.reserve(1000);
    edges.reserve(3000);

    string line;

    // Fast I/O
    ios::sync_with_stdio(false);
    fin.tie(nullptr);

    while (getline(fin, line)) {

        // ==== DRIVERS ====
        if (line == "======= [DRIVERS] =======") {
            int n = 0;
            if (!(fin >> n)) break;
            fin.ignore(numeric_limits<streamsize>::max(), '\n');

            driverObjs.reserve(n);
            for (int i = 0; i < n; ++i) {
                Driver d(0, 0, 0);
                fin >> d.id >> d.x >> d.y;
                driverObjs.emplace_back(d);
            }
            fin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        // ==== CUSTOMERS ====
        else if (line == "======= [CUSTOMERS] =======") {
            int n = 0;
            if (!(fin >> n)) break;
            fin.ignore(numeric_limits<streamsize>::max(), '\n');

            customerObjs.reserve(n);
            for (int i = 0; i < n; ++i) {
                Customer c(0, 0, 0);
                fin >> c.id >> c.x >> c.y;
                customerObjs.emplace_back(c);
            }
            fin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        // ==== WAREHOUSES ====
        else if (line == "======= [WAREHOUSES] =======") {
            int n = 0;
            if (!(fin >> n)) break;
            fin.ignore(numeric_limits<streamsize>::max(), '\n');

            warehouseObjs.reserve(n);
            for (int i = 0; i < n; ++i) {
                Warehouse w(0, 0, 0);
                fin >> w.id >> w.x >> w.y;
                warehouseObjs.emplace_back(w);
            }
            fin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        // ==== INTERSECTIONS ====
        else if (line == "======= [INTERSECTIONS] =======") {
            int n = 0;
            if (!(fin >> n)) break;
            fin.ignore(numeric_limits<streamsize>::max(), '\n');

            intersections.reserve(n);
            for (int i = 0; i < n; ++i) {
                Intersection inter;
                fin >> inter.id >> inter.x >> inter.y;
                intersections.emplace_back(inter);
            }
            fin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        // ==== EDGES ====
        else if (line == "======= [EDGES] =======") {
            int n = 0;
            if (!(fin >> n)) break;
            fin.ignore(numeric_limits<streamsize>::max(), '\n');

            edges.reserve(n);
            for (int i = 0; i < n; ++i) {
                Edge e;
                fin >> e.id >> e.from >> e.to >> e.length;

                // Chuyển về index 0-based an toàn
                if (e.from > 0) e.from--;
                if (e.to > 0) e.to--;

                edges.emplace_back(e);
            }
            fin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    fin.close();

    cout << "Fast Load done: "
              << intersections.size() << " intersections, "
              << edges.size() << " edges\n";
    return true;
}

// ========================================================= MAP =========================================================//

void CityMap::RenderAllMap(SDL_Renderer* renderer, TTF_Font* font)
{
    // Clear screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    for (auto& e : edges) {
        if (e.permanent) continue;

        const auto& a = intersections[e.from];
        const auto& b = intersections[e.to];

        SDL_SetRenderDrawColor(renderer, 70, 130, 180, 255); // map color
        DrawThickLine(renderer, a.x, a.y, b.x, b.y, 2);
    }

    for (auto& e : edges) {
        if (!e.permanent) continue;

        const auto& a = intersections[e.from];
        const auto& b = intersections[e.to];

        // viền đen (rất quan trọng để không bị chìm)
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        DrawThickLine(renderer, a.x, a.y, b.x, b.y, 3);

        // màu driver
        SDL_SetRenderDrawColor(renderer,
            e.color.r, e.color.g, e.color.b, 255);
        DrawThickLine(renderer, a.x, a.y, b.x, b.y, 3);
    }
    for (auto& p : intersections)
        DrawIntersection(renderer, p);

    SDL_Color driverColor    = {0,200,255,255};
    SDL_Color customerColor  = {255,100,100,255};
    SDL_Color warehouseColor = {255,215,0,255};

    for (auto& d : driverObjs) {
        int drawX, drawY;

        if (logisticsRunning || logisticsFinished) {
            drawX = d.x;
            drawY = d.y;
        } else {
            drawX = (int)d.startX;
            drawY = (int)d.startY;
        }

        SDLHelper::SDL_DrawNode(renderer, {drawX, drawY}, d.id, driverColor, false);
    }

    for (auto& c : customerObjs)
        SDLHelper::SDL_DrawNode(renderer, {c.x,c.y}, c.id, customerColor, false);

    for (auto& w : warehouseObjs)
        SDLHelper::SDL_DrawNode(renderer, {w.x,w.y}, w.id, warehouseColor, false);

}

void CityMap::GenerateRandomCityMap(SDL_Renderer* renderer, TTF_Font* font, int screenW, int screenH) {
    srand((unsigned)time(nullptr));

    if (LoadCityData("Citymap.txt")) {
        cout << "Dang ve lai ban do tu file\n";

        RenderAllMap(renderer, font);
        return;
    }


    const int NUM_INTERSECTIONS = 200;
    const float MIN_DISTANCE = min(screenW, screenH)/18.0f;
    const int MAX_ATTEMPTS = 10000;

    intersections.clear();
    edges.clear();
    int edgeCounter = 1;

    for (int i = 0; i < NUM_INTERSECTIONS; i++) {
        Intersection p; p.id=i+1;
        bool valid=false; int attempts=0;
        while(!valid && attempts<MAX_ATTEMPTS){
            p.x=randRange(50,screenW-80);
            p.y=randRange(70,screenH-80);
            valid=true;
            for(auto& other: intersections)
                if(distanceBetween(p,other)<MIN_DISTANCE){ valid=false; break;}
            attempts++;
        }
        if(valid) intersections.push_back(p);
        else break;
    }

    for(int i=0;i<(int)intersections.size();i++){
        vector<pair<float,int>> dist;
        for(int j=0;j<(int)intersections.size();j++){
            if(i==j) continue;
            dist.push_back({distanceBetween(intersections[i],intersections[j]),j});
        }
        sort(dist.begin(),dist.end());
        int numLinks = randRange(2,3);
        for(int k=0;k<numLinks && k<(int)dist.size();k++){
            int to=dist[k].second;
            float len=dist[k].first;
            DrawRoad(renderer, intersections[i], intersections[to]);
            edges.push_back({edgeCounter++,i,to,len});
        }
    }


    for(auto& p: intersections) DrawIntersection(renderer,p);
    SDL_RenderPresent(renderer);

    SDL_Color driverColor = {0,200,255,255};
    SDL_Color customerColor = {255,100,100,255};
    SDL_Color warehouseColor = {255,215,0,255};

    driverObjs.clear(); customerObjs.clear(); warehouseObjs.clear();

    const float MIN_BIGNODE_DIST=180.0f;
    auto isTooClose=[&](int x,int y){
        for(auto& d: driverObjs) if(distanceBetween({0,d.GetX(),d.GetY()},{0,x,y})<MIN_BIGNODE_DIST) return true;
        for(auto& c: customerObjs) if(distanceBetween({0,c.GetX(),c.GetY()},{0,x,y})<MIN_BIGNODE_DIST) return true;
        for(auto& w: warehouseObjs) if(distanceBetween({0,w.GetX(),w.GetY()},{0,x,y})<MIN_BIGNODE_DIST) return true;
        return false;
    };

    auto spawnNodes=[&](auto& vec,int count,SDL_Color color){
        for(int i=0;i<count;i++){
            int idx;
            do{ idx=randRange(0,intersections.size()-1);}
            while(isTooClose(intersections[idx].x,intersections[idx].y));
            vec.emplace_back(i+1,intersections[idx].x,intersections[idx].y);
            SDLHelper::SDL_DrawNode(renderer,{intersections[idx].x,intersections[idx].y},i+1,color,false);
        }
    };

    spawnNodes(driverObjs,4,driverColor);
    spawnNodes(customerObjs,4,customerColor);
    spawnNodes(warehouseObjs,4,warehouseColor);

    SaveCityData("Citymap.txt");
    SDL_RenderPresent(renderer);
}

// ========================================================= ADD MAP ELEMENT =========================================================//

void CityMap::AddIntersectionWithEdges(SDL_Renderer* renderer,int clickX,int clickY){
    Intersection newPoint;
    newPoint.id=intersections.size()+1;
    newPoint.x=clickX; newPoint.y=clickY;
    intersections.push_back(newPoint);

    int newIndex = intersections.size()-1;
    vector<int> candidates;
    for(int i=0;i<newIndex;i++)
        if(distanceBetween(newPoint,intersections[i])<=100.0f)
            candidates.push_back(i);

    int numLinks = min((int)candidates.size(),1+rand()%4);
    MySelf::shuffle(candidates);
    for(int i=0;i<numLinks;i++){
        int to=candidates[i];
        float len=distanceBetween(newPoint,intersections[to]);
        edges.push_back({(int)edges.size()+1,newIndex,to,len});
        DrawRoad(renderer,newPoint,intersections[to]);
    }

    DrawIntersection(renderer,newPoint);
    SDL_RenderPresent(renderer);

    SaveCityData("Citymap.txt");
}

void CityMap::AddNodeOnly(SDL_Renderer* renderer, SDL_Pos clickPos, AddMode addMode){
    const float NODE_RADIUS=25.0f;
    int nearestIdx=-1;
    float minD=1e9;
    for(int i=0;i<intersections.size();i++){
        float d=distanceBetween(intersections[i],{0,clickPos.x,clickPos.y});
        if(d<minD){ minD=d; nearestIdx=i; }
    }
    if(nearestIdx==-1 || minD>NODE_RADIUS){ cout<<"Khong co intersection nao du gan!\n"; return;}
    int newId=1;
    if(addMode==ADD_CUSTOMER) for(auto& c: customerObjs) newId=max(newId,c.GetID()+1);
    else if(addMode==ADD_WAREHOUSE) for(auto& w: warehouseObjs) newId=max(newId,w.GetID()+1);
    else if(addMode==ADD_DRIVER) for(auto& d: driverObjs) newId=max(newId,d.GetID()+1);

    auto& P = intersections[nearestIdx];
    if(addMode==ADD_CUSTOMER) customerObjs.push_back(Customer(newId,P.x,P.y));
    else if(addMode==ADD_WAREHOUSE) warehouseObjs.push_back(Warehouse(newId,P.x,P.y));
    else if(addMode==ADD_DRIVER) driverObjs.push_back(Driver(newId,P.x,P.y));

    SDL_Color color;
    if(addMode==ADD_CUSTOMER) color={255,100,100,255};
    if(addMode==ADD_WAREHOUSE) color={255,215,0,255};
    if(addMode==ADD_DRIVER) color={0,200,255,255};

    SDLHelper::SDL_DrawNode(renderer,{P.x,P.y},newId,color,false);
    SDL_RenderPresent(renderer);

    SaveCityData("Citymap.txt");
}

void CityMap::AddNodeWithEdges(SDL_Renderer* renderer, SDL_Pos clickPos, AddMode addMode){
    int newId=1;
    if(addMode==ADD_CUSTOMER) for(auto& c: customerObjs) newId=max(newId,c.GetID()+1);
    else if(addMode==ADD_WAREHOUSE) for(auto& w: warehouseObjs) newId=max(newId,w.GetID()+1);
    else if(addMode==ADD_DRIVER) for(auto& d: driverObjs) newId=max(newId,d.GetID()+1);

    AddIntersectionWithEdges(renderer,clickPos.x,clickPos.y);

    if(addMode==ADD_CUSTOMER) customerObjs.push_back(Customer(newId,clickPos.x,clickPos.y));
    else if(addMode==ADD_WAREHOUSE) warehouseObjs.push_back(Warehouse(newId,clickPos.x,clickPos.y));
    else if(addMode==ADD_DRIVER) driverObjs.push_back(Driver(newId,clickPos.x,clickPos.y));

    SDL_Color color;
    if(addMode==ADD_CUSTOMER) color={255,100,100,255};
    if(addMode==ADD_WAREHOUSE) color={255,215,0,255};
    if(addMode==ADD_DRIVER) color={0,200,255,255};

    SDLHelper::SDL_DrawNode(renderer,clickPos,newId,color,false);
    SDL_RenderPresent(renderer);

    SaveCityData("Citymap.txt");
}

void CityMap::HandleLeftClick(SDL_Renderer* renderer, SDL_Pos clickPos, AddMode addMode, EdgeMode edgeMode){
    if(addMode==ADD_NONE && edgeMode==EDGE_ON){
        AddIntersectionWithEdges(renderer,clickPos.x,clickPos.y);
        return;
    }
    if(addMode==ADD_NONE) {
        return;
    }
    if(edgeMode==EDGE_ON) {
        AddNodeWithEdges(renderer,clickPos,addMode);
        return;
    }
    AddNodeOnly(renderer,clickPos,addMode);
}

// ========================================================= ALGORITHM DIVIDE (WAREHOUSE) =========================================================//

// Dijkstra trả về khoảng cách từ một intersection đến tất cả intersections
vector<float> CityMap::DijkstraDistances(int startInter) const {
    int n = (int)intersections.size();
    vector<float> dist(n, 1e18f);
    vector<char> visited(n, 0);

    using P = pair<float,int>;   // (dist, node)
    vector<P> heap;              // heap tự tạo

    dist[startInter] = 0.0f;
    MySelf::heapPush(heap, P(0.0f, startInter));

    while (!MySelf::heapEmpty(heap)) {
        P cur = MySelf::heapTop(heap);
        MySelf::heapPop(heap);

        float d = cur.first;
        int u = cur.second;

        if (visited[u]) continue;
        visited[u] = 1;

        for (size_t i = 0; i < edges.size(); ++i) {
            int v = -1;
            if (edges[i].from == u) v = edges[i].to;
            else if (edges[i].to == u) v = edges[i].from;

            if (v != -1 && dist[u] + edges[i].length < dist[v]) {
                dist[v] = dist[u] + edges[i].length;
                MySelf::heapPush(heap, P(dist[v], v));
            }
        }
    }
    return dist;
}


// TSP đơn giản bằng heuristic nearest neighbor
TSPResult CityMap::SolveTSP(const vector<int>& custInterIds, int startInter) const {
    vector<int> remaining = custInterIds;
    vector<int> route;
    float totalDist = 0;

    int current = startInter;
    while(!remaining.empty()) {
        auto distArr = DijkstraDistances(current);

        // tìm customer gần nhất
        int bestIdx = 0;
        float bestD = distArr[remaining[0]];
        for (size_t i = 1; i < remaining.size(); i++)
            if (distArr[remaining[i]] < bestD) { bestD = distArr[remaining[i]]; bestIdx = i; }

        current = remaining[bestIdx];
        route.push_back(current);
        totalDist += bestD;
        remaining.erase(remaining.begin() + bestIdx);
    }

    return {route, totalDist};
}

//============================================ HIGHLIGHT (REPLACE THIS BLOCK) + ALGORITHM MOVE ==========================================//

SDL_Color CityMap::GetDriverColor(int driverIndex) {
    static const vector<SDL_Color> palette = {
        {255, 0, 0, 255},     {0, 255, 0, 255},     {0, 0, 255, 255},
        {255, 255, 0, 255},   {0, 255, 255, 255},   {255, 0, 255, 255},
        {255, 165, 0, 255},   {128, 0, 128, 255},   {255, 192, 203, 255},
        {0, 128, 128, 255},   {128, 128, 0, 255},   {0, 0, 128, 255},
        {128, 0, 0, 255},     {0, 128, 0, 255},     {128, 128, 128, 255},
        {192, 192, 192, 255}, {255, 99, 71, 255},   {75, 0, 130, 255},
        {240, 230, 140, 255}, {173, 216, 230, 255}, {32, 178, 170, 255},
        {255, 20, 147, 255},  {218, 112, 214, 255}, {210, 105, 30, 255},
        {154, 205, 50, 255}
    };
    return palette[driverIndex % palette.size()];
}

int CityMap::GetNearestIntersectionIndex(float x, float y) const {
    int bestIdx = -1;
    float minD = numeric_limits<float>::infinity();
    for (size_t i = 0; i < intersections.size(); ++i) {
        float dx = intersections[i].x - x;
        float dy = intersections[i].y - y;
        float d = sqrtf(dx*dx + dy*dy);
        if (d < minD) { minD = d; bestIdx = (int)i; }
    }
    return bestIdx;
}

vector<int> CityMap::FindAndColorPath(SDL_Renderer* renderer,
                                     int startNodeIdx,
                                     int endNodeIdx,
                                     SDL_Color pathColor)
{
    vector<int> pathNodes;
    if (startNodeIdx < 0 || endNodeIdx < 0) return pathNodes;
    if (startNodeIdx == endNodeIdx) {
        pathNodes.push_back(startNodeIdx);
        return pathNodes;
    }

    int n = (int)intersections.size();
    vector<float> dist(n, numeric_limits<float>::infinity());
    vector<int> parent(n, -1);
    vector<int> edgeToParent(n, -1);

    // ---------- Build adjacency ----------
    vector<vector<pair<int,int>>> adj(n);
    for (size_t i = 0; i < edges.size(); ++i) {
        int u = edges[i].from;
        int v = edges[i].to;
        if (u >= 0 && u < n && v >= 0 && v < n) {
            adj[u].push_back(pair<int,int>(v, (int)i));
            adj[v].push_back(pair<int,int>(u, (int)i));
        }
    }

    using P = pair<float,int>;
    vector<P> heap;                   // heap tự tạo

    dist[startNodeIdx] = 0.0f;
    MySelf::heapPush(heap, P(0.0f, startNodeIdx));

    // ---------- Dijkstra ----------
    while (!MySelf::heapEmpty(heap)) {
        P cur = MySelf::heapTop(heap);
        MySelf::heapPop(heap);

        float d = cur.first;
        int u = cur.second;

        if (d > dist[u]) continue;
        if (u == endNodeIdx) break;

        for (size_t i = 0; i < adj[u].size(); ++i) {
            int v = adj[u][i].first;
            int eIdx = adj[u][i].second;
            float w = edges[eIdx].length;

            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                parent[v] = u;
                edgeToParent[v] = eIdx;
                MySelf::heapPush(heap, P(dist[v], v));
            }
        }
    }

    // ---------- No path ----------
    if (!isfinite(dist[endNodeIdx])) return pathNodes;

    // ---------- Reconstruct + color ----------
    int cur = endNodeIdx;
    while (cur != -1) {
        pathNodes.push_back(cur);
        int eIdx = edgeToParent[cur];
        if (eIdx != -1) {
            edges[eIdx].color = pathColor;
            edges[eIdx].permanent = true;
        }
        cur = parent[cur];
    }

    MySelf::reverse(pathNodes);
    RenderAllMap(renderer, nullptr);
    return pathNodes;
}


vector<float> CityMap::DijkstraDistancesFrom(int startIdx) const {
    int n = (int)intersections.size();
    vector<float> dist(n, numeric_limits<float>::infinity());
    vector<char> visited(n, 0);

    using P = pair<float,int>;
    vector<P> pq;                      // heap tự tạo

    dist[startIdx] = 0.0f;
    MySelf::heapPush(pq, P(0.0f, startIdx));

    // Build adjacency once
    vector<vector<pair<int,float>>> adj(n);
    for (auto &e : edges) {
        if (e.from >= 0 && e.from < n)
            adj[e.from].push_back({e.to, e.length});
        if (e.to >= 0 && e.to < n)
            adj[e.to].push_back({e.from, e.length});
    }

    while (!MySelf::heapEmpty(pq)) {
        P cur = MySelf::heapTop(pq);
        MySelf::heapPop(pq);

        float d = cur.first;
        int u = cur.second;

        if (visited[u]) continue;
        visited[u] = 1;

        for (auto &pr : adj[u]) {
            int v = pr.first;
            float w = pr.second;
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                MySelf::heapPush(pq, P(dist[v], v));
            }
        }
    }

    return dist;
}


vector<int> CityMap::NearestNeighborTSP_OnIntersections(const vector<int>& targetInterIdxs, int startIdx) const {
    vector<int> order;
    if (targetInterIdxs.empty()) return order;

    // copy targets
    vector<int> remaining = targetInterIdxs;
    int current = startIdx;

    // We'll compute Dijkstra distances on the fly from current
    while(!remaining.empty()) {
        vector<float> dist = DijkstraDistancesFrom(current);
        // find nearest remaining (by graph distance)
        int bestPos = 0;
        float bestD = numeric_limits<float>::infinity();
        for (size_t i=0;i<remaining.size();++i) {
            int inter = remaining[i];
            if (dist[inter] < bestD) { bestD = dist[inter]; bestPos = (int)i; }
        }
        // append chosen
        int chosen = remaining[bestPos];
        order.push_back(chosen);
        current = chosen;
        remaining.erase(remaining.begin() + bestPos);
    }
    return order; // intersections order
}

int CityMap::CustomerToNearestInterIdx(int customerId) const {
    for (const auto &c : customerObjs) {
        if (c.id == customerId) return GetNearestIntersectionIndex(c.x, c.y);
    }
    return -1;
}

int CityMap::WarehouseToNearestInterIdx(int warehouseId) const {
    for (const auto &w : warehouseObjs) {
        if (w.id == warehouseId) return GetNearestIntersectionIndex(w.x, w.y);
    }
    return -1;
}

template<typename T>
void erase_duplicates_keep_order(vector<T>& v) {
    unordered_set<T> seen;
    vector<T> out; out.reserve(v.size());
    for (const auto &x : v) if (seen.insert(x).second) out.push_back(x);
    v.swap(out);
}

float CityMap::DistanceIntersections(int a, int b) const
{
    if (a < 0 || b < 0 ||
        a >= (int)intersections.size() ||
        b >= (int)intersections.size())
        return 1e18f;

    float dx = intersections[a].x - intersections[b].x;
    float dy = intersections[a].y - intersections[b].y;
    return sqrtf(dx*dx + dy*dy);
}

bool CityMap::IsWarehouseIntersection(int interIdx)
{
    for (auto &w : warehouseObjs) {
        if (GetNearestIntersectionIndex(w.x, w.y) == interIdx)
            return true;
    }
    return false;
}

bool CityMap::IsCustomerIntersection(int interIdx)
{
    for (auto &c : customerObjs) {
        if (GetNearestIntersectionIndex(c.x, c.y) == interIdx)
            return true;
    }
    return false;
}

void CityMap::ResetAllDriversAndLogistics() {

    driverRoutes.clear();
    driverRoutes.resize(driverObjs.size());

    for (auto &drv : driverObjs) {
        drv.x = (int)drv.startX;
        drv.y = (int)drv.startY;
    }

    for (auto &e : edges) {
        e.color = {70, 130, 180, 255};
        e.permanent = false;
    }

    logisticsRunning  = false;
    logisticsPaused   = false;
    logisticsFinished = false;
}

void CityMap::RunAllDriversLogistics(SDL_Renderer* renderer, int mode, SDLInterface* ui)
{
    static bool algorithmSaved = false;
    algorithmSaved = false;
    SDLHelper::InitClockFont("C:/Windows/Fonts/arial.ttf", 18);
    if (mode < 1 || mode > 3) mode = 0;

    cout << "\nRUN LOGISTICS (ORDER-BASED, FINAL) MODE " << mode << "\n";

    // ================= LOOKUP =================
    unordered_map<int, Customer*> custById;
    unordered_map<int, Warehouse*> whById;
    for (auto &c : customerObjs) custById[c.id] = &c;
    for (auto &w : warehouseObjs) whById[w.id] = &w;

    // reset edges
    for (auto &e : edges) {
        e.color = {70,130,180,255};
        e.permanent = false;
    }
    RenderAllMap(renderer, nullptr);

    driverRoutes.clear();
    driverRoutes.resize(driverObjs.size());

    // =============== BUILD ROUTE FOR EACH DRIVER =============
    for (size_t d = 0; d < driverObjs.size(); ++d)
    {
        Driver &drv = driverObjs[d];
        DriverRoute &route = driverRoutes[d];
        SDL_Color color = GetDriverColor((int)d);

        // ---------- BUILD ORDERS (ORDER-BASED) ----------
        struct Order {
            int wid;
            int cid;
            bool delivered = false;
        };
        vector<Order> orders;

        for (size_t i = 0; i < drv.customerIds.size(); ++i) {
            orders.push_back({ drv.warehouseIds[i],
                               drv.customerIds[i],
                               false });
        }

        if (orders.empty()) {
            route.finished = true;
            continue;
        }
        route.orderWid.clear();
        route.orderCid.clear();

        for (auto &o : orders) {
            route.orderWid.push_back(o.wid);
            route.orderCid.push_back(o.cid);
        }

        route.picked.assign(orders.size(), false);
        route.done.assign(orders.size(), false);


        // ---------- STATE ----------
        unordered_set<int> visitedWarehouse;
        int curInter = GetNearestIntersectionIndex(drv.x, drv.y);

        route.pathNodeIndices.clear();
        route.pathNodeIndices.push_back(curInter);

        cout << "Driver " << drv.id << ": ";

        // ================== MAIN LOOP ========================
        while (true)
        {
            bool allDone = true;
            for (auto &o : orders)
                if (!o.delivered) { allDone = false; break; }
            if (allDone) break;

            // ---------- BUILD FEASIBLE STOPS ----------
            struct Stop {
                int type;   // 0 = warehouse, 1 = customer
                int orderIdx;
                int inter;
                float dist;
            };
            vector<Stop> candidates;

            // ---- warehouses ----
            for (size_t i = 0; i < orders.size(); ++i)
            {
                if (visitedWarehouse.count(orders[i].wid)) continue;

                int wi = GetNearestIntersectionIndex(
                    whById[orders[i].wid]->x,
                    whById[orders[i].wid]->y
                );

                float d = hypot(intersections[wi].x - intersections[curInter].x,
                                intersections[wi].y - intersections[curInter].y);

                candidates.push_back({0, (int)i, wi, d});
            }

            // ---- customers (ONLY if warehouse visited) ----
            for (size_t i = 0; i < orders.size(); ++i)
            {
                if (orders[i].delivered) continue;
                if (!visitedWarehouse.count(orders[i].wid)) continue;

                int ci = GetNearestIntersectionIndex(
                    custById[orders[i].cid]->x,
                    custById[orders[i].cid]->y
                );

                float d = hypot(intersections[ci].x - intersections[curInter].x,
                                intersections[ci].y - intersections[curInter].y);

                candidates.push_back({1, (int)i, ci, d});
            }

            if (candidates.empty()) {
                cout << "DEADLOCK\n";
                break;
            }

            // ================ SELECT NEXT ====================
            Stop next = candidates[0];

            if (mode == 1) {
                // Nearest Neighbor
                for (auto &s : candidates)
                    if (s.dist < next.dist) next = s;
            }
            else if (mode == 2) {
                // Prefer CUSTOMER if possible
                bool found = false;
                for (auto &s : candidates)
                    if (s.type == 1 && (!found || s.dist < next.dist)) {
                        next = s;
                        found = true;
                    }
                if (!found)
                    for (auto &s : candidates)
                        if (s.dist < next.dist) next = s;
            }
            else if (mode == 3) {
                // Prefer WAREHOUSE if exists
                bool found = false;
                for (auto &s : candidates)
                    if (s.type == 0 && (!found || s.dist < next.dist)) {
                        next = s;
                        found = true;
                    }
                if (!found)
                    for (auto &s : candidates)
                        if (s.dist < next.dist) next = s;
            }

            // =================== MOVE ========================
            // 🔐 ĐÁNH DẤU NODE CẦN DỪNG (NHẬN / GIAO HÀNG)
            route.serviceNodes.insert(next.inter);
            vector<int> path = FindAndColorPath(
                renderer, curInter, next.inter, color
            );

            if (path.size() > 1)
                route.pathNodeIndices.insert(
                    route.pathNodeIndices.end(),
                    path.begin() + 1, path.end()
                );

            curInter = next.inter;

            if (next.type == 0) {
                visitedWarehouse.insert(orders[next.orderIdx].wid);
                cout << "[W" << orders[next.orderIdx].wid << "] ";
            } else {
                orders[next.orderIdx].delivered = true;
                cout << "[C" << orders[next.orderIdx].cid << "] ";
            }
        }

        cout << "(done)\n";
    }

    // =================== ANIMATION LOOP ==================
    for (auto &r : driverRoutes) {
        r.currentNodeIdx = 0;
        r.progress = 0.0f;
        r.finished = false;
        r.waiting = false;
        r.distanceTraveled = 0.0f; // reset distance
    }

    // 1️⃣ Trước khi vòng animation, lưu vị trí gốc:
    vector<SDL_Point> driverOriginalPos(driverObjs.size());
    for (size_t i = 0; i < driverObjs.size(); ++i)
    {
        driverOriginalPos[i] = { driverObjs[i].x, driverObjs[i].y };
    }
    logisticsRunning  = true;
    logisticsPaused   = false;
    logisticsFinished = false;

    const float SPEED = 0.3f;
    bool moving = true;

    // --- CLOCK ---
    clockStartTicks = SDL_GetTicks();
    clockStopTicks  = 0;
    logisticsFinished = false;
    Uint32 pausedTicks = 0;
    Uint32 pauseStartTicks = 0;

    // Thêm biến để giữ màu gốc của nút (để reset khi không hover)
    SDL_Color originalButtonColor = ui->buttons[31].bgColor; // Lấy màu của nút Back (hoặc bất kỳ nút nào ở trang 7)

    while (logisticsRunning)
    {
        moving = false; // Đặt lại moving (logic này sẽ được Driver reset thành true nếu còn chạy)
        if (logisticsPaused)
            moving = true; // giữ loop sống khi pause

        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            if (ui && e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT)
            {
                SDL_Pos p = { e.button.x, e.button.y };
                // BACK
                if (ui->buttons.size() > 31 && SDLInterface::SDLButton_isContain(p, ui->buttons[31]))
                {
                    logisticsRunning = false;
                    logisticsPaused  = false;
                    ResetAllDriversAndLogistics();
                    ui->logisticsStarted = false;
                    ui->needResetLogistics = false;
                    ui->currentPageId = 6;
                    return;
                }
                // RESET
                if (ui->buttons.size() > 30 && SDLInterface::SDLButton_isContain(p, ui->buttons[30]))
                {
                    logisticsRunning = false;
                    ui->needResetLogistics = true;
                    return;
                }
                // PAUSE
                if (ui->buttons.size() > 32 && SDLInterface::SDLButton_isContain(p, ui->buttons[32]))
                {
                    logisticsPaused = true;
                    pauseStartTicks = SDL_GetTicks();
                    wasPaused = true;
                }
                // RESUME
                if (ui->buttons.size() > 33 && SDLInterface::SDLButton_isContain(p, ui->buttons[33]))
                {
                    logisticsPaused = false;

                    if (wasPaused) {
                        pausedTicks += SDL_GetTicks() - pauseStartTicks;
                        wasPaused = false;
                    }
                }
            }
            if (ui && e.type == SDL_MOUSEMOTION)
            {
                SDL_Pos p = { e.motion.x, e.motion.y };

                for (size_t i = 0; i < ui->buttons.size(); i++) {
                    if (ui->buttons[i].pageId != 7) continue;

                    if (SDLInterface::SDLButton_isContain(p, ui->buttons[i])) {
                        ui->buttons[i].bgColor = {0x00, 0xFF, 0xFF, 0xFF};
                        SDL_Cursor* cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
                        SDL_SetCursor(cursor);
                    } else {
                        ui->buttons[i].bgColor = originalButtonColor;
                        SDL_Cursor* cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
                        SDL_SetCursor(cursor);
                    }
                }
            }
        }

        bool allowMove = !logisticsPaused;

        // LOGIC DI CHUYỂN
        if (allowMove) {
            for (size_t i = 0; i < driverObjs.size(); ++i)
            {
                DriverRoute &r = driverRoutes[i];

                if (r.finished || r.currentNodeIdx >= (int)r.pathNodeIndices.size() - 1)
                {
                    r.finished = true;
                    continue;
                }

                if (r.waiting) {
                    if (SDL_GetTicks() - r.waitStartTicks < 1500) {
                        moving = true;
                        continue;
                    } else {
                        r.waiting = false;
                    }
                }

                moving = true;
                r.progress += SPEED;

                int a = r.pathNodeIndices[r.currentNodeIdx];
                int b = r.pathNodeIndices[r.currentNodeIdx + 1];

                float x1 = intersections[a].x;
                float y1 = intersections[a].y;
                float x2 = intersections[b].x;
                float y2 = intersections[b].y;

                // --- CỘNG DISTANCE ---
                float segDist = hypot(x2 - x1, y2 - y1);
                r.distanceTraveled += segDist * SPEED;

                if (r.progress >= 1.0f) {
                    r.progress = 0.0f;
                    r.currentNodeIdx++;
                    driverObjs[i].x = (int)x2;
                    driverObjs[i].y = (int)y2;

                    int arrivedNode = r.pathNodeIndices[r.currentNodeIdx];

                    bool shouldWait = false;

                    if (r.serviceNodes.count(arrivedNode))
                    {
                        for (size_t k = 0; k < r.orderWid.size(); ++k)
                        {
                            if (r.done[k]) continue;

                            int wNode = GetNearestIntersectionIndex(
                                whById[r.orderWid[k]]->x,
                                whById[r.orderWid[k]]->y
                            );

                            int cNode = GetNearestIntersectionIndex(
                                custById[r.orderCid[k]]->x,
                                custById[r.orderCid[k]]->y
                            );

                            // 🚚 PICK
                            if (!r.picked[k] && arrivedNode == wNode)
                            {
                                r.picked[k] = true;
                                shouldWait = true;
                            }

                            // 📦 DROP (CHỈ DROP ĐƠN NÀY)
                            if (r.picked[k] && !r.done[k] && arrivedNode == cNode)
                            {
                                r.done[k] = true;
                                shouldWait = true;
                            }
                        }
                    }


                    if (shouldWait)
                    {
                        r.waiting = true;
                        r.waitStartTicks = SDL_GetTicks();
                    }


                }
                else {
                    driverObjs[i].x = (int)(x1 + (x2 - x1) * r.progress);
                    driverObjs[i].y = (int)(y1 + (y2 - y1) * r.progress);
                }
            }
        }

        // STOP CLOCK WHEN ALL DRIVERS FINISHED
        bool allFinished = true;
        for (auto &r : driverRoutes) {
            if (!r.finished) {
                allFinished = false;
                break;
            }
        }

        // chỉ chạy 1 lần
        if (allFinished && !logisticsFinished) {
            logisticsFinished = true;
            clockStopTicks = SDL_GetTicks();
        }

        // [SAVE RESULT TO FILE - CHỈ CHẠY 1 LẦN]
        if (allFinished && logisticsFinished && !algorithmSaved)
        {
            algorithmSaved = true;   // KHÓA NGAY

            Uint32 elapsedMs = clockStopTicks - clockStartTicks - pausedTicks;

            int min  = elapsedMs / 60000;
            int sec  = (elapsedMs % 60000) / 1000;
            int cent = (elapsedMs % 1000) / 10;

            float idealRatio = 1.0f / driverRoutes.size();
            float variance = 0.0f;
            float totalDistance = 0.0f;

            // 1. Tổng quãng đường
            for (auto &r : driverRoutes)
                totalDistance += r.distanceTraveled;

            // 2. Variance
            for (auto &r : driverRoutes)
            {
                float actualRatio = r.distanceTraveled / totalDistance;
                float diff = actualRatio - idealRatio;
                variance += diff * diff;
            }
            variance /= driverRoutes.size();

            // 3. StdDev
            float stddev = sqrt(variance);

            // 4. BALANCE
            float balance = (idealRatio > 1e-6f)
                ? (1.0f / (1.0f + stddev / idealRatio))
                : 1.0f;

            ofstream fout("Algorithm.txt", ios::app);
            if (fout.is_open()) {
                string modeName;
                if (mode == 1 || mode == 4)
                    modeName = "Nearest Neighbor";
                else if (mode == 2 || mode == 5)
                    modeName = "Prefer Customer";
                else if (mode == 3 || mode == 6)
                    modeName = "Prefer Warehouse";
                else
                    modeName = "Unknown Mode";
                fout << selectedAssignName
                     << " | " << modeName
                     << " | "
                     << setw(2) << setfill('0') << min << ":"
                     << setw(2) << sec << "."
                     << setw(2) << cent
                     << " | " << fixed << setprecision(1) << totalDistance
                     << " | balance=" << fixed << setprecision(2) << balance
                     << "\n";
                fout.close();
            }
        }

        // TÍNH THỜI GIAN & TỔNG DISTANCE
        Uint32 elapsedMs;

        if (logisticsFinished) {
            elapsedMs = clockStopTicks - clockStartTicks - pausedTicks;
        }
        else if (logisticsPaused) {
            elapsedMs = pauseStartTicks - clockStartTicks - pausedTicks;
        }
        else {
            elapsedMs = SDL_GetTicks() - clockStartTicks - pausedTicks;
        }

        float totalDistance = 0.0f;
        for (auto &r : driverRoutes)
            totalDistance += r.distanceTraveled;

        // VẼ LẠI MÀN HÌNH
        RenderAllMap(renderer, nullptr);

        // Vẽ các nút từ UI (Page 7)
        if (ui) {
            for (auto &b : ui->buttons)
                if (b.pageId == 7)
                    SDLInterface::SDLButton_draw(renderer, b);
        }

        // --- VẼ ĐỒNG HỒ + TỔNG DISTANCE ---
        SDL_Rect clockRect;
        clockRect.x = 10;
        clockRect.y = 660;
        SDLHelper::RenderClockAndTotalDistance(renderer, elapsedMs, totalDistance, {255,255,255,255}, clockRect);

        // --- VẼ DISTANCE TỪNG DRIVER ---
        for (size_t i = 0; i < driverObjs.size(); ++i)
        {
            SDLHelper::RenderDriverDistance(
                renderer,
                driverRoutes[i].distanceTraveled,
                driverObjs[i].x - 17,
                driverObjs[i].y - 40,
                {255,255,255,255}
            );
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }
}
