#ifndef TEST_H__
#define TEST_H__
#include <string>
#include <iostream>
#include <fstream>
#include <array>
#include "../code_1/hash.hpp"
#include "../code_1/miniGit.hpp"

using namespace std;

// TODO add detailed explanation on what the function should do
std::string exec(const char* cmd);

// Takes a text file and turns it into a single string
// It first checks if the file exists
// If it does, then it opens the file and reads it line by line while adding each line to a string
// Finally, it returns the text file as a single string
std::string readFileIntoString(std::string fileName);

// Tests the insert function for hash.cpp
// First, it creates a new hash table
// It then takes each word in the array, hashes them, and then puts them into the table
// Finally, it prints out the table
string test_insert(string arr[], int len, int tabSize);

// Tests the search function for hash.cpp
// First, it creates a new hash table
// It then fills in the hash table with the word array
// Next, it checks to see if the any of the key words are in the hash table
// If they are, it adds them to an output string
// Finally, it returns the string of found words
string test_search(string arr[], int len, int tabSize, string keys[], int numKeys);

#endif // TEST_H__