//
// Created by max on 15.05.18.
//

#pragma once

#include <vector>
#include <memory>
#include <cassert>
#include <cstring>

using std::shared_ptr;
using std::vector;

class OptimizedArrayList {
public:

    OptimizedArrayList();

    OptimizedArrayList &operator=(OptimizedArrayList const &other);

    OptimizedArrayList(OptimizedArrayList const &other);

    OptimizedArrayList(OptimizedArrayList  &&other) noexcept;

    OptimizedArrayList(std::initializer_list<uint32_t> list);

    ~OptimizedArrayList();

    size_t size() const;

    uint32_t back() const;

    void push_back(uint32_t x);

    void pop_back();

    uint32_t &operator[](size_t ind);

    uint32_t const &operator[](size_t ind) const;

    void resize(size_t len);

    void clear();

    typedef uint32_t *iterator;
    typedef const uint32_t *const_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    iterator begin() {
        make_unique();
        if (!isSmall) {
            return data.big->data();
        } else {
            return &data.small;
        }
    }

    iterator end() {
        make_unique();
        if (!isSmall) {
            return data.big->data() + _size;
        } else {
            return (&data.small) + 1;
        }
    }

    const_iterator begin() const {
        if (!isSmall) {
            return data.big->data();
        } else {
            return &data.small;
        }
    }

    const_iterator end() const {
        if (!isSmall) {
            return data.big->data() + _size;
        } else {
            return (&data.small) + 1;
        }
    }

    const_reverse_iterator rend() const {
        return const_reverse_iterator(begin());
    }

    const_reverse_iterator rbegin() const {
        return const_reverse_iterator(end());
    }

    reverse_iterator rend() {
        return reverse_iterator(begin());
    }

    reverse_iterator rbegin() {
        return reverse_iterator(end());
    }

private:
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
};
