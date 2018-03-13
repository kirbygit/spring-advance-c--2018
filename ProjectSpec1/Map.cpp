//====================================================
//  Map.cpp                                         //
//  Class: Advance C++                              //
//  Professor: Solomon L. Russell                   //
//  Project#: Spec 1                                //
//  OS Used: MacOS(Xcode)/Windows(Visual Studio)    //
//  Last successful run: 3/12/18                    //
//  Created by Kirby James Fernandez on 3/04/18.    //
//  Copyright © 2018 oc. All rights reserved.       //
//====================================================
#include "Map.hpp"
#include <type_traits>
#include <iostream>
#include <cassert>

#define CHECKTYPE(f, t) { auto p = (t)(f); (void)p; }

static_assert(std::is_default_constructible<Map>::value,
              "Map must be default-constructible.");
static_assert(std::is_copy_constructible<Map>::value,
              "Map must be copy-constructible.");

using namespace std;

void ThisFunctionWillNeverBeCalled()
{
    CHECKTYPE(&Map::operator=,      Map& (Map::*)(const Map&));
    CHECKTYPE(&Map::empty,          bool (Map::*)() const);
    CHECKTYPE(&Map::size,           int  (Map::*)() const);
    CHECKTYPE(&Map::insert,         bool (Map::*)(const KeyType&, const ValueType&));
    CHECKTYPE(&Map::update,         bool (Map::*)(const KeyType&, const ValueType&));
    CHECKTYPE(&Map::insertOrUpdate, bool (Map::*)(const KeyType&, const ValueType&));
    CHECKTYPE(&Map::erase,          bool (Map::*)(const KeyType&));
    CHECKTYPE(&Map::contains,       bool (Map::*)(const KeyType&) const);
    CHECKTYPE(&Map::get,            bool (Map::*)(const KeyType&, ValueType&) const);
    CHECKTYPE(&Map::get,            bool (Map::*)(int, KeyType&, ValueType&) const);
    CHECKTYPE(&Map::swap,           void (Map::*)(Map&));
    
    CHECKTYPE(combine,              bool(*)(const Map&, const Map&, Map&));
    CHECKTYPE(subtract,             void(*)(const Map&, const Map&, Map&));
}

// Constructor initialization list
Map::Map() : element_size(0)
{
    // create dummy node
    Head = new Node;
    Head->next = Head;
    Head->prev = Head;
}

// Copy Constructor
Map::Map(const Map& other) : element_size(other.element_size)
{
    // Create a dummy node and set the previous node to its Head
    Head = new Node;
    Node* prev = Head;
    
    // this iteration will assign Head next of the copied one of the previous node
    for (Node* ptr = other.Head->next; ptr != other.Head; ptr = ptr->next)
    {
        // Create a copy of the node p points to
        Node* newPtr = new Node;
        newPtr->key = ptr->key;
        newPtr->value = ptr->value;
        
        // Connect the new node to the previous one
        newPtr->prev = prev;
        prev->next = newPtr;
        
        // Reset prev to last node created
        prev = newPtr;
    }
    
    // Connect last node created to m_head
    Head->prev = prev;
    prev->next = Head;
}

// Assignment Operator
Map& Map::operator=(const Map& other)
{
    if (this != &other)
    {
        Map temp(other);
        swap(temp);
    }
    return *this;
}

bool Map::empty() const
{
    if (Head == NULL)
    {
        return true;
    }
    return false;
}

int Map::size() const
{
    return element_size;
}

bool Map::insert(const KeyType& key, const ValueType& value)
{
    Node* n = new Node;
    
    if (!contains(key)) {
        n->key = key;
        n->value = value;
        
        n->prev = Head->prev;
        n->prev->next = n;
        
        n->next = Head;
        Head->prev = n;
        element_size++;
        
        return true;
    }
    else {
        cerr << "Key is already in the map." << endl;
        return false;
    }
}

// iteration is checking if current key is the actual key in the map
// then set its  value to its current value in the map and return true
// otherwise return false
bool Map::update(const KeyType& key, const ValueType& value)
{
    Node* n;
    
    for (n = Head; n != NULL; n = n->next)
    {
        if (n->key == key) {
            n->value = value;
            return true;
        }
    }
    return false;
}

// check condition if the key is in the map
// update its key and value pair and return true
// otherwise if it doesn't contain the key then insert key and value pair and still return true
// since this function always return true
bool Map::insertOrUpdate(const KeyType& key, const ValueType& value)
{
    if (contains(key))
    {
        update(key, value);
        return true;
    }
    else
    {
        insert(key, value);
        return true;
    }
}

