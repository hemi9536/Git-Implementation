#include <iostream>
#include <fstream>
#include "../code_1/miniGit.hpp"
#include <filesystem>
using namespace std;

/*
 * Purpose; displays a menu with options
 */
void displayMenu()
{
    cout << "Select a numerical option:" << endl;
    cout << "+=====Main Menu=========+" << endl;
    cout << " 1. init" << endl;
    cout << " 2. add" << endl;
    cout << " 3. rm" << endl;
    cout << " 4. commit" << endl;
    cout << " 5. checkout" << endl;
    cout << " 6. search" << endl;
    cout << " 7. quit" << endl;
    cout << "+-----------------------+" << endl;
    cout << "#> ";
}

// We were getting errors with just using stoi, so here is a safer way to get the value
// which catches the error if there is one.
// We referenced this source: https://stackoverflow.com/a/40031878
int toint(string s) {
    try {
        return stoi(s);
    } catch(...) {
    }
    return -1;
}
//add,remove.commit
int main(int argc, char* argv[]) {

    MiniGit m; // = new MiniGit;
    while (true) {
        displayMenu();
        string text;
        getline(cin, text);
        int selection = toint(text.c_str());

        // Init
        if(selection == 1) { //not sure if this is right
            m.init(5);
        }

        // Adding a file
        else if(selection == 2) {
            if (m.isCheckedOut()) {
                cout << "Add is disabled when checked-out." << endl;
            } else {
                string input;
                while (true) {
                    string fileName;
                    cout << "Name of file to be added: ";
                    getline(cin, fileName);
                    ifstream userFile(fileName);
                    if (userFile.good()) {
                        m.add(fileName); //add to end of files
                        break;
                    } else {
                        cout << "Invalid file name." << endl;
                    }
                }
            }
        }

        // Removing a file
        else if(selection == 3) {
            if (m.isCheckedOut()) {
                cout << "Remove is disabled when checked-out." << endl;
            } else {
                string input;
                while (true) {
                    string fileName;
                    cout << "Name of file to be removed: ";
                    getline(cin, fileName);
                    ifstream userFile(fileName);
                    if (userFile.good() && m.searchForFile(fileName)) {
                        m.rm(fileName);
                        break;
                    } else {
                        cout << "Invalid file name." << endl;
                    }
                }
            }
        }

        // Committing
        else if(selection == 4) {
            if (m.isCheckedOut()) {
                cout << "Commit is disabled when checked-out." << endl;
            } else {
                string commitMsg = "";
                cout << " Commit message? (Can't be empty): ";
                getline(cin, commitMsg);
                while (commitMsg == "") {
                    cout << "Invalid response. Try again: ";
                    getline(cin, commitMsg);
                }
                m.commit(commitMsg);
            }
        }

        // Checkout
        else if(selection == 5) {
            string checkoutString;
            cout << "WARNING: Local changes will be lost!" << endl;
            cout << "Enter a commit number to checkout: ";
            while (true) {
                getline(cin, checkoutString);
                int checkoutNum = toint(checkoutString.c_str());
                if (checkoutNum >= 0) {
                    m.checkout(checkoutNum);
                    break;
                }
                cout << "Invalid response. Try again: ";
            }
        }
    
        // Search
        else if(selection == 6) {
            string searchKey;
            cout << "Enter a key message to be searched (One word): ";
            getline(cin, searchKey);
            m.search(searchKey);
        }

        // Quit
        else if(selection == 7) {
            cout << "Goodbye!" << endl;
            break;
        }
    }
   
    return 0;
}