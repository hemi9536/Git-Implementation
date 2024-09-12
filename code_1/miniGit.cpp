#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
using namespace std;

#include <filesystem>
namespace fs = std::filesystem;

#include "miniGit.hpp"
#include <vector>

MiniGit::MiniGit() {
    fs::remove_all(".minigit"); // removes directory and contents
    fs::create_directory(".minigit"); // creates new directory

    commitHead = nullptr;
    commits = 0;
    curCommit = -1;
    ht = nullptr;
}

MiniGit::~MiniGit() {   
    // Any postprocessing that may be required
    fs::remove_all(".minigit");
}

void MiniGit::init(int hashtablesize) {
    if (ht == nullptr) {
        ht = new HashTable(hashtablesize);
        createNewCommitNode();
    }
}

BranchNode* MiniGit::createNewCommitNode() {

    BranchNode *newBranch = new BranchNode;
    newBranch->commitID = 0;
    newBranch->next = nullptr;
    newBranch->previous = nullptr;
    newBranch->fileHead = nullptr;

    BranchNode *curBranch = currCommit(commits);
    if (curBranch != nullptr)
    {
        newBranch->commitID = curBranch->commitID + 1;

        FileNode *lastNode = nullptr;
        for (FileNode *curFileNode = curBranch->fileHead; curFileNode; curFileNode = curFileNode->next)
        {
            // Create a copy of our cur file node to use with our new branch
            FileNode *newFileNode = createNewFileNode(curFileNode->name);
            newFileNode->version = curFileNode->version;

            // Add the new file node to the end of our new branch
            if (newBranch->fileHead == nullptr) {
                newBranch->fileHead = newFileNode;
            } else {
                lastNode->next = newFileNode;
            }
            lastNode = newFileNode;
        }

        // Add the next branch and advance to it
        curBranch->next = newBranch;
        newBranch->previous = curBranch;

        curCommit = commits;
        commits++;

    } else {
        commitHead = newBranch;
        commits = 0;
        curCommit = -1;
    }

    return curBranch;
}

FileNode* MiniGit::createNewFileNode(string name) {
    FileNode* nf = new FileNode;
    nf->name = name;
    nf->next = nullptr;
    nf->version = -1;
    return nf;
}

BranchNode* MiniGit::currCommit(int numCommits) {
    BranchNode* temp = this->commitHead;
    for(int i = 0; temp && (i < numCommits); i++) {
        temp = temp->next;
    }
    return temp;
}

FileNode* MiniGit::searchForFile(string key) {
    FileNode* loop = (currCommit(this->commits))->fileHead;
    while(loop != NULL) {
        if(loop->name == key) {
            return loop;
        }
        loop = loop->next;
    }
    return loop;
}

void MiniGit::add(string fileName) {

    // Don't allow add if are checked out.
    if (isCheckedOut()) {
        return;
    }

    if(searchForFile(fileName) == NULL) {
        FileNode* newF = createNewFileNode(fileName);
        if(currCommit(this->commits)->fileHead == NULL) {
            currCommit(this->commits)->fileHead = newF;
        }
        else {
            FileNode* loop = currCommit(this->commits)->fileHead;
            while(loop->next != NULL) {
                loop = loop->next;
            }
            loop->next = newF;
        }
    }
}

void MiniGit::rm(string fileName) {

    // Don't allow remove if are checked out.
    if (isCheckedOut()) {
        return;
    }

    if(searchForFile(fileName) != NULL) {
        FileNode* crawler = currCommit(this->commits)->fileHead;
        FileNode* trail = NULL;
        while(crawler->name != fileName) {
            crawler = crawler->next;
        }
        if(trail == NULL) {
            currCommit(this->commits)->fileHead = crawler->next;
            delete crawler;
        }
        else {
            trail->next = crawler->next;
            delete crawler;
        }
    }
}

void MiniGit::search(string key) {
    HashNode* temp = ht->searchItem(key);
    if (temp != nullptr) {
        cout << "CommitIDs with the search key " << key << ":" << endl;
        for(int i = 0; i < int(temp->commitNums.size()); i++) {
            cout << temp->commitNums.at(i) << endl;
        }
        cout << endl;
    } else {
        cout << "Could not find that search key." << endl;
    }
}

void MiniGit::printSearchTable()
{
    ht->printTable(); // is this what it is for?
}

bool MiniGit::isCheckedOut() {
    return curCommit < (commits - 1);
}

