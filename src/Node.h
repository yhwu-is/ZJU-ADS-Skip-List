#ifndef _NODE_H
#define _NODE_H

template<class K, class V>
class SkipList;  // forward declaration

template<class K, class V>
class Node {
public:
    Node() {}
    Node(K k, V v) : key(k), value(v) {}
    ~Node() { delete[] forward; }
    K getKey() const { return key; }
    V getValue() const { return value; }
private:
    K key;
    V value;
    Node<K, V> **forward;
    int nodeLevel;
    friend class SkipList<K, V>;
};

#endif