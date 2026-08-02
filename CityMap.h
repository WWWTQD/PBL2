#pragma once
#include "Driver.h"
#include "Customer.h"
#include "Warehouse.h"
#include "SDLHelper.h"
#include "unordered_set"
#include <iostream>

using namespace std;

struct Edge {
    int id;
    int from;
    int to;
    double length;

    // trạng thái render
    SDL_Color color = {70, 130, 180, 255}; // mặc định
    bool permanent = false; // nếu true giữ màu (đã được chọn vào path)
};

struct Intersection {
    int id;
    int x, y;
};

struct Cluster {
    vector<int> driverIds;
    vector<int> customerIds;
    int centerX, centerY;
};

struct TSPResult {
    vector<int> order;  // thứ tự customerId
    float totalDist;
};

enum AddMode {
    ADD_NONE,
    ADD_CUSTOMER,
    ADD_WAREHOUSE,
    ADD_DRIVER
};

enum EdgeMode {
    EDGE_NONE,
    EDGE_ON
};

enum ALGORITHM1 {
    ALGORITHM1_NONE,
    ALGORITHM1_RANDOM,
    ALGORITHM1_GREEDY,
    ALGORITHM1_HUNGARIAN,
    ALGORITHM1_KMEANS,
    ALGORITHM1_CLUSTERTSP,
    ALGORITHM1_HUNGARIANKMEANS
};

enum ALGORITHM2 {
    ALGORITHM2_NONE,
    ALGORITHM2_NEARESTNEIGHBOR,
    ALGORITHM2_PREFERCUSTOMER,
    ALGORITHM2_PREFERWAREHOUSE,
};

struct DriverRoute {
    vector<int> pathNodeIndices;
    int currentNodeIdx = 0;
    float progress = 0.0f;
    bool finished = false;

    bool waiting = false;
    Uint32 waitStartTicks = 0;
    float distanceTraveled = 0.0f;
    unordered_set<int> serviceNodes; // nơi thật sự nhận / giao hàng
    vector<int> orderWid;
    vector<int> orderCid;
    vector<bool> picked;   // picked[i] = đã lấy hàng order i chưa
    vector<bool> done;     // done[i] = đã giao xong order i
};

class SDLInterface;
class Driver;
class CityMap {
private:
    SDL_Color GetDriverColor(int driverIndex);
    vector<int> FindAndColorPath(SDL_Renderer* renderer, int startNode, int endNode, SDL_Color pathColor);
    vector<int> stopNodeIndices;
public:
    string selectedAssignName = "";
    bool algorithmSaved = false;
    bool logisticsRunning = false;
    bool logisticsPaused  = false;
    bool logisticsFinished = false;
    bool logisticsStarted = false;
    int currentMode = 0; // mode đang chạy
    Uint32 clockStartTicks = 0;
    Uint32 clockStopTicks  = 0;
    bool wasPaused = false;

    vector<Driver> driverObjs;
    vector<Customer> customerObjs;
    vector<Warehouse> warehouseObjs;
    vector<Intersection> intersections;
    vector<Edge> edges;
    vector<DriverRoute> driverRoutes;


    int randRange(int min, int max);
    float distanceBetween(const Intersection& a, const Intersection& b);
    float DistancePoint(float x1, float y1, float x2, float y2) const;
    void DrawRoad(SDL_Renderer* renderer, const Intersection& a, const Intersection& b);
    void DrawIntersection(SDL_Renderer* renderer, const Intersection& p);

    void SaveCityData(const string& filename = "Citymap.txt");
    bool LoadCityData(const string& filename = "Citymap.txt");
    void SaveData(const vector<Customer>& customerObjs, const vector<Driver>& driverObjs, const vector<Warehouse>& warehouseObjs, const string& filename = "Data.txt");
    bool LoadSavedData(vector<Customer>& customers, vector<Driver>& drivers, vector<Warehouse>& warehouses, const string& filename);
    bool LoadCityData1(const string& filename);

    void RenderAllMap(SDL_Renderer* renderer, TTF_Font* font);
    void GenerateRandomCityMap(SDL_Renderer* renderer, TTF_Font* font, int screenW, int screenH);

    void AddIntersectionWithEdges(SDL_Renderer* renderer, int clickX, int clickY);
    void AddNodeOnly(SDL_Renderer* renderer, SDL_Pos clickPos, AddMode addMode);
    void AddNodeWithEdges(SDL_Renderer* renderer, SDL_Pos clickPos, AddMode addMode);
    void HandleLeftClick(SDL_Renderer* renderer, SDL_Pos clickPos, AddMode addMode, EdgeMode edgeMode);

    vector<float> DijkstraDistances(int startInter) const;
    TSPResult SolveTSP(const vector<int>& custInterIds, int startInter) const;

    int GetNearestIntersectionIndex(float x, float y) const;
    float DistanceIntersections(int a, int b) const;
    vector<float> DijkstraDistancesFrom(int startIdx) const;
    vector<int> NearestNeighborTSP_OnIntersections(const vector<int>& targetInterIdxs, int startIdx) const;
    bool IsWarehouseIntersection(int interIdx) ;
    bool IsCustomerIntersection(int interIdx) ;
    int CustomerToNearestInterIdx(int customerId) const;
    int WarehouseToNearestInterIdx(int warehouseId) const;

    void ResetAllDriversAndLogistics();
    void RunAllDriversLogistics(SDL_Renderer* renderer, int mode, SDLInterface* ui);
};
