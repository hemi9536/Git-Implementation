#pragma once

#include<iostream>
#include<vector>
#include "hash.hpp"

using namespace std;


enum GITOP {init, add, rm, commit, checkout, status};

struct FileNode {
    string name; // name of file user inputs
    int version; // what version of the file it is
    FileNode* next; // points to next file in list
};

// DLL, each one represents a different commit
struct BranchNode {
    int commitID; // which commit you are on
    string commitMessage; // message put after committing
    BranchNode* next; // points to next commit
    BranchNode* previous; // points to last commit
    FileNode* fileHead; // headpointer to SLL of files  
};

//*** You can add helper functions if you desire ***//
class MiniGit {
private:    
    BranchNode* commitHead;
    int commits;
    int curCommit;
    HashTable* ht;
    
   

public:
    MiniGit();
    ~MiniGit();

    void init(int hashtablesize);
    void add(string fileName);
    void rm(string fileName);
    void search(string key);
    string commit(string msg);
    bool checkout(int commitID);
    void printSearchTable();
    bool isCheckedOut();
    
    BranchNode* currCommit(int numCommits);

    BranchNode* createNewCommitNode();

    FileNode* createNewFileNode(string name);

    FileNode* searchForFile(string key);
    void printAll();
};