// iterate each until current key is found in its map
// check the previous node if its not null and set its Head to the next node
// else if the next node is null then set next and previous to null
// lastly set node previous to next to next and next to previous to previous
// delete the node and decrement and return true otherwise false
bool Map::erase(const KeyType& key)
{
    Node* n;

    for (n = Head; n != NULL; n = n->next)
    {
        if (n->key == key) {
            if (n->prev == NULL) {
                Head = n->next;
            }
            else if (n->next == NULL) {
                n->prev->next = NULL;
            }
            else {
                n->prev->next = n->next;
                n->next->prev = n->prev;
            }
            
            delete n;
            element_size--;
            return true;
        }
    }
    return false;
}

bool Map::contains(const KeyType& key) const
{
    Node* n;
    
    // if current key is the key in the map then return true otherwise false
    for (n = Head->next; n != Head; n = n->next)
    {
        if (n->key == key)
        {
            return true;
        }
    }
    return false;
}

bool Map::get(const KeyType& key, ValueType& value) const
{
    Node* n = new Node;
    // key found in its map set value to current value and return true otherwise return false
    if (contains(key))
    {
        value = n->value;
        return true;
    }
    return false;
}

//
bool Map::get(int i, KeyType& key, ValueType& value) const
{
    if (i < 0 || i >= element_size) // if this condition is being met it returns false
    {
        return false;
    }
    
    Node* n;
    // track the position near the head
    if (i < element_size / 2)
    {
        n = Head->next;
        for (int j = 0; j != i; j++)
            n = n->next;
    }
    // track the position near its tail
    else
    {
        n = Head->prev;
        for (int j = element_size - 1; j != i; j--)
        {
            n = n->prev;
        }
    }
    
    key = n->key;
    value = n->value;
    return true;
}

void Map::swap(Map& other)
{
    // swap its pointers
    Node* temp = Head;
    Head = other.Head;
    other.Head = temp;
    
    // swap its sizes
    int t = element_size;
    element_size = other.element_size;
    other.element_size = t;
}

// virtual deconstructor
Map::~Map()
{
    // completely deallocate all dummy and non-dummy nodes being allocated
    for (Node* ptr = Head->prev; element_size >= 0; element_size--)
    {
        Node* toDelete = ptr;
        ptr = ptr->prev;
        delete toDelete;
    }
}

void test()
{
    // default constructor
    Map m;
    assert(m.insert("Fred", 123));
    assert(m.insert("Ethel", 456));
    assert(m.size() == 2);
    double d = 42;
    assert(m.get("Fred", d) && d == 123);
    d = 42;
    string s1;
    assert(m.get(0, s1, d) &&
           ((s1 == "Fred"  &&  d == 123) || (s1 == "Ethel"  &&  d == 456)));
    string s2;
    assert(m.get(1, s2, d) && s1 != s2 &&
           ((s2 == "Fred"  &&  d == 123) || (s2 == "Ethel"  &&  d == 456)));
}

int main()
{
    
    Map m;
    m.insert("A", 10);
    m.insert("B", 44);
    m.insert("C", 10);
    string all;
    double total = 0;
    for (int n = 0; n < m.size(); n++)
    {
        string k;
        double v;
        m.get(n, k, v);
        all += k;
        total += v;
    }
    cout << all << total <<endl;
    
    Map gpas;
    gpas.insert("Fred", 2.956);
    gpas.insert("Ethel", 3.538);
    double v;
    string k1;
    assert(gpas.get(1, k1, v) && (k1 == "Fred" || k1 == "Ethel"));
    string k2;
    assert(gpas.get(1, k2, v) && k2 == k1);
    
    Map gpas2;
    gpas2.insert("Fred", 2.956);
    assert(!gpas2.contains(""));
    gpas2.insert("Ethel", 3.538);
    gpas2.insert("", 4.000);
    gpas2.insert("Lucy", 2.956);
    assert(gpas2.contains(""));
    gpas2.erase("Fred");
    assert(gpas2.size() == 3 && gpas2.contains("Lucy") && gpas2.contains("Ethel") &&
           gpas2.contains(""));
    
    Map m1;
    m1.insert("Fred", 2.956);
    Map m2;
    m2.insert("Ethel", 3.538);
    m2.insert("Lucy", 2.956);
    m1.swap(m2);
    assert(m1.size() == 2 && m1.contains("Ethel") && m1.contains("Lucy") &&
           m2.size() == 1 && m2.contains("Fred"));
}


