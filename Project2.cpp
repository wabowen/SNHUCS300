//============================================================================
// Name        : BinarySearchTree.cpp
// Author      : Andrew Bowen
// Version     : 1.0
// Copyright   : Copyright © 2017 SNHU COCE
// Description : Binary Search Tree implementation
//============================================================================

#include <iostream>
#include <time.h>
#include <iostream> //cout cin etc
#include <fstream> //files etc
#include <string> //strings
#include <vector> //vectors
#include <algorithm> //for counting and some stuff
using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

//forward declarations


// define a structure to hold course information
struct Course {
    string courseNumber; // class identifier ie CSCI100
    string courseName; // class name ie Intro to C++
    vector<string> prereqs; //vector of the n (0 or more) possible prereq course numbers, in string format
    //ie COMSCI101, COMSCI102 if they are prerequisites for COMSCI300
};

// Internal structure for tree node
struct Node {
    Course course;
    Node *left;
    Node *right;

    // default constructor
    Node() {
        left = nullptr;
        right = nullptr;
    }

    // initialize with a Course (not used?)
    Node(Course aCourse) :
            Node(){
        course = aCourse;
    }
};

//============================================================================
// Binary Search Tree class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a binary search tree
 */
class BinarySearchTree{
private:
    Node* root;
    //void addNode(Node* node, Course course);
    void InOrder(Node* node);

public:
    BinarySearchTree();
    virtual ~BinarySearchTree();
    void InOrder(); // will use this to print the courses in the bst in "order"
    void Insert(Course course); // will use this to insert during the loading ?
    void Search(string courseName);
};

//functions of BinarySearchTree-----------------------
//default constructor and destructors for this
BinarySearchTree::BinarySearchTree(){
this->root = nullptr;
}
BinarySearchTree::~BinarySearchTree(){
}

//will print all of the courses in the BST in order.
//this is the private function that takes a pointer, so it can then recurse
//it just initially gets called from the public no arguments version.
void BinarySearchTree::InOrder(Node* node){
if (node == nullptr){
      return;}                    
InOrder(node->left);
cout << node->course.courseNumber << endl;
InOrder(node->right);
}

//the public InOrder function that just calls the private InOrder function.
void BinarySearchTree::InOrder() {
InOrder(this->root);
}

//used in the loadCourses function to insert the course into the BST
void BinarySearchTree::Insert(Course course) {
Node* cur;
if (this->root == nullptr){//if the root is empty at first
//cout << "node was empty" << endl;
this->root = new Node(course);
//cout << "root assigned supposed node" << endl;
this->root->left = nullptr;
this->root->right = nullptr;
//cout << "its left/right assigned null" << endl;

}//new Node made from the passed course and the left/right null
else{//wasn't empty
cur = this->root;
   while (cur != nullptr){ 
     if (course.courseNumber < cur->course.courseNumber){
     //cout << "passed course number was less than node already in tree" << endl;
        if (cur->left == nullptr){
          cur->left = new Node(course);
          //cout << "left node was empty and now has inserted node" << endl;
          cur = nullptr;}
        else{
        //cout << "checking next left" << endl;
        cur = cur->left;}
    }else{ 
    //cout << "passed course number was greater than node already in tree" << endl;
        if (cur->right == nullptr){
        //cout << "the right node was null" << endl;
        cur->right = new Node(course);
        //cout << "the right node is now inserted node" << endl;
        cur = nullptr;}
        else{
        //cout << "checking next right" << endl;
        cur = cur->right;}
        }
     }
   }
}


