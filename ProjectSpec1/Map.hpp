//====================================================
//  Map.hpp                                         //
//  Class: Advance C++                              //
//  Professor: Solomon L. Russell                   //
//  Project#: Spec 1                                //
//  OS Used: MacOS(Xcode)/Windows(Visual Studio)    //
//  Last successful run: 3/12/18                    //
//  Created by Kirby James Fernandez on 3/04/18.    //
//  Copyright © 2018 oc. All rights reserved.       //
//====================================================
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
    };
    
    Node * Head;
    int element_size;
public:
    
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
    void copy(const Map &other);
    
    // inline destroy function
    void destroy();
    
    // virtual deconstructor calls the destroy function
    virtual ~Map();
    
};

// the larger map value should be the result and the smaller map value should
// be looped through until the correct adjustment is being done for better implementation
// strt is a copy of the larger map and it is the beginning
// for iteration of smaller pairs it is determined by these rules:
// If a key appears in exactly one of m1 and m2, then result must contain a pair consisting of that key and its corresponding value.
// If a key appears in both m1 and m2, with the same corresponding value in both, then result must contain a pair with that key and value.
// If there exists a key that appears in both m1 and m2, but with different corresponding values, then this function returns false
// If there is no key like this, the function returns true. Even if the function returns false, result must be constituted as defined by the above rules.
inline bool combine(const Map& m1, const Map& m2, Map& result)
{

    const Map* L;
    const Map* S;
    if (m2.size() <= m1.size())
    {
        S = &m2;
        L = &m1;
    }
    else
    {
        S = &m1;
        L = &m2;
    }

    bool status = true;
    // starts as larger copy of a map
    Map strt(*L);
    for (int n = 0; n < S->size(); n++)
    {
        KeyType k;
        ValueType valS;
        S->get(n, k, valS);
        ValueType vbig;
        
        if (!strt.get(k, vbig))
        {
            strt.insert(k, valS);
        }
        else if (vbig != valS)
        {
            strt.erase(k);
            status = false;
        }
    }
    result.swap(strt);
    return status;
}

// When this function returns, result must contain a copy of all the pairs in m1 whose keys don't appear in m2
// it must not contain any other pairs.
// if m2 is larger than m1 then it should be a basis of the outcome because its key is not in m1 and insert it
// else if m2 is smaller then the outcome should be copied and be removed from its outcome
inline void subtract(const Map& m1, const Map& m2, Map& result)
{
    if(m2.size() >= m1.size())
    {
        // If m1 is smaller, if an item in m1 should be in the result because
        // its key is not in m2, add it
        
        Map strt;
        for (int n = 0; n < m1.size(); n++)
        {
            KeyType k;
            ValueType val;
            m1.get(n, k, val);
            if (!m2.contains(k))
                strt.insert(k, val);
        }
        result.swap(strt);
    }
    else
    {
        Map strt(m1);
        for (int n = 0; n < m2.size(); n++)
        {
            KeyType k;
            ValueType v;
            m2.get(n, k, v);
            strt.erase(k);
        }
        result.swap(strt);
    }
}

#include <stdio.h> // xcode generated include

#endif // end


