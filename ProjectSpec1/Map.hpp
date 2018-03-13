#pragma once
#ifndef _Map_Implementing_Doubly_Linked_List
#define _Map_Implementing_Doubly_Linked_List
#include <string>

typedef std::string KeyType;
typedef double ValueType;

class Map
{
private:
    struct Node
    {
        Node* next;
        Node* prev;
        
        KeyType key;
        ValueType value;
        
        Node(Node * nptr, Node * pptr, KeyType k, ValueType v);
    };
public:
    Node * Head;
    int element_size;
    
    // Create an empty map (i.e., one with no key/value pairs)
    Map();
    
    // Copy Constructor
    Map(const Map& Other);
    
    // Assignment Operator
    Map &operator= (const Map& Other);
    
    // Return true if the map is empty, otherwise false.
    bool empty() const;
    
    // Return the number of key/value pairs in the map.
    int size() const;
    
    // If key is not equal to any key currently in the map, and if the
    // key/value pair can be added to the map, then do so and return true.
    // Otherwise, make no change to the map and return false (indicating
    // that either the key is already in the map).
    bool insert(const KeyType& key, const ValueType& value);
    
    // If key is equal to a key currently in the map, then make that key no
    // longer map to the value it currently maps to, but instead map to
    // the value of the second parameter; return true in this case.
    // Otherwise, make no change to the map and return false.
    bool update(const KeyType& key, const ValueType& value);
    
    // If key is equal to a key currently in the map, then make that key no
    // longer map to the value it currently maps to, but instead map to
    // the value of the second parameter; return true in this case.
    // If key is not equal to any key currently in the map then add it and
    // return true. In fact this function always returns true.
    bool insertOrUpdate(const KeyType& key, const ValueType& value);

    // If key is equal to a key currently in the map, remove the key/value
    // pair with that key from the map and return true.  Otherwise, make
    // no change to the map and return false.
    bool erase(const KeyType& key);
   
    // Return true if key is equal to a key currently in the map, otherwise
    // false.
    bool contains(const KeyType& key) const;
    
    // If key is equal to a key currently in the map, set value to the
    // value in the map that that key maps to, and return true.  Otherwise,
    // make no change to the value parameter of this function and return
    // false.
    bool get(const KeyType& key, ValueType& value) const;
   
    // If 0 <= i < size(), copy into the key and value parameters the
    // key and value of one of the key/value pairs in the map and return
    // true.  Otherwise, leave the key and value parameters unchanged and
    // return false.  (See below for details about this function.)
    bool get(int i, KeyType& key, ValueType& value) const;
    
    // Exchange the contents of this map with the other one.
    void swap(Map& other);
    
    // inline copy function
    void copy(const Map &New);
    
    // inline destroy function
    void destroy();
    
    // virtual deconstructor calls the destroy function
    virtual ~Map();
    
};

inline bool combine(const Map& m1, const Map& m2, Map& result)
{
    // to be fix
    return false;
}

//
inline void subtract(const Map& m1, const Map& m2, Map& result)
{
    // to be fix
}
#include <stdio.h>

#endif // 

