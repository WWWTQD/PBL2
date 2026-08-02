#include "Warehouse.h"
#include <sstream>


Warehouse::Warehouse(int id, int x, int y) : id(id), x(x), y(y) {}

void Warehouse::addCustomer(int cid) {
    customerIds.push_back(cid);
}

void Warehouse::addDriver(int did) {
    driverIds.push_back(did);
}

string Warehouse::toString() const {
    ostringstream oss;
    oss << id << " " << x << " " << y;
    if (!customerIds.empty()) {
        oss << " |Customers:";
        for (int cid : customerIds) oss << cid << ",";
    }
    if (!driverIds.empty()) {
        oss << " |Drivers:";
        for (int did : driverIds) oss << did << ",";
    }
    return oss.str();
}

// ========================================================= RANDOM =========================================================//

void Warehouse::AssignByRandom(vector<Warehouse>& warehouses, const vector<Customer>& customers, vector<Driver>& drivers)
{
    if (warehouses.empty() || customers.empty() || drivers.empty()) return;

    // --- 1) Chuẩn bị dữ liệu phụ trợ ---
    // tạo map driverId -> index để cập nhật nhanh
    unordered_map<int, size_t> driverIndex;
    for (size_t i = 0; i < drivers.size(); ++i)
        driverIndex[drivers[i].id] = i;

    // --- 2) Gán đơn hàng cho từng Warehouse ---
    for (auto& w : warehouses)
    {
        w.customerIds.clear();
        w.driverIds.clear();

        // lấy danh sách đơn hàng thực sự của warehouse này
        vector<int> orders;
        for (const auto& c : customers)
            if (MySelf::find(c.warehouseIds, w.id) != -1)
                orders.push_back(c.id);

        // nếu không có đơn thì bỏ qua
        if (orders.empty()) continue;

        // khởi tạo tải hiện tại của mỗi driver
        vector<int> driverLoad(drivers.size(), 0);

        // --- 3) Phân phối đơn theo tải nhỏ nhất (Random + Cân bằng) ---
        for (int cid : orders)
        {
            // tìm tải nhỏ nhất hiện tại
            int minLoad = MySelf::min_element(driverLoad);

            // lấy danh sách driver có tải nhỏ nhất
            vector<size_t> available;
            for (size_t i = 0; i < drivers.size(); ++i)
                if (driverLoad[i] <= minLoad)
                    available.push_back(i);

            if (available.empty()) break;

            // chọn ngẫu nhiên 1 driver trong nhóm ít tải
            size_t chosenPos = available[rand() % available.size()];
            driverLoad[chosenPos]++;

            // gán đơn cho driver
            drivers[chosenPos].customerIds.push_back(cid);
            drivers[chosenPos].warehouseIds.push_back(w.id);

            // cập nhật quan hệ cho warehouse
            w.customerIds.push_back(cid);
            w.driverIds.push_back(drivers[chosenPos].id);
        }

        // loại bỏ trùng lặp an toàn cho warehouse
        MySelf::sortAndUnique(w.customerIds);
        MySelf::sortAndUnique(w.driverIds);
    }
}

// ========================================================= HUNGARIAN =========================================================//

vector<int> HungarianAssign(const vector<vector<float>>& cost)
{
    if (cost.empty()) return {};

    int n = cost.size();
    int m = cost[0].size();
    int N = max(n, m);

    // mở rộng ma trận thành NxN (Hungarian yêu cầu vuông)
    vector<vector<float>> a(N, vector<float>(N, 0));
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            a[i][j] = (i < n && j < m) ? cost[i][j] : 1e9;

    vector<float> u(N + 1), v(N + 1);
    vector<int> p(N + 1), way(N + 1);

    for (int i = 1; i <= N; i++) {
        p[0] = i;
        int j0 = 0;
        vector<float> minv(N + 1, 1e18);
        vector<char> used(N + 1, false);

        do {
            used[j0] = true;
            int i0 = p[j0], j1 = 0;
            float delta = 1e18;

            for (int j = 1; j <= N; j++) {
                if (!used[j]) {
                    float cur = a[i0 - 1][j - 1] - u[i0] - v[j];
                    if (cur < minv[j]) { minv[j] = cur; way[j] = j0; }
                    if (minv[j] < delta) { delta = minv[j]; j1 = j; }
                }
            }

            for (int j = 0; j <= N; j++) {
                if (used[j]) { u[p[j]] += delta; v[j] -= delta; }
                else minv[j] -= delta;
            }

            j0 = j1;
        }
        while (p[j0] != 0);

        do {
            int j1 = way[j0];
            p[j0] = p[j1];
            j0 = j1;
        }
        while (j0);
    }

    vector<int> assignment(n, -1);
    for (int j = 1; j <= N; j++) {
        if (p[j] <= n && j <= m)
            assignment[p[j] - 1] = j - 1;
    }
    return assignment;
}

