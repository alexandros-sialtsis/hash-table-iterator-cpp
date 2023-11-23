
#include "ExtHashTable.hpp"
#include <iostream>
#include <string>
#include <vector>

ExtHashTable::ExtHashTable(double upper_bound_ratio, double lower_bound_ratio,
            int size) : HashTable(size) {
    this->upper_bound_ratio = upper_bound_ratio;
    this->lower_bound_ratio = lower_bound_ratio;
}

ExtHashTable::ExtHashTable(const ExtHashTable &t) : HashTable(t) {
    this->upper_bound_ratio = t.upper_bound_ratio;
    this->lower_bound_ratio = t.lower_bound_ratio;
}

void ExtHashTable::rehash() {
    double load_factor;
    vector<string> tmp; 

    if(size == 0) return;
    load_factor = (double) size / (double) capacity;

    if(load_factor >= lower_bound_ratio && load_factor <= upper_bound_ratio) {
        return;
    }

    for(auto iter = this->begin(); iter != this->end(); iter++) {
        tmp.push_back(*iter);
    }

    for(int i = 0; i < capacity; i++) {
        if(table[i] != nullptr && table[i] != (string *)0xFFFF) {
            delete table[i];
        }
    }
    delete []table;  

    if(load_factor < lower_bound_ratio) {
        capacity = capacity / 2;
    }
    else if(load_factor > upper_bound_ratio) {
        capacity = capacity * 2;
    }
    
    table = new (nothrow) string*[capacity];
    if(table == nullptr) {
        throw bad_alloc();
    }
    size = 0;
    for(int i = 0; i < capacity; i++) {
        table[i] = nullptr;
    }

    for(auto iter = tmp.begin(); iter != tmp.end(); iter++) {
        HashTable::add(*iter);
    }

    cout << "--> Size: " << size << ", New capacity: " << capacity << endl;
}

bool ExtHashTable::add(const string &str) {
    rehash();
    return(HashTable::add(str));
}

bool ExtHashTable::add(const char *s) {
    rehash();
    return(HashTable::add(s));
}

bool ExtHashTable::remove(const string &str) {
    if(HashTable::remove(str)) {
        rehash();
        return(true); 
    }
    else {
        return(false);
    }
}

bool ExtHashTable::remove(const char *s) {
    if(HashTable::remove(s)) {
        rehash();
        return(true); 
    }
    else {
        return(false);
    }
}

ExtHashTable& ExtHashTable::operator=(const ExtHashTable &t) {
    HashTable::operator=(t);
    lower_bound_ratio = t.lower_bound_ratio;
    upper_bound_ratio = t.upper_bound_ratio;
    return(*this);
}

ExtHashTable ExtHashTable::operator + (const string& str) const {
    ExtHashTable *new_hash_table = new ExtHashTable(*this);
    new_hash_table->add(str);
    return(*new_hash_table);
}

ExtHashTable ExtHashTable::operator + (const char* s) const {
    ExtHashTable *new_hash_table = new ExtHashTable(*this);
    new_hash_table->add(s);
    return(*new_hash_table);
}

ExtHashTable ExtHashTable::operator - (const string& str) const {
    ExtHashTable *new_hash_table = new ExtHashTable(*this);
    new_hash_table->remove(str);
    return(*new_hash_table);
}

ExtHashTable ExtHashTable::operator - (const char* s) const {
    ExtHashTable *new_hash_table = new ExtHashTable(*this);
    new_hash_table->remove(s);
    return(*new_hash_table);
}

ExtHashTable& ExtHashTable::operator += (const string& str) {
    add(str);
    return(*this);
}

ExtHashTable& ExtHashTable::operator += (const char* s) {
    add(s);
    return(*this);
}

ExtHashTable& ExtHashTable::operator -= (const string& str) {
    remove(str);
    return(*this);
}

ExtHashTable& ExtHashTable::operator -= (const char* s) {
    remove(s);
    return(*this);
}

ExtHashTable ExtHashTable::operator+(const ExtHashTable &t) const {
    ExtHashTable *new_hash = new ExtHashTable(*this);

    for(auto iter = t.begin(); iter != t.end(); iter++) {
        new_hash->add(*iter);
    }
    return(*new_hash);
}

ExtHashTable& ExtHashTable::operator+=(const ExtHashTable &t) {
    for(auto iter = t.begin(); iter != t.end(); iter++) {
        add(*iter);
    }
    return(*this);
}
