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

    union data {
        uint32_t small;
        shared_ptr<vector<uint32_t>> big;

        data() {};

        ~data() {};
    };

    data data;
    size_t _size;
    bool isSmall;


    void make_big();

    void make_unique();

public:

    OptimizedArrayList();

    ~OptimizedArrayList();

    OptimizedArrayList &operator=(OptimizedArrayList const &other);

    OptimizedArrayList(OptimizedArrayList const &other);

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
