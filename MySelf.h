#ifndef MYSELF_H
#define MYSELF_H
#pragma once

#include <vector>
#include <functional>
#include <cstdlib>
#include <ctime>

class MySelf {
public:
    template<typename T>
    static void swap(T& a, T& b) {
        T tmp = a;
        a = b;
        b = tmp;
    }

    template<typename T>
    static void shuffle(std::vector<T>& vec) {
        static bool seeded = false;
        if (!seeded) {
            srand((unsigned)time(nullptr));
            seeded = true;
        }
        for (int i = (int)vec.size() - 1; i > 0; --i) {
            int j = rand() % (i + 1);
            swap(vec[i], vec[j]);
        }
    }

    template<typename T>
    static void sort(std::vector<T>& vec) {
        for (size_t i = 0; i < vec.size(); ++i)
            for (size_t j = i + 1; j < vec.size(); ++j)
                if (vec[j] < vec[i])
                    swap(vec[i], vec[j]);
    }

    template<typename T>
    static void reverse(std::vector<T>& vec) {
        size_t n = vec.size();
        for (size_t i = 0; i < n / 2; ++i)
            swap(vec[i], vec[n - 1 - i]);
    }

    template<typename T>
    static void unique(std::vector<T>& vec) {
        std::vector<T> res;
        for (auto &x : vec) {
            bool dup = false;
            for (auto &y : res)
                if (x == y) { dup = true; break; }
            if (!dup) res.push_back(x);
        }
        vec = res;
    }

    template<typename T>
    static int find(const std::vector<T>& vec, const T& value) {
        for (size_t i = 0; i < vec.size(); ++i)
            if (vec[i] == value) return (int)i;
        return -1;
    }

    template<typename T>
    static int find_if(const std::vector<T>& vec,
                       std::function<bool(const T&)> pred) {
        for (size_t i = 0; i < vec.size(); ++i)
            if (pred(vec[i])) return (int)i;
        return -1;
    }

    template<typename T>
    static void remove_if(std::vector<T>& vec,
                          std::function<bool(const T&)> pred) {
        std::vector<T> res;
        for (auto &x : vec)
            if (!pred(x)) res.push_back(x);
        vec = res;
    }

    template<typename T>
    static T min_element(const std::vector<T>& vec) {
        if (vec.empty()) return T();
        T m = vec[0];
        for (auto &x : vec)
            if (x < m) m = x;
        return m;
    }

    template<typename T>
    static void sortAndUnique(std::vector<T>& vec) {
        sort(vec);
        unique(vec);
    }

    template<typename T>
    static bool heapEmpty(const std::vector<T>& h) {
        return h.empty();
    }

    template<typename T>
    static T heapTop(const std::vector<T>& h) {
        return h[0];
    }

    template<typename T>
    static void heapPush(std::vector<T>& h, const T& val) {
        h.push_back(val);
        int i = (int)h.size() - 1;
        while (i > 0) {
            int p = (i - 1) / 2;
            if (!(h[i] < h[p])) break;
            T tmp = h[i]; h[i] = h[p]; h[p] = tmp;
            i = p;
        }
    }

    template<typename T>
    static void heapPop(std::vector<T>& h) {
        if (h.empty()) return;
        h[0] = h.back();
        h.pop_back();
        int n = (int)h.size();
        int i = 0;
        while (true) {
            int l = 2*i + 1;
            int r = 2*i + 2;
            int smallest = i;
            if (l < n && h[l] < h[smallest]) smallest = l;
            if (r < n && h[r] < h[smallest]) smallest = r;
            if (smallest == i) break;
            T tmp = h[i]; h[i] = h[smallest]; h[smallest] = tmp;
            i = smallest;
        }
    }
};

#endif
