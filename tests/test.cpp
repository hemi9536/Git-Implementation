// If you change anything in this file, your changes will be ignored 
// in your homework submission.
// Chekout TEST_F functions bellow to learn what is being tested.
#include <gtest/gtest.h>
// #include "../code_1/<HEADER FILE>.hpp"

#include <fstream>
#include <iostream>
#include <string>
#include <cmath>
#include <array>
#include <vector>
#include "test_helpers.hpp"

using namespace std;

class test_x : public ::testing::Test {
protected:
	// This function runs only once before any TEST_F function
	static void SetUpTestCase(){
		std::ofstream outgrade("./total_grade.txt");
		if(outgrade.is_open()){
			outgrade.clear();
			outgrade << (int)0;
			outgrade.close();
		}
	}

	// This function runs after all TEST_F functions have been executed
	static void TearDownTestCase(){
		std::ofstream outgrade("./total_grade.txt");
		if(outgrade.is_open()){
			outgrade.clear();
			outgrade << (int)std::ceil(100*total_grade/max_grade);
			outgrade.close();
			std::cout << "Total Grade is : " << (int)std::ceil(100*total_grade/max_grade) << std::endl;
		}
	}

	void add_points_to_grade(double points){
		if(!::testing::Test::HasFailure()){
			total_grade += points;
		}
	}

	// this function runs before every TEST_F function
	void SetUp() override {}

	// this function runs after every TEST_F function
	void TearDown() override {
		std::ofstream outgrade("./total_grade.txt");
		if(outgrade.is_open()){
			outgrade.clear();
			outgrade << (int)std::ceil(100*total_grade/max_grade);
			outgrade.close();
		}
	}
	
	static double total_grade;
	static double max_grade;
};

double test_x::total_grade = 0;
double test_x::max_grade = 100;



/////////////////////////////////////////
// Tests start here
/////////////////////////////////////////

TEST_F(test_x, TestInsert){
    
    string result;
    string expected;
       
	   
    int len0 = 5;
    string commits0[] = {"computer", "science", "fun", "difficult", "science"};
    int tabSize0 = 5;
    result = test_insert(commits0, len0, tabSize0);
    expected = "0|| science(1,4,)\n1|| \n2|| \n3|| \n4|| difficult(3,)-->fun(2,)-->computer(0,)\n";
    ASSERT_EQ(expected, result);
    add_points_to_grade(10);

	// Start of our custom made tests for insert.
	int len1 = 5;
    string commits1[] = {"test", "first", "second", "third", "final"};
    int tabSize1 = 5;
    result = test_insert(commits1, len1, tabSize1);
    expected = "0|| \n1|| second(2,)\n2|| final(4,)-->first(1,)\n3|| test(0,)\n4|| third(3,)\n";
    ASSERT_EQ(expected, result);
    add_points_to_grade(10);

	int len2 = 7;
    string commits2[] = {"comments", "tests", "fix", "type", "readme", "bug", "error"};
    int tabSize2 = 7;
    result = test_insert(commits2, len2, tabSize2);
    expected = "0|| \n1|| error(6,)\n2|| type(3,)-->comments(0,)\n3|| bug(5,)-->tests(1,)\n4|| \n5|| fix(2,)\n6|| readme(4,)\n";
    ASSERT_EQ(expected, result);
    add_points_to_grade(10);

	int len3 = 6;
    string commits3[] = {"final", "small", "changes", "to", "hash", "miniGit"};
    int tabSize3 = 6;
    result = test_insert(commits3, len3, tabSize3);
    expected = "0|| hash(4,)-->final(0,)\n1|| miniGit(5,)\n2|| \n3|| changes(2,)-->small(1,)\n4|| \n5|| to(3,)\n";
    ASSERT_EQ(expected, result);
    add_points_to_grade(10);

	int len4 = 0;
    string commits4[] = {};
    int tabSize4 = 0;
    result = test_insert(commits4, len4, tabSize4);
    expected = "";
    ASSERT_EQ(expected, result);
    add_points_to_grade(10);
}

//TODO add more tests for insert. Design tests for search from hashTable.

// Custom tests for the hash search function.
TEST_F(test_x, TestSearch){
    
    string result;
    string expected;
       

    int len0 = 5;
    string commits0[] = {"computer", "science", "fun", "difficult", "science"};
	string findWords0[] = {"computer", "first", "fun"};
    int tabSize0 = 5;
	int numKeys0 = 3;
    result = test_search(commits0, len0, tabSize0, findWords0, numKeys0);
    expected = "computer,fun,";
    ASSERT_EQ(expected, result);
    add_points_to_grade(10);

	int len1 = 5;
    string commits1[] = {"test", "first", "second", "third", "final"};
	string findWords1[] = {"test", "computer", "science", "final", "difficult"};
    int tabSize1 = 5;
	int numKeys1 = 5;
    result = test_search(commits1, len1, tabSize1, findWords1, numKeys1);
    expected = "test,final,";
    ASSERT_EQ(expected, result);
    add_points_to_grade(10);

	int len2 = 7;
    string commits2[] = {"comments", "tests", "fix", "type", "readme", "bug", "error"};
	string findWords2[] = {"errors", "comments", "tests", "fun"};
    int tabSize2 = 7;
	int numKeys2 = 4;
    result = test_search(commits2, len2, tabSize2, findWords2, numKeys2);
    expected = "comments,tests,";
    ASSERT_EQ(expected, result);
    add_points_to_grade(10);

	int len3 = 6;
    string commits3[] = {"final", "small", "changes", "to", "hash", "miniGit"};
	string findWords3[] = {"hashing", "ending", "final", "small", "finish"};
    int tabSize3 = 6;
	int numKeys3 = 5;
    result = test_search(commits3, len3, tabSize3, findWords3, numKeys3);
    expected = "final,small,";
    ASSERT_EQ(expected, result);
    add_points_to_grade(10);

	int len4 = 1;
    string commits4[] = {"commiting"};
	string findWords4[] = {"computer", "fast", "fun", "science"};
    int tabSize4 = 1;
	int numKeys4 = 4;
    result = test_search(commits4, len4, tabSize4, findWords4, numKeys4);
    expected = "";
    ASSERT_EQ(expected, result);
    add_points_to_grade(10);
}