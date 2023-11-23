
#include "HashTable.hpp"
#include "HashTableException.hpp"
#include <iostream>
#include <string>
#include <cstring>

using namespace std;

unsigned long HashTable::getHashCode(const char *str) {
    unsigned long hash = 97;
    int c;

    while((c = *(str++)) != '\0') {
        hash = ((hash << 5) + hash) + c;
    }
    return(hash);
}

unsigned long HashTable::getHashCode(const string& str) {
    return getHashCode(&str[0]);
}

HashTable::HashTable(int capacity) {
    if(capacity < 0) {
        throw bad_alloc();
    }
    table = new (nothrow) string*[capacity];
    if(table == nullptr) {
        throw bad_alloc();
    }
    size = 0;
    this->capacity = capacity;
    for(int i = 0; i < capacity; i++) {
        table[i] = nullptr;
    }
}

HashTable::HashTable(const HashTable &ht) {
    this->capacity = ht.capacity;
    this->size = ht.size;

    table = new (nothrow) string*[this->capacity];
    if(table == nullptr) {
        throw bad_alloc();
    }

    for(int i = 0; i < this->capacity; i++) {
        if(ht.table[i] == (string *)0xFFFF) {
            this->table[i] = (string *)0xFFFF;
        }
        else if(ht.table[i] == nullptr) {
            this->table[i] = nullptr;
        }
        else {
            this->table[i] = new string(*ht.table[i]);
        }
    }
}

HashTable::~HashTable() {
    for(int i = 0; i < capacity; i++) {
        if(table[i] != nullptr && table[i] != (string *)0xFFFF) {
            delete table[i];
        }
    }
    delete []table;
}

int HashTable::getSize() const {
    return size;
}

int HashTable::getCapacity() const {
    return capacity;
}

bool HashTable::isEmpty(int pos) const {
    if(pos >= capacity) {
        return(false);
    }
    else if(table[pos] == nullptr) {
        return(true);
    }
    else {
        return(false);
    }
}

bool HashTable::isTomb(int pos) const {
    if(pos >= capacity) {
        return(false);
    }
    else if(table[pos] == (string *)0xFFFF) {
        return(true);
    }
    else {
        return(false);
    }
}

bool HashTable::isAvailable(int pos) const {
    return(this->isEmpty(pos) || this->isTomb(pos));
}

// returns pos of the string in the table
// or next empty (null) position
int HashTable::findToSearch(const string& str) const {
    unsigned long hash_value;
    int pos, start_pos;

    hash_value = getHashCode(str);
    pos = start_pos = hash_value % capacity;
    while(1) {
        if(isEmpty(pos) || (!isTomb(pos) && *table[pos] == str) ) {
            return(pos);
        }
        hash_value++;
        pos = hash_value % capacity;
        if(pos == start_pos) {
            return(pos);
        }
    }
}

// returns where str is supposed to be inserted
int HashTable::findToInsert(const string& str) const {
    unsigned long hash_value;
    int pos, start_pos;

    hash_value = getHashCode(str);
    pos = start_pos = hash_value % capacity;
    while(1) {
        if(isAvailable(pos)) {
            return(pos);
        }
        hash_value++;
        pos = hash_value % capacity;
        if(pos == start_pos) {
            return(-1);
        }
    }
}

bool HashTable::contains(const string &s) const {
    int pos;

    pos = findToSearch(s);
    if(isEmpty(pos) || isTomb(pos) || *table[pos] != s) {
        return(false);
    }
    else {
        return(true);
    }
}

bool HashTable::contains(const char *s) const {
    int pos;

    pos = findToSearch(s);
    if(isEmpty(pos) || isTomb(pos) || *table[pos] != s) {
        return(false);
    }
    else {
        return(true);
    }
}

bool HashTable::add(const string &s) {
    int pos;

    if(s.empty() || contains(s)) return(false);

    pos = findToInsert(s);
    if(pos == -1) {
        throw HashTableException();
        return(false);
    }

    table[pos] = new (nothrow) string(s);
    if(table[pos] == nullptr) return(false);
    size++;
    return(true);
}

