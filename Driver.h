#ifndef DRIVER_H
#define DRIVER_H

#pragma once
#include <string>
#include <vector>
#include "CityMap.h"

using namespace std;

class CityMap;
class Customer;
class Warehouse;
class Driver {
public:
    int id;
    int x, y;
    float startX;
    float startY;

    vector<int> customerIds;
    vector<int> warehouseIds;
    string relationshipStr = "";

    Driver() : id(0), x(0), y(0) {}
    Driver(int id, int x, int y);

    int GetID() const { return id; }
    int GetX() const { return x; }
    int GetY() const { return y; }

    void addCustomer(int cid);
    void addWarehouse(int wid);

    string toString() const;

    static void RandomAssignToCustomerWarehouse(vector<Driver>& drivers, const vector<Customer>& customers, const vector<Warehouse>& warehouses);
};
#endif