void Warehouse::AssignByHungarian(vector<Warehouse>& warehouses, const vector<Customer>& customers, vector<Driver>& drivers, const CityMap& cityMap)
{
    if (warehouses.empty() || customers.empty() || drivers.empty()) return;

    for (auto& w : warehouses)
    {
        w.customerIds.clear();
        w.driverIds.clear();

        // --- 1) Thu thập các đơn thực sự thuộc warehouse này ---
        vector<int> orders;
        for (const auto& c : customers)
        {
            // nếu customer có warehouse này trong danh sách đơn
            if (MySelf::find(c.warehouseIds, w.id) != -1)
                orders.push_back(c.id);
        }

        // không có đơn thì bỏ qua warehouse này
        if (orders.empty()) continue;

        int n = orders.size();   // số đơn (Customer-Warehouse)
        int m = drivers.size();  // số driver

        // --- 2) Xây dựng ma trận chi phí cho Hungarian ---
        vector<vector<float>> cost(n, vector<float>(m));

        for (int i = 0; i < n; i++)
        {
            int cid = orders[i];

            // tọa độ customer
            float cx = customers[cid - 1].x;
            float cy = customers[cid - 1].y;

            for (int j = 0; j < m; j++)
            {
                // tọa độ driver
                float dx = drivers[j].x;
                float dy = drivers[j].y;

                // tọa độ warehouse
                float wx = w.x;
                float wy = w.y;

                // chi phí = driver → warehouse → customer
                cost[i][j] =
                    cityMap.DistancePoint(dx, dy, wx, wy) +
                    cityMap.DistancePoint(wx, wy, cx, cy);
            }
        }

        // --- 3) Chạy Hungarian để gán mỗi đơn cho 1 driver tối ưu ---
        vector<int> assign = HungarianAssign(cost);

        // --- 4) Ghi kết quả gán vào Driver và Warehouse ---
        for (int i = 0; i < n; i++)
        {
            if (assign[i] == -1) continue;

            int driverIdx = assign[i];
            int cid = orders[i];

            // cập nhật driver
            drivers[driverIdx].customerIds.push_back(cid);
            drivers[driverIdx].warehouseIds.push_back(w.id);

            // cập nhật warehouse
            w.customerIds.push_back(cid);
            w.driverIds.push_back(drivers[driverIdx].id);
        }

        // loại trùng an toàn
        MySelf::sortAndUnique(w.customerIds);
        MySelf::sortAndUnique(w.driverIds);
    }
}


// ========================================================= GREEDY =========================================================//

void Warehouse::AssignByGreedy(vector<Warehouse>& warehouses, const vector<Customer>& customers, vector<Driver>& drivers, const CityMap& cityMap)
{
    if (warehouses.empty() || customers.empty() || drivers.empty()) return;

    // reset dữ liệu cũ
    for (auto& w : warehouses) {
        w.customerIds.clear();
        w.driverIds.clear();
    }
    for (auto& d : drivers) {
        d.customerIds.clear();
        d.warehouseIds.clear();
    }

    // --- 1) Xử lý từng warehouse ---
    for (auto& w : warehouses)
    {
        // lấy danh sách customer thực sự thuộc warehouse này
        vector<int> orders;
        for (const auto& c : customers)
            if (MySelf::find(c.warehouseIds, w.id) != -1)
                orders.push_back(c.id);

        if (orders.empty()) continue;

        // --- 2) Gán từng đơn (Customer–Warehouse) theo Greedy ---
        for (int cid : orders)
        {
            const auto& c = customers[cid - 1];

            float bestCost = 1e18;
            int bestDriver = -1;

            // tìm driver có chi phí nhỏ nhất
            for (int j = 0; j < (int)drivers.size(); j++)
            {
                float dx = drivers[j].x;
                float dy = drivers[j].y;

                float cost =
                    cityMap.DistancePoint(dx, dy, w.x, w.y) +   // driver → warehouse
                    cityMap.DistancePoint(w.x, w.y, c.x, c.y); // warehouse → customer

                if (cost < bestCost) {
                    bestCost = cost;
                    bestDriver = j;
                }
            }

            if (bestDriver == -1) continue;

            // --- 3) Cập nhật kết quả gán ---
            drivers[bestDriver].customerIds.push_back(cid);
            drivers[bestDriver].warehouseIds.push_back(w.id);

            w.customerIds.push_back(cid);
            w.driverIds.push_back(drivers[bestDriver].id);
        }

        // dọn trùng cho warehouse (an toàn)
        MySelf::sortAndUnique(w.customerIds);
        MySelf::sortAndUnique(w.driverIds);
    }
}