void BinarySearchTree::Search(string courseNumber){
Node* cur;
cur = this->root;
cout << "-----------------------------" << endl;
cout << "Searching for: " << courseNumber << endl;
while (cur != nullptr){
    if (courseNumber == cur->course.courseNumber){
    cout << cur->course.courseNumber << ": " << cur->course.courseName << endl;
    cout << "------Prerequisites--------" << endl;
        if (cur->course.prereqs.size() > 0){
        for(int v = 0; v < cur->course.prereqs.size(); v++){
        cout << cur->course.prereqs.at(v) << endl;
        }}
        else{
        cout << "NO PREREQUISITES" << endl;}//end for (int v = 0; v < cur->course.prereqs.size(); v++)
    cur = nullptr;
    }//end if (courseNumber == cur->course.courseNumber)
    else{
    if (courseNumber < cur->course.courseNumber){
    cur = cur->left;}
    if (courseNumber > cur->course.courseNumber){
    cur = cur->right;}
    }
}
}
    
//-------------------end functions in BinarySearchTree

//function to read the file, put data into course object, and then call Insert to put course
//object into BST. reset course object and then do it again if required.
void loadCourses(string csvPath, BinarySearchTree* bst) {
    cout << "Loading CSV file " << csvPath << endl;
    ifstream inFS;       //Input file stream
    string inString;     //File data variable
    Course course;      //course object to put info into
    inFS.open(csvPath);
    if (!inFS.is_open()) {
      cout << "Could not open file" << endl;
      return;
    }
    while ((!inFS.fail()) && (!inFS.eof())) {
    getline(inFS, inString); //gets whole line
    string tmp = "";
    int j = 0;
    
    //for number of times of "how many commas + 1"
    for (int i = 0; i < (count(inString.begin(), inString.end(), ','))+1; ++i){
    while((j < inString.size()) && (inString.at(j) != ',')){//fills tmp up until comma or eol
    tmp += inString.at(j);
    j++;//iterates character
    }
    
    //checks which spot to put the gathered info in:
    //i == 0 is courseNumber, i == 1 is courseName, i > 1 is the prerequisites vector
    
    //assigning courseNumber
    if (i == 0){
    course.courseNumber = tmp;
    tmp = "";//resets temp
    //if adding 1 makes out of bounds, dont do it
    if (j+1 != inString.size()){
    j++;}
    }
    
    //assigning courseName
    if (i == 1){
    course.courseName = tmp;
    tmp = "";//resets tmp
    //if adding 1 makes out of bounds, dont do it
    if (j+1 != inString.size()){
    j++;}
    }
    
    //push prerequisites into vector
    if(i > 1){
    course.prereqs.push_back(tmp);
    tmp = "";//reset tmp
    //if adding 1 makes out of bounds, dont do it
    if (j+1 != inString.size()){
    j++;}
    
    }//end while loop not fail and eof
    }
    //inserts the course made into the bst object passed in
    bst->Insert(course);
    
    //using the same Course object over and over so
    //do this to reset them
    course.courseNumber = "";
    course.courseName = "";
    course.prereqs.clear();
    }
}//end loadCourses

//--------------MAIN---------------------
int main(){
//creates the BST object that will be populated and read from
BinarySearchTree mainBST;

//initial values for some variables
    int choice = 0;
    string inFilename = "";
    string inSearch = "";
    
//menu to call the various options
    while (choice != 4) {
        cout << "Menu:" << endl;
        cout << "  1. Load data" << endl;
        cout << "  2. Display all courses in alphanumeric order" << endl;
        cout << "  3. Display a particular course and its prerequisites" << endl;
        cout << "  4. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;
        
        switch (choice) {
        case 1: //load the data
            cin.ignore();
            cout << "Enter filename to load:" << endl;
            cout << "(default file: courselist.csv in current directory)" << endl;
            cout << ":";
            getline(cin, inFilename);
            loadCourses(inFilename, &mainBST);
            break;

        case 2: //display all courses in alphanumeric order
            cout << "All courses:" << endl;
            mainBST.InOrder();
            break;

        case 3: //display particular course
            cin.ignore();
            cout << "Enter a Course Number to search for; ie COMSCI101" << endl;
            cout << ":";
            getline(cin, inSearch);
            mainBST.Search(inSearch);
            break;

        case 4:
        break;
        }
    }
    cout << "END" << endl;
}