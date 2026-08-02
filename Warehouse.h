#ifndef WAREHOUSE_H
#define WAREHOUSE_H

#pragma once
#include <cmath>
#include "CityMap.h"
#include "MySelf.h"

using namespace std;
class CityMap;
class Customer;
class Driver;
class Warehouse {
public:
    int id;
    int x, y;
    vector<int> customerIds;
    vector<int> driverIds;
    string relationshipStr = "";

    Warehouse() : id(0), x(0), y(0) {}
    Warehouse(int id, int x, int y);

    int GetID() const { return id; }
    int GetX() const { return x; }
    int GetY() const { return y; }

    void addCustomer(int cid);
    void addDriver(int did);

    string toString() const;

    // ========================================================= ALGORITHM DIVIDE =========================================================//
    static void AssignByRandom(std::vector<Warehouse>& warehouses, const std::vector<Customer>& customers, std::vector<Driver>& drivers);
    static void AssignByHungarian(vector<Warehouse>& warehouses, const vector<Customer>& customers, vector<Driver>& drivers, const CityMap& cityMap);
    static void AssignByGreedy(vector<Warehouse>& warehouses, const vector<Customer>& customers, vector<Driver>& drivers, const CityMap& cityMap);
    static void AssignByHungarianKMeans(vector<Warehouse>& warehouses, const vector<Customer>& customers, vector<Driver>& drivers, const CityMap& cityMap);
    static void AssignByKMeans(vector<Warehouse>& warehouses, const vector<Customer>& customers, vector<Driver>& drivers);
    static void AssignByClusterTSP(vector<Warehouse>& warehouses, const vector<Customer>& customers, vector<Driver>& drivers, CityMap& cityMap);
};
#endif