// ========================================================= HUNGARIAN & K-MEANS =========================================================//

void Warehouse::AssignByHungarianKMeans(vector<Warehouse>& warehouses, const vector<Customer>& customers, vector<Driver>& drivers, const CityMap& cityMap)
{
    if (warehouses.empty() || customers.empty() || drivers.empty()) return;

    // --- 1) CLEAR TOÀN BỘ ---
    for (auto& d : drivers) {
        d.customerIds.clear();
        d.warehouseIds.clear();
    }
    for (auto& w : warehouses) {
        w.customerIds.clear();
        w.driverIds.clear();
    }

    int K = drivers.size();

    // --- 2) K-MEANS THEO DRIVER (PHÂN VÙNG CUSTOMER) ---
    // tâm ban đầu = vị trí driver
    vector<float> cx(K), cy(K);
    for (int k = 0; k < K; k++) {
        cx[k] = drivers[k].x;
        cy[k] = drivers[k].y;
    }

    int N = customers.size();
    vector<int> cluster(N, -1);
    bool changed = true;

    while (changed) {
        changed = false;

        // gán mỗi customer vào cluster gần nhất
        for (int i = 0; i < N; i++) {
            float bestD = 1e18f;
            int bestK = 0;
            for (int k = 0; k < K; k++) {
                float dx = customers[i].x - cx[k];
                float dy = customers[i].y - cy[k];
                float d = dx * dx + dy * dy;
                if (d < bestD) {
                    bestD = d;
                    bestK = k;
                }
            }
            if (cluster[i] != bestK) {
                cluster[i] = bestK;
                changed = true;
            }
        }

        // cập nhật lại tâm cluster
        vector<float> sx(K, 0), sy(K, 0);
        vector<int> cnt(K, 0);
        for (int i = 0; i < N; i++) {
            int k = cluster[i];
            sx[k] += customers[i].x;
            sy[k] += customers[i].y;
            cnt[k]++;
        }
        for (int k = 0; k < K; k++) {
            if (cnt[k] > 0) {
                cx[k] = sx[k] / cnt[k];
                cy[k] = sy[k] / cnt[k];
            }
        }
    }

    // --- 3) HUNGARIAN THEO TỪNG WAREHOUSE ---
    for (auto& w : warehouses)
    {
        // gom các đơn thực sự của warehouse này
        vector<int> orderCustIds;
        for (int i = 0; i < N; i++) {
            if (MySelf::find(customers[i].warehouseIds, w.id) != -1)
                orderCustIds.push_back(customers[i].id);
        }
        if (orderCustIds.empty()) continue;

        int n = orderCustIds.size();

        // danh sách driver hợp lệ (KMeans chỉ bias cost)
        vector<int> validDrivers;
        for (int k = 0; k < K; k++)
            validDrivers.push_back(k);

        int m = validDrivers.size();
        vector<vector<float>> cost(n, vector<float>(m));

        for (int i = 0; i < n; i++) {
            int cid = orderCustIds[i];
            const auto& c = customers[cid - 1];

            for (int j = 0; j < m; j++) {
                int dIdx = validDrivers[j];
                const auto& d = drivers[dIdx];

                float baseCost =
                    cityMap.DistancePoint(d.x, d.y, w.x, w.y) +
                    cityMap.DistancePoint(w.x, w.y, c.x, c.y);

                // penalty nếu customer khác cluster của driver
                float penalty = (cluster[cid - 1] == dIdx) ? 0.0f : 10000.0f;
                cost[i][j] = baseCost + penalty;
            }
        }

        vector<int> assign = HungarianAssign(cost);

        // --- 4) PUSH ĐƠN HÀNG (KHÔNG DỌN TRÙNG DRIVER) ---
        for (int i = 0; i < n; i++) {
            if (assign[i] == -1) continue;

            int driverIdx = validDrivers[assign[i]];
            int cid = orderCustIds[i];

            drivers[driverIdx].customerIds.push_back(cid);
            drivers[driverIdx].warehouseIds.push_back(w.id);

            w.customerIds.push_back(cid);
            w.driverIds.push_back(drivers[driverIdx].id);
        }

        // dọn trùng cho warehouse
        MySelf::sortAndUnique(w.customerIds);
        MySelf::sortAndUnique(w.driverIds);
    }
}

