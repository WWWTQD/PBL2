#include "Driver.h"
#include <sstream>

Driver::Driver(int id, int x, int y) : id(id), x(x), y(y), startX((float)x), startY((float)y){}

void Driver::addCustomer(int cid) {
    for (int c : customerIds) if (c == cid) return;
    customerIds.push_back(cid);
}

void Driver::addWarehouse(int wid) {
    for (int w : warehouseIds) if (w == wid) return;
    warehouseIds.push_back(wid);
}

std::string Driver::toString() const {
    std::ostringstream oss;
    oss << id << " " << x << " " << y;
    if (!customerIds.empty()) {
        oss << " |Customers:";
        for (int cid : customerIds) oss << cid << ",";
    }
    if (!warehouseIds.empty()) {
        oss << " |Warehouses:";
        for (int wid : warehouseIds) oss << wid << ",";
    }
    return oss.str();
}
void Driver::RandomAssignToCustomerWarehouse(vector<Driver>& drivers, const vector<Customer>& customers, const vector<Warehouse>& warehouses)
{
    // Reset
    for (auto& d : drivers) { d.customerIds.clear(); d.warehouseIds.clear(); }

    for (const auto& w : warehouses)
    {
        if (w.customerIds.empty() || w.driverIds.empty()) continue; // skip nếu chưa có dữ liệu

        for (size_t i = 0; i < w.customerIds.size(); ++i)
        {
            int cid = w.customerIds[i];
            int did = w.driverIds[i % w.driverIds.size()];

            // cập nhật driver bằng map id->index (tạo map một lần ngoài vòng lặp nếu muốn)
            for (auto& d : drivers)
            {
                if (d.id == did)
                {
                    d.customerIds.push_back(cid);
                    d.warehouseIds.push_back(w.id);
                    break;
                }
            }
        }
    }
}




