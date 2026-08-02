#ifndef CUSTOMER_H
#define CUSTOMER_H

#pragma once
#include <string>
#include <vector>
#include "CityMap.h"

using namespace std;

class Warehouse;
class Customer {
public:
    int id;
    int x, y;
    vector<int> warehouseIds;
    string relationshipStr = "";

    Customer(): id(0), x(0), y(0) {}
    Customer(int id, int x, int y);

    int GetID() const { return id; }
    int GetX() const { return x; }
    int GetY() const { return y; }

    void addWarehouse(int wid);

    string toString() const;

    static void RandomAssignToWarehouses(vector<Customer>& customers, const vector<Warehouse>& warehouses);
};
#endif
