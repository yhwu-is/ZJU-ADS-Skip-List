#include "SkipList.h"

template<class K, class V>
SkipList<K, V>::SkipList(K footerKey) : rand(0x12345678) {
    createNode(0, footer);

    footer->key = footerKey;
    this->level = 0;
    //设置头结
    createNode(MAX_LEVEL, header);
    for (int i = 0; i < MAX_LEVEL; i++)
        header->forward[i] = footer;
    nodeCount = 0;
}

template<class K, class V>
SkipList<K, V>::~SkipList() {
    Node<K, V> *p = header;
    Node<K, V> *q;
    while (p != NULL) {
        q = p->forward[0];
        delete p;
        p = q;
    }
    delete p;
}

template<class K, class V>
void SkipList<K, V>::createNode(int level, Node<K, V> *&node) {
    node = new Node<K, V>(NULL, NULL);
    node->forward = new Node<K, V> *[level + 1];  // need to initialize the forward node array
    node->nodeLevel = level;
    assert(node != NULL);
};

template<class K, class V>
void SkipList<K, V>::createNode(int level, Node<K, V> *&node, K key, V value) {
    node = new Node<K, V>(key, value);
    if (level > 0)  // need to initialize the forward node array
        node->forward = new Node<K, V> *[level + 1];
    node->nodeLevel = level;
    assert(node != NULL);
};

template<class K, class V>
Node<K, V> *SkipList<K, V>::search(const K key) const {
    Node<K, V> *node = header;
    for (int i = level; i >= 0; --i) {
        while ((node->forward[i])->key < key) {
            node = *(node->forward + i);
        }
    }
    node = node->forward[0];
    if (node->key == key)
        return node;
    else
        return nullptr;
};

template<class K, class V>
bool SkipList<K, V>::insert(K key, V value) {
    Node<K, V> *update[MAX_LEVEL];

    Node<K, V> *node = header;

    for (int i = level; i >= 0; --i) {
        while ((node->forward[i])->key < key) {
            node = node->forward[i];
        }
        update[i] = node;
    }
    //首个结点插入时，node->forward[0]其实就是footer
    node = node->forward[0];

    //如果key已存在，则直接返回false
    if (node->key == key) {
        return false;
    }

    int nodeLevel = getRandomLevel();

    if (nodeLevel > level) {
        nodeLevel = ++level;
        update[nodeLevel] = header;
    }

    //创建新结点
    Node<K, V> *newNode;
    createNode(nodeLevel, newNode, key, value);

    //调整forward指针
    for (int i = nodeLevel; i >= 0; --i) {
        node = update[i];
        newNode->forward[i] = node->forward[i];
        node->forward[i] = newNode;
    }
    ++nodeCount;

#ifdef DEBUG
    dumpAllNodes();
#endif

    return true;
};

template<class K, class V>
void SkipList<K, V>::dumpAllNodes() {
    Node<K, V> *tmp = header;
    while (tmp->forward[0] != footer) {
        tmp = tmp->forward[0];
        dumpNodeDetail(tmp, tmp->nodeLevel);
        cout << "----------------------------" << endl;
    }
    cout << endl;
}

template<class K, class V>
void SkipList<K, V>::dumpNodeDetail(Node<K, V> *node, int nodeLevel) {
    if (node == nullptr) {
        return;
    }
    cout << "node->key:" << node->key << ",node->value:" << node->value << endl;
    //注意是i<=nodeLevel而不是i<nodeLevel
    for (int i = 0; i <= nodeLevel; ++i) {
        cout << "forward[" << i << "]:" << "key:" << node->forward[i]->key << ",value:" << node->forward[i]->value
             << endl;
    }
}

template<class K, class V>
bool SkipList<K, V>::remove(K key, V &value) {
    Node<K, V> *update[MAX_LEVEL];
    Node<K, V> *node = header;
    for (int i = level; i >= 0; --i) {
        while ((node->forward[i])->key < key) {
            node = node->forward[i];
        }
        update[i] = node;
    }
    node = node->forward[0];
    //如果结点不存在就返回false
    if (node->key != key) {
        return false;
    }

    value = node->value;
    for (int i = 0; i <= level; ++i) {
        if (update[i]->forward[i] != node) {
            break;
        }
        update[i]->forward[i] = node->forward[i];
    }

    //释放结点
    delete node;

    //更新level的值，因为有可能在移除一个结点之后，level值会发生变化，及时移除可避免造成空间浪费
    while (level > 0 && header->forward[level] == footer) {
        --level;
    }

    --nodeCount;

#ifdef DEBUG
    dumpAllNodes();
#endif

    return true;
};

template<class K, class V>
int SkipList<K, V>::getRandomLevel() {
    int level = static_cast<int>(rand.Uniform(MAX_LEVEL));
    if (level == 0) {
        level = 1;
    }
    return level;
}