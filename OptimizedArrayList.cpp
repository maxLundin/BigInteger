//
// Created by max on 02.06.18.
//

#include "OptimizedArrayList.h"

OptimizedArrayList::OptimizedArrayList() : __size(0), isSmall(true) {}

void OptimizedArrayList::make_big() {
    if (!isSmall)
        return;

    isSmall = false;
    data.big = std::make_shared<vector<uint32_t>>(__size);

    for (size_t i = 0; i < __size; i++) {
        (*data.big)[i] = data.small[i];
    }
}

OptimizedArrayList::OptimizedArrayList(OptimizedArrayList const &other) noexcept {
    __size = other.__size;
    if (other.isSmall) {
        for (size_t i = 0; i < other.__size; i++) {
            data.small[i] = other.data.small[i];
        }
        isSmall = true;
        data.big = nullptr;
    } else {
        data.big = std::make_shared<vector<uint32_t>>(*other.data.big);
        isSmall = false;
    }
}

OptimizedArrayList &OptimizedArrayList::operator=(OptimizedArrayList const &other) noexcept {
    __size = other.__size;
    if (other.isSmall) {
        for (size_t i = 0; i < other.__size; i++) {
            data.small[i] = other.data.small[i];
        }
        isSmall = true;
        data.big = nullptr;
    } else {
        data.big = std::make_shared<vector<uint32_t>>(*other.data.big);
        isSmall = false;
    }
    return *this;
}

size_t OptimizedArrayList::size() const {
    return __size;
}

uint32_t OptimizedArrayList::back() const {
    assert(__size > 0);
    return (isSmall ? data.small[__size - 1] : (*data.big)[__size - 1]);
}

void OptimizedArrayList::push_back(uint32_t const x) {
    if (isSmall && __size < SMALL_SIZE) {
        data.small[__size] = x;
        __size++;
        return;
    }
    make_big();
    data.big->push_back(x);
    __size++;
}

void OptimizedArrayList::pop_back() {
    assert(__size > 0);
    __size--;
    if (isSmall) {
        return;
    }
    data.big->pop_back();
}

uint32_t &OptimizedArrayList::operator[](size_t ind) {
    assert(ind < __size);
    if (isSmall)
        return data.small[ind];
    return (*data.big)[ind];
}

uint32_t const &OptimizedArrayList::operator[](size_t ind) const {
    assert(ind < __size);
    return (isSmall ? data.small[ind] : (*data.big)[ind]);
}

void OptimizedArrayList::resize(size_t len) {
    if (isSmall) {
        if (len >= SMALL_SIZE) {
            make_big();
            (*data.big).resize(len);
        } else {
            for (size_t i = __size; i < SMALL_SIZE; i++) {
                (data.small[i]) = 0;
            }
        }
    } else {
        (*data.big).resize(len);
    }
    __size = len;
}

void OptimizedArrayList::clear() {
    if (!isSmall) {
        (*data.big).clear();
        isSmall = true;
    }
    __size = 0;
}
