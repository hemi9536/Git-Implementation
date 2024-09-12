// CPP program to implement hashing with chaining
#include<iostream>
#include "hash.hpp"


using namespace std;

HashNode* HashTable::createNode(string key, HashNode* next, int cNum)
{
    HashNode* nw = new HashNode;
    nw->key = key;
    nw->next = next;
    nw->commitNums.push_back(cNum);
    return nw;
}

HashTable::HashTable(int bsize)
{
    this->tableSize = bsize;
    table = new HashNode*[tableSize];
    for(int i = 0; i < tableSize; i++) {
        table[i] = NULL;
    }
}

//function to calculate hash function
unsigned int HashTable::hashFunction(string s)
{
    int sum = 0, index = 0;
    for (string::size_type i = 0; i < s.length(); i++) {
        sum += s[i];
    }
    index = sum % tableSize;
    return index;
}

// TODO Complete this function
//function to search
HashNode* HashTable::searchItem(string key)
{
    //TODO
    HashNode* temp = table[hashFunction(key)];
    while(temp != NULL) {
        if(temp->key == key) {
            return temp;
        }
        temp = temp->next;
    }

    return NULL;
}

//TODO Complete this function
//function to insert
bool HashTable::insertItem(string key, int cNum) {
    HashNode* node = searchItem(key);
    if (node != nullptr) {
        node->commitNums.push_back(cNum);
    } else {
        int hashBucket = hashFunction(key);
        table[hashBucket] = createNode(key, table[hashBucket], cNum);
    }
    return true;
}

// function to display hash table //
/* assume the table size is 5. For each bucket it will show the 
** the string key and the commit number (separated by comma) within parenthesis
** e.g. the key is science and commit numbers are 1 and 4. The key science
** is hashed to position 0. So the print format will be-

0|| science(1,4,)
1|| 
2|| 
3|| 
4|| difficult(3,)-->fun(2,)-->computer(0,)

*/
void HashTable::printTable() {   
    for(int i = 0; i < tableSize; i++) {
        HashNode* temp = table[i];
        cout << i << "|| ";
        while(temp != NULL) {
            if (temp != table[i]) {
                cout << "-->";
            }
            cout << temp->key << "(";
            for(int j = 0; j < int(temp->commitNums.size()); j++) {
                cout << (temp->commitNums).at(j) << ",";
            }
            cout << ")";
            temp = temp->next;
        }
        cout << endl;
    }
}
