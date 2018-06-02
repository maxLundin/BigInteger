//
// Created by max on 15.05.18.
//

#ifndef LOL
#define LOL

#include <vector>
#include <memory>
#include <cassert>
#include <cstring>

using std::shared_ptr;
using std::vector;

class OptimizedArrayList {
private:
    static const size_t SMALL_SIZE = 1;

    struct data {
        uint32_t small[SMALL_SIZE];
        shared_ptr<vector<uint32_t>> big;

        data() : big(nullptr) {};

        ~data() { big = nullptr; };
    };

    data data;
    size_t __size;
    bool isSmall;

    void make_big();

public:
    OptimizedArrayList();

    OptimizedArrayList(OptimizedArrayList const &other) noexcept;

    OptimizedArrayList &operator=(OptimizedArrayList const &other) noexcept;

    size_t size() const;

    uint32_t back() const;

    void push_back(uint32_t x);

    void pop_back();

    uint32_t &operator[](size_t ind);

    uint32_t const &operator[](size_t ind) const;

    void resize(size_t len);

    void clear();
};


#endif /* LOL */
