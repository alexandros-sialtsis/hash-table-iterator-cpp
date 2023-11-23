
#include "HashTable.hpp"
#include "ExtHashTable.hpp"
#include <iostream>
#include <string>

using namespace std;

int main() {
    ExtHashTable hash(0.5, 0.125, 4);

    hash.myprint();

    hash.add("hello");
    hash.myprint();

    hash.add("bitch");
    hash.myprint();

    hash.add("fuck");
    hash.myprint();

    hash.add("off");
    hash.myprint();

    hash.add("why");
    hash.myprint();

    hash.add("ti thes?");
    hash.myprint();

    hash.add("ela mpro");
    hash.myprint();

    hash.add("omitsi");
    hash.myprint();

    hash.add("coco re tchee");
    hash.myprint();

}