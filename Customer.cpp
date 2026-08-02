#include "Customer.h"
#include <sstream>


Customer::Customer(int id, int x, int y) : id(id), x(x), y(y) {}

void Customer::addWarehouse(int wid) {
    warehouseIds.push_back(wid);
}


std::string Customer::toString() const {
    std::ostringstream oss;
    oss << id << " " << x << " " << y;
    if (!warehouseIds.empty()) {
        oss << " |Warehouses:";
        for (int wid : warehouseIds) oss << wid << ",";
    }
    return oss.str();
}
void Customer::RandomAssignToWarehouses(vector<Customer>& customers, const vector<Warehouse>& warehouses)
{
    if (warehouses.empty()) return;

    vector<int> wIDs;
    for (auto& w : warehouses) wIDs.push_back(w.id);

    for (auto& c : customers) {
        c.warehouseIds.clear();

        int maxPick = min(3, (int)wIDs.size());
        int numPick = 1 + rand() % maxPick;

        vector<int> shuffled = wIDs;
        MySelf::shuffle(shuffled);

        // Lấy numPick kho đầu tiên
        c.warehouseIds.assign(shuffled.begin(), shuffled.begin() + numPick);
    }
}