// ========================================================= K-MEANS =========================================================//

void Warehouse::AssignByKMeans(vector<Warehouse>& warehouses, const vector<Customer>& customers, vector<Driver>& drivers)
{
    if (customers.empty() || drivers.empty()) return;

    int K = drivers.size();

    // --- 1) CLEAR DỮ LIỆU CŨ ---
    for (auto& w : warehouses) {
        w.customerIds.clear();
        w.driverIds.clear();
    }
    for (auto& d : drivers) {
        d.customerIds.clear();
        d.warehouseIds.clear();
    }

    // --- 2) TẠO DANH SÁCH ĐƠN (CUSTOMER – WAREHOUSE) ---
    vector<int> orderCustomer;
    vector<int> orderWarehouse;

    for (const auto& c : customers) {
        for (int wid : c.warehouseIds) {
            orderCustomer.push_back(c.id);
            orderWarehouse.push_back(wid);
        }
    }

    int N = orderCustomer.size();
    if (N == 0) return;

    // --- 3) KHỞI TẠO CENTROID = VỊ TRÍ DRIVER ---
    vector<float> cx(K), cy(K);
    for (int i = 0; i < K; i++) {
        cx[i] = drivers[i].x;
        cy[i] = drivers[i].y;
    }

    vector<int> cluster(N, -1);
    bool changed = true;
    int iter = 0;

    // --- 4) K-MEANS TRÊN TẬP ĐƠN ---
    while (changed && iter++ < 20) {
        changed = false;

        // gán mỗi đơn vào centroid gần nhất
        for (int i = 0; i < N; i++) {
            int cid = orderCustomer[i];
            const auto& c = customers[cid - 1];

            float best = 1e18f;
            int bestK = 0;

            for (int k = 0; k < K; k++) {
                float dx = c.x - cx[k];
                float dy = c.y - cy[k];
                float d = dx * dx + dy * dy;
                if (d < best) {
                    best = d;
                    bestK = k;
                }
            }

            if (cluster[i] != bestK) {
                cluster[i] = bestK;
                changed = true;
            }
        }

        // cập nhật lại centroid
        vector<float> sumX(K, 0), sumY(K, 0);
        vector<int> cnt(K, 0);

        for (int i = 0; i < N; i++) {
            int k = cluster[i];
            int cid = orderCustomer[i];
            sumX[k] += customers[cid - 1].x;
            sumY[k] += customers[cid - 1].y;
            cnt[k]++;
        }

        for (int k = 0; k < K; k++) {
            if (cnt[k] > 0) {
                cx[k] = sumX[k] / cnt[k];
                cy[k] = sumY[k] / cnt[k];
            }
        }
    }

    // --- 5) GÁN MỖI ĐƠN → 1 DRIVER THEO CLUSTER ---
    for (int i = 0; i < N; i++) {
        int cid = orderCustomer[i];
        int wid = orderWarehouse[i];
        int k = cluster[i];

        drivers[k].customerIds.push_back(cid);
        drivers[k].warehouseIds.push_back(wid);

        warehouses[wid - 1].customerIds.push_back(cid);
        warehouses[wid - 1].driverIds.push_back(drivers[k].id);
    }

    // --- 6) DỌN TRÙNG DỮ LIỆU ---
    for (auto& d : drivers) {
        MySelf::sortAndUnique(d.customerIds);
        MySelf::sortAndUnique(d.warehouseIds);
    }
    for (auto& w : warehouses) {
        MySelf::sortAndUnique(w.customerIds);
        MySelf::sortAndUnique(w.driverIds);
    }
}


