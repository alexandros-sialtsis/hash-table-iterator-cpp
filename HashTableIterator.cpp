
#include "HashTable.hpp"
#include <iostream>
#include <string>

using namespace std;

HashTable::Iterator::Iterator(const HashTable *t, bool start) {
    int i = 0;

    ht = t;
    if(start) {
        while(ht->isAvailable(i)) {
            i++;
        }
        position = i;
        curr = &ht->table[position];
    }
    else {
        position = ht->capacity;
        curr = &ht->table[position];
    }
}

HashTable::Iterator::Iterator(const Iterator &it) {
    this->curr = it.curr;
    this->position = it.position;
    this->ht = it.ht;
}

HashTable::Iterator& HashTable::Iterator::operator=(const Iterator &it) {
    this->curr = it.curr;
    this->position = it.position;
    this->ht = it.ht;
    return(*this);
}

HashTable::Iterator HashTable::Iterator::operator++() {
    for(position++; ht->isAvailable(position) &&
        position != ht->capacity; position++);
    curr = &ht->table[position];
    return(*this);
}

HashTable::Iterator HashTable::Iterator::operator++(int a) {
    HashTable::Iterator *old_iter = new HashTable::Iterator(*this);

    for(position++; ht->isAvailable(position) &&
        position != ht->capacity; position++);
    curr = &ht->table[position];
    return(*old_iter);
}

bool HashTable::Iterator::operator==(const Iterator &it) const {
    if(this->ht != it.ht) {
        return(false);
    }
    if(this->curr != it.curr) {
        return(false);
    }
    return(true);
}

bool HashTable::Iterator::operator!=(const Iterator &it) const {
    return !(*this == it);
}

const string& HashTable::Iterator::operator*() {
    return(**curr);
}

const string* HashTable::Iterator::operator->() {
    return(*curr);
}

int HashTable::Iterator::pos() const {
    return(position);
}