char* readFile(string fileName, streamsize &size) {
    char *data = nullptr;
    size = -1;

    // Open the file with ios::binary (we could maybe do text?) and ios::ate (so the seek pointer is at the end)
    // How to get size of file: https://stackoverflow.com/a/13394183
    ifstream file(fileName, ios::binary | ios::ate);

    // Make sure the file exists.
    if (file.good()) {

        // Since we opened the file with ios::ate, the seek pointer is at the end.
        // We can read the seek pointer to get the size of the file.
        size = file.tellg();

        // Reset the seek pointer back to the beginning of the file before we read it.
        file.seekg(0, ios::beg);

        // Allocate a buffer large enough to hold the whole file.
        data = new char[size];

        // read the entire file into the buffer.
        if (!file.read(data, size)) {

            // If it failed, just delete it.
            delete[] data;
            data = nullptr;
            size = -1;
        }
    }
    return data;
}

bool writeFile(string fileName, char *data, streamsize size) {
    // Open the file with ios::binary (we could maybe do text?)
    ofstream file(fileName, ios::binary);

    // If the file seems good, then write the entire buffer to it.
    if (file.good()) {
        file.write(data, size);
        return true;
    }
    return false;
}

string getVersionFileName(string userFileName, int version) {
    
    // Convert the version number to a 2 digit string with a leading zero.
    // Where we found what decimal in the sprintf to use: https://stackoverflow.com/a/6117033
    char versionString[10];
    sprintf(versionString, "%02d", version);

    // Figure out where the file extension starts in our file name.
    uint indexOfLastDot = userFileName.find_last_of('.');

    // Get the name part and the extension part, and then stick the version in between them.
    string name = userFileName.substr(0, indexOfLastDot);
    string extension = userFileName.substr(indexOfLastDot);
    return ".minigit/" + name + "_" + versionString + extension;
}

string MiniGit::commit(string msg) {
    // Don't allow commit if are checked out.
    if (isCheckedOut()) {
        return "error: checked out";
    }

    BranchNode *curBranch = currCommit(commits);
    curBranch->commitMessage = msg;

    // Adding each word to the hash table
    stringstream str(msg);
    string token;
    while (getline(str, token, ' ')) {
        ht->insertItem(token, commits);
    }

    // Loop through the SLL file nodes
    for (FileNode *crawler = curBranch->fileHead; crawler != nullptr; crawler = crawler->next) {

        // Read the user file
        // streamsize is just a size type that the compiler likes
        streamsize userSize = -1;
        char* userData = readFile(crawler->name, userSize);

        streamsize versionSize = -1;
        char *versionData = nullptr;
        if (crawler->version >= 0) {
            // read the version file. if it doesn't exist, versionSize will be -1
            versionData = readFile(getVersionFileName(crawler->name, crawler->version), versionSize);
        }

        // check if the version file doesn't exist or is different than the user file
        if ((userSize != versionSize) || (0 != memcmp(userData, versionData, userSize))) {

            // write the data from the user file to the new version file
            crawler->version++;
            writeFile(getVersionFileName(crawler->name, crawler->version), userData, userSize);
        }

        // Free the stuff we read from the files
        delete[] userData;
        delete[] versionData;
    }

    // Create the new DLL node
    createNewCommitNode();

    return to_string(commits - 1); // should return the commitID of the commited DLL node
}

bool MiniGit::checkout(int commitID) {

    // Checking for valid commitID
    if ((commitID < 0) || (commitID >= commits)) {
        return false;
    }

    // Get the BranchNode the user specified.
    BranchNode* commitNode = currCommit(commitID);
    
    // Exit if we didn't find it.
    if (commitNode == nullptr) {
        return false;
    }

    curCommit = commitID;

    bool good = true;

    // Loop over ever file we need to copy.
    for (FileNode *fileNode = commitNode->fileHead; fileNode; fileNode = fileNode->next) {

        // Read the .minigit file for this version.
        string versionFileName = getVersionFileName(fileNode->name, fileNode->version);
        streamsize size = -1;
        char *data = readFile(versionFileName, size);

        // Write the data to the user file, while also checking if you can even write it
        if ((data == nullptr) || !writeFile(fileNode->name, data, size)) {
            good = false;
        }

        // Delete the file buffer.
        delete[] data;
    }

    return good;
}

/* We used this for testing
void MiniGit::printAll() {
    cout << endl;
    for (BranchNode *crawlerDLL = commitHead; crawlerDLL != nullptr; crawlerDLL = crawlerDLL->next) {
        printf("ID:%d, Msg:%s\n", crawlerDLL->commitID, crawlerDLL->commitMessage.c_str());
        for (FileNode *crawlerSLL = crawlerDLL->fileHead; crawlerSLL; crawlerSLL = crawlerSLL->next) {
            printf("   Name:%s, Ver:%d\n", crawlerSLL->name.c_str(), crawlerSLL->version);
        }
    }

    cout << endl;
    ht->printTable();

    cout << endl;
} */