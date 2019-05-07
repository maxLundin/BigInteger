//
// Created by max on 02.06.18.
//

#include <iostream>
#include "OptimizedArrayList.h"

OptimizedArrayList::OptimizedArrayList() : _size(0), isSmall(true) {
    data.small = 0;
}

OptimizedArrayList::OptimizedArrayList(OptimizedArrayList &&other) noexcept {
    _size = other._size;
    isSmall = other.isSmall;
    if (isSmall) {
        data.small = other.data.small;
    } else {
        new(&data.big) std::shared_ptr<vector<uint32_t>>(other.data.big);
        other.isSmall = true;
        other.data.big.~shared_ptr();
        other.data.small = 0;
        other._size = 0;
    }
}

OptimizedArrayList::OptimizedArrayList(OptimizedArrayList const &other) {
    _size = other._size;
    isSmall = other.isSmall;
    if (other.isSmall) {
        data.small = other.data.small;
    } else {
        new(&data.big) std::shared_ptr<vector<uint32_t>>(other.data.big);
    }
}

OptimizedArrayList::OptimizedArrayList(std::initializer_list<uint32_t> list) {
    isSmall = (list.size() <= 1);
    _size = list.size();
    if (!isSmall) {
        vector<uint32_t> *vect = new vector<uint32_t>(list);
        new(&data.big) std::shared_ptr<vector<uint32_t >>(vect);
    } else {
        data.small = *list.begin();
    }
}


OptimizedArrayList &OptimizedArrayList::operator=(OptimizedArrayList const &other) {
    _size = other._size;

    if (!isSmall) {
        data.big.reset();
    }
    if (other.isSmall) {

        data.small = other.data.small;
    } else {
        new(&data.big) std::shared_ptr<vector<uint32_t>>(other.data.big);
    }
    isSmall = other.isSmall;
    return *this;
}

size_t OptimizedArrayList::size() const {
    return _size;
}

uint32_t OptimizedArrayList::back() const {
    assert(_size > 0);
    return (isSmall ? data.small : data.big->back());
}

void OptimizedArrayList::push_back(uint32_t const x) {
    if (isSmall && _size < 1) {
        data.small = x;
        _size++;
        return;
    }
    make_unique();
    make_big();
    (*data.big.get()).push_back(x);
    _size++;
}

void OptimizedArrayList::make_big() {
    if (!isSmall)
        return;

    isSmall = false;

    uint32_t mas = data.small;

    vector<uint32_t> *vect = new vector<uint32_t>(_size);

    new(&data.big) std::shared_ptr<std::vector<uint32_t>>(vect);

    (*data.big.get())[0] = mas;
}

void OptimizedArrayList::pop_back() {
    assert(_size > 0);
    _size--;
    if (isSmall) {
        return;
    }
    make_unique();
    (*data.big.get()).pop_back();
    if (data.big->size() == 1) {
        uint32_t tmp = data.big->back();
        data.big.~shared_ptr();
        isSmall = true;
        data.small = tmp;
    }
}

uint32_t &OptimizedArrayList::operator[](size_t ind) {
    assert(ind < _size);
    if (isSmall)
        return data.small;
    make_unique();
    return (*data.big.get())[ind];
}

uint32_t const &OptimizedArrayList::operator[](size_t ind) const {
    assert(ind < _size);
    return (isSmall ? data.small : (*data.big.get())[ind]);
}

void OptimizedArrayList::resize(size_t len) {
    _size = len;
    if (!isSmall) {

        data.big.reset();
        data.big.~shared_ptr();
    }
    isSmall = true;

    make_big();

    for (size_t i = 0; i < _size; i++) {
        (*data.big)[i] = 0;
    }

}

void OptimizedArrayList::clear() {
    make_unique();
    if (!isSmall) {
        data.big.reset();
        data.big.~shared_ptr();
        isSmall = true;
    }
    memset(&data.small, 0, sizeof(uint32_t));
    _size = 0;
}

void OptimizedArrayList::make_unique() {
    if (isSmall || data.big.unique()) {
        return;
    }
    data.big = std::make_shared<std::vector<uint32_t >>(*data.big);
}

OptimizedArrayList::~OptimizedArrayList() {
    if (!isSmall) {
        data.big.reset();
        data.big.~shared_ptr();
    }
}