bool HashTable::add(const char *s) {
    int pos;

    if(!strcmp(s, "") || contains(s)) return(false);

    pos = findToInsert(s);
    if(pos == -1) {
        throw HashTableException();
        return(false);
    }

    table[pos] = new (nothrow) string(s);
    if(table[pos] == nullptr) return(false);
    size++;
    return(true);
}

bool HashTable::remove(const string &s) {
    int pos;

    pos = findToSearch(s);
    if(isAvailable(pos) || *table[pos] != s) {
        return false;
    }

    delete table[pos];
    table[pos] = (string *) 0xFFFF;
    size--;
    return(true);
}

bool HashTable::remove(const char *s) {
    int pos;

    pos = findToSearch(s);
    if(isAvailable(pos) || *table[pos] != s) {
        return false;
    }

    delete table[pos];
    table[pos] = (string *) 0xFFFF;
    size--;
    return(true);
}

string HashTable::print() const {
    string str;
    char buf[128];

    for(int i = 0; i < capacity; i++) {
        if( !isAvailable(i) ) {
            sprintf(buf, "%2d. -%s-\n", i, (*table[i]).c_str());
            str.append(buf);
        }
    }

    sprintf(buf, " --- CAPACITY: %d, SIZE: %d ---\n", capacity, size);
    str.append(buf);
    return(str);
}

void HashTable::myprint() {
    int i;

    for(i = 0; i < capacity; i++) {
        cout << "[ ";
        if(table[i] == nullptr) {
            cout << " ";
        }
        else if(table[i] == (string *) 0xFFFF) {
            cout << "X";
        }
        else {
            cout << *table[i];
        }
        cout << " ]    ";
    }
    cout << endl;
}

HashTable& HashTable::operator= (const HashTable &ht) {
    int i;

    for(i = 0; i < capacity; i++) {
        if(table[i] != nullptr && table[i] != (string *) 0xFFFF) {
            delete table[i];
        }
    }
    delete []table;

    size = ht.size;
    capacity = ht.capacity;
    table = new string*[capacity];

    for(i = 0; i < capacity; i++) {
        if(ht.table[i] == (string *)0xFFFF) {
            this->table[i] = (string *)0xFFFF;
        }
        else if(ht.table[i] == nullptr) {
            this->table[i] = nullptr;
        }
        else {
            this->table[i] = new string(*ht.table[i]);
        }
    }
    return(*this);
}

HashTable& HashTable::operator += (const string& str) {
    add(str);
    return(*this);
}

HashTable& HashTable::operator += (const char* s) {
    add(s);
    return(*this);
}

HashTable& HashTable::operator -= (const string& str) {
    remove(str);
    return(*this);
}

HashTable& HashTable::operator -= (const char* s) {
    remove(s);
    return(*this);
}

HashTable HashTable::operator + (const string& str) const {
    HashTable *new_hash_table = new HashTable(*this);
    new_hash_table->add(str);
    return(*new_hash_table);
}

HashTable HashTable::operator + (const char* s) const {
    HashTable *new_hash_table = new HashTable(*this);
    new_hash_table->add(s);
    return(*new_hash_table);
}

HashTable HashTable::operator - (const string& str) const {
    HashTable *new_hash_table = new HashTable(*this);
    new_hash_table->remove(str);
    return(*new_hash_table);
}

HashTable HashTable::operator - (const char* s) const {
    HashTable *new_hash_table = new HashTable(*this);
    new_hash_table->remove(s);
    return(*new_hash_table);
}

ostream& operator << (ostream &stream, const HashTable &t) {
    return stream << t.print();
}

HashTable::Iterator HashTable::begin() const {
    HashTable::Iterator *iter = new HashTable::Iterator(this, true);
    return(*iter);
}

HashTable::Iterator HashTable::end() const {
    HashTable::Iterator *iter = new HashTable::Iterator(this, false);
    return(*iter);
}