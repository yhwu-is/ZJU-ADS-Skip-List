#include <iostream>
#include "src/SkipList.h"
using namespace std;

int main(int argc, char* argv[])
{
    cout << "Start skiplist test..." << endl;

    //给一个最大的整型值
    SkipList<int> skipList(0x7fffffff);

    int length = 10;

    for (int i = 1; i <= length; i++)
        skipList.insert(i);

    cout << "The number of elements in skiplist is:" << skipList.size() << endl;

    if (skipList.size() != length)
        cout << "insert failure." << endl;
    else
        cout << "insert success." << endl;

    //测试查找
    int data = 9;
    Node<int> *searchResult = skipList.search(data);
    if (searchResult != nullptr) {
        data = searchResult->getData();
        cout << "search result for data " << data << endl;
    } else {
        cout << "search failure for data " << data << endl;
    }

    //测试移除,测试不通过
    data = 6;
    cout << endl << "start to remove" << endl;
    bool removeResult = skipList.remove(data);
    if (removeResult) {
        cout << "removed node whose data is " << data << endl;
    } else {
        cout << "removed failure" << endl;
    }
    return 0;
}