// ========================================================= CLUSTER & TSP =========================================================//

void Warehouse::AssignByClusterTSP(vector<Warehouse>& warehouses, const vector<Customer>& customers, vector<Driver>& drivers, CityMap& cityMap)
{
    if (warehouses.empty() || customers.empty() || drivers.empty()) return;

    int K = drivers.size();
    int N = customers.size();

    // --- 1) CLEAR DỮ LIỆU CŨ ---
    for (auto& w : warehouses) {
        w.customerIds.clear();
        w.driverIds.clear();
    }
    for (auto& d : drivers) {
        d.customerIds.clear();
        d.warehouseIds.clear();
    }

    // --- 2) K-MEANS: CLUSTER CUSTOMER THEO DRIVER ---
    vector<float> cx(K), cy(K);
    for (int k = 0; k < K; k++) {
        cx[k] = drivers[k].x;
        cy[k] = drivers[k].y;
    }

    vector<int> cluster(N, -1);
    bool changed = true;
    int iter = 0;

    while (changed && iter++ < 20) {
        changed = false;

        // gán customer vào cluster gần nhất
        for (int i = 0; i < N; i++) {
            float best = 1e18f;
            int bestK = 0;

            for (int k = 0; k < K; k++) {
                float dx = customers[i].x - cx[k];
                float dy = customers[i].y - cy[k];
                float d = dx*dx + dy*dy;

                // thêm nhiễu ±0.1~0.5 để phá vỡ tie
                d += (rand() % 1000 / 1000.0f) * 0.5f;
                if (d < best) {
                    best = d;
                    bestK = k;
                }
            }

            if (cluster[i] != bestK) {
                cluster[i] = bestK;
                changed = true;
            }
        }

        // cập nhật lại centroid
        vector<float> sumX(K, 0), sumY(K, 0);
        vector<int> cnt(K, 0);

        for (int i = 0; i < N; i++) {
            int k = cluster[i];
            sumX[k] += customers[i].x;
            sumY[k] += customers[i].y;
            cnt[k]++;
        }

        for (int k = 0; k < K; k++) {
            if (cnt[k] > 0) {
                cx[k] = sumX[k] / cnt[k];
                cy[k] = sumY[k] / cnt[k];
            }
        }
    }

    // --- 3) GOM CUSTOMER THEO CLUSTER ---
    vector<vector<int>> clusters(K);
    for (int i = 0; i < N; i++)
        clusters[cluster[i]].push_back(customers[i].id);

    // --- 4) MỖI CLUSTER → TSP → GÁN ĐƠN ---
    for (int k = 0; k < K; k++)
    {
        if (clusters[k].empty()) continue;

        vector<int> custIds = clusters[k];
        vector<int> custInterIds;

        // map customer → nearest intersection
        for (int cid : custIds) {
            int inter = cityMap.CustomerToNearestInterIdx(cid);
            custInterIds.push_back(inter);
        }

        // intersection bắt đầu của driver
        int driverStart = cityMap.GetNearestIntersectionIndex(
            drivers[k].x, drivers[k].y
        );

        // chạy TSP trên các intersection
        auto tsp = cityMap.SolveTSP(custInterIds, driverStart);

        // gán đơn theo thứ tự TSP
        for (int interId : tsp.order)
        {
            int idx = MySelf::find(custInterIds, interId);
            if (idx == -1) continue;

            int cid = custIds[idx];

            // gán customer cho driver
            drivers[k].customerIds.push_back(cid);

            // gán các warehouse của customer cho driver
            for (auto& w : warehouses) {
                if (MySelf::find(customers[cid - 1].warehouseIds, w.id) != -1) {
                    drivers[k].warehouseIds.push_back(w.id);
                    w.customerIds.push_back(cid);
                    w.driverIds.push_back(drivers[k].id);
                }
            }
        }
    }

    // --- 5) DỌN DUPLICATE ---
    for (auto& d : drivers) {
        MySelf::sortAndUnique(d.customerIds);
        MySelf::sortAndUnique(d.warehouseIds);
    }

    for (auto& w : warehouses) {
        MySelf::sortAndUnique(w.customerIds);
        MySelf::sortAndUnique(w.driverIds);
    }
}

