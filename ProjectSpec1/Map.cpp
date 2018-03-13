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
Map::Node::Node(Node * nptr, Node * pptr, KeyType k, ValueType v) : next(nptr), prev(pptr), key(k), value(v)
{
    
}

// Copy Constructor
Map::Map(const Map &New)
{
    if (this == &New)
    {
        exit(0); // self copy is not allowed
    }
    copy(New);
}

// Assignment Operator
Map& Map::operator= (const Map &New)
{
    if(this != &New)
    {
        destroy();
        copy(New);
    }
    return *this;
}

Map::Map()
{
    Head = NULL;
    element_size = 0;
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
    Node* n = new Node(nullptr, nullptr, "-", 0.0);
    
    if (!contains(key)) {
        if (empty())
        {
            n->next = NULL;
            n->prev = NULL;
            
            Head = n;
        }
        else
        {
            n->next = Head;
            n->prev = NULL;
            Head->prev = n;
            Head = n;
        }
        
        n->key = key;
        n->value = value;
        element_size++;
        
        return true;
    }
    else {
        cerr << "Key is already in the map." << endl;
        return false;
    }
}

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
    
    for (n = Head; n != NULL; n = n->next)
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
    Node* n = new Node(nullptr, nullptr, "-", 0.0);
    
    if (contains(key))
    {
        value = n->value;
        
        return true;
    }
    return false;
}

bool Map::get(int i, KeyType& key, ValueType& value) const
{
    Node* n;

    if (i >= 0  && i < size())
    {
        for (n = Head; n != NULL; n = n->next)
        {
            key = n->key;
            value = n->value;
        }
       return true;
    }
    return false;
}

void Map::swap(Map& other)
{
    Map temp = other;
    other.copy(*this);
    this->copy(temp);
}

// copy function
void Map::copy(const Map &New)
{
    Node* PresentNext = New.Head->next;
    Node* PresentPrev = New.Head->prev;
    Node* IterateNext = Head = new Node(nullptr, nullptr, "-", 0.0);
    Node* IteratePrev = Head = new Node(nullptr, nullptr, "-", 0.0);
    while (PresentNext != nullptr && PresentPrev != nullptr)
    {
        // create a copy of node pointed to by current and link up
        IterateNext->next = new Node(nullptr, nullptr, PresentNext->key, PresentNext->value);
        IteratePrev->prev = new Node(nullptr, nullptr, PresentPrev->key, PresentPrev->value);
        
        // update pointers
        PresentNext = PresentNext->next;
        PresentPrev = PresentPrev->prev;
        IterateNext = IterateNext->next;
        IteratePrev = IteratePrev->prev;
    }
}

// destroy function
void Map::destroy()
{
    Node* Present = Head;
    while (Present != nullptr)
    {
        Node* save = Present;
        Present = Present->next;
        delete save;
    }
}

// virtual deconstructor
Map::~Map()
{
    destroy();
}

void test()
{
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
    m.erase("A");
    m.insertOrUpdate("A", 20);
    m.update("A", 30);
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
    m.erase("A");
    cout << all << total << endl;
}

