//============================================================================
// Name        : ProjectTwoCoursePlanner.cpp
// Author      : David Novosad
// Version     : 1.0
// Copyright   : Copyright © 2023 SNHU COCE
// Description : Project Two for ABC University - Course Planner for Students
//============================================================================

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <cctype>

using namespace std;

// structure to hold course information
struct Course {
    string idCourse; //variable that holds the id of the course

    string nameCourse; // variable that holds name of the course

    vector<string> prerequisitesList; // vector structure that holds prerequisites of the course

};

/*
* Fuction to display the courses
*/
void displayCourse(Course course);

vector<vector<string>> readFile(string csvPath);

/*
* Initiation of the internal structure for BST
*/
struct Node {
    Course course;
    Node* left;
    Node* right;

    Node() {
        left = nullptr;
        right = nullptr;
    }

    Node(Course c) : Node() {
        course = c;
    }
};

/*
*  Defining a Binary Search Tree class
*/
class BinarySearchTree {

private:
    Node* root;

    void addNode(Node* node, Course course);
    void displayInOrder(Node* node);
    void deleteTree(Node* node);

public:
    BinarySearchTree();
    virtual ~BinarySearchTree();
    void Insert(Course course);
    void DisplayInOrder();
    Course Search(string courseNumber);
};

/**
* Default constructor for BST
*/
BinarySearchTree::BinarySearchTree() {
    // Root initialization as empty ("nullptr")
    root = nullptr;
}

/**
* Destructor for BST
*/
void BinarySearchTree::deleteTree(Node* node) {
    if (node == nullptr) {
        return;
    }

    // delete the tree's left child/ node
    deleteTree(node->left);

    // delete the tree's right child/ node
    deleteTree(node->right);

    // delete the tree's node itself
    delete node;

}

BinarySearchTree::~BinarySearchTree() {
    deleteTree(root);
}


/**
*  Functon to add node into BST
*/
void BinarySearchTree::addNode(Node* node, Course course) {
    Node* currNode = node;

    while (currNode != nullptr) {

        if (course.idCourse.compare(currNode->course.idCourse) < 0) {
            // if the tree's left child/node is empty
            if (currNode->left == nullptr) {
                // add the course as the left node 
                currNode->left = new Node(course);
                break;
            }
            // current node becomes the assigned left node and continue from there
            currNode = currNode->left;
        }
        else {
            // if the tree's right child/node is empty
            if (currNode->right == nullptr) {
                // add the course as the right node                 
                currNode->right = new Node(course);
                break;
            }
            // current node becomes the assigned left node and continue from there
            currNode = currNode->right;
        }
    }
}

/*
*  Function to insert a course into BST
*/
void BinarySearchTree::Insert(Course course) {
    //if the tree's root node is empty
    if (root == nullptr) { 
        // the node becomes the root/parent node
        root = new Node(course);
    }
    else {
        // else go through the addNode function and add teh course into appropriate place
        addNode(root, course);
    }
}

/**
* Display the Courses in alphanumerical order
*/
void BinarySearchTree::displayInOrder(Node* node) {
    //if the node is empty
    if (node == nullptr) {
        return;
    }

    // display the tree's left node
    displayInOrder(node->left);

    // display the left node assigned course
    displayCourse(node->course);

    // display the tree's right node
    displayInOrder(node->right);
}

/**
* Display the Tree's courses in Odred
*/
void BinarySearchTree::DisplayInOrder() {

    // call the displayInOrder function starting from the root
    displayInOrder(root);
}

/**
* function to search for a specific course by its ID
*/
Course BinarySearchTree::Search(string idCourse) {
    Course course;

    // begin at the root of the Tree
    Node* currNode = root;

    // Loop through the BST searching for the course until current node is empty
    while (currNode != nullptr) {

        //if the course is found return the course information
        if (idCourse.compare(currNode->course.idCourse) == 0) {
            course = currNode->course;
            break;
        }

        // if the value of idCourse is lower than cuurent course id go to the left sub node
        else if (idCourse.compare(currNode->course.idCourse) < 0) {
            currNode = currNode->left;
        }

        // else go to the right sub node
        else {
            currNode = currNode->right;
        }
    }

    return course;
}

/**
* Function to display course information to the user
*/
void displayCourse(Course course) {

    // show all the course information
    cout << "Course ID: " << course.idCourse;
    cout << ", Course Name: " << course.nameCourse;
    cout << ", Prerequisite(s): ";

    // if no prerequisites present display "none"
    if (course.prerequisitesList.size() == 0) {
        cout << "None.";
    }
    //else if prerequisites present display them
    else {
        cout << course.prerequisitesList.at(0);

        for (unsigned int i = 1; i < course.prerequisitesList.size(); ++i) {
            cout << ", " << course.prerequisitesList.at(i);
        }
    }
    cout << endl;
}


/*
*  Error handling for parsing through the CSV file
*/
struct FileParserError : public exception {
    string  msg;
    FileParserError(string err) {
        msg = string("File Parser: ").append(err);
    }
    const char* what() const throw () {
        return msg.c_str();
    }
};



/**
* Function for separating the string by comma delimiter
* Created with a help from a website:
* https: //www.grepper.com/answers/592594/how+to+split+string+in+c%2B%2B
*/
vector<string> parseLine(string line) {
    vector<string> row;
    string delimiter = ",";
    int startIndex = 0;
    int endIndex = line.find(delimiter);

    while (endIndex != -1) {
        row.push_back(line.substr(startIndex, endIndex - startIndex));
        startIndex = endIndex + delimiter.size();
        endIndex = line.find(delimiter, startIndex);
    }
    row.push_back(line.substr(startIndex, endIndex - startIndex));
    return row;
}

/**
* Function to open a CSV file and parse through it
*/
vector<vector<string>> readFile(string csvPath) {
    vector<vector<string>> fileContents;
    fstream csvFile;

    // this opens the file
    csvFile.open(csvPath, ios::in);

    //if the file is open
    if (csvFile.is_open()) {
        string line;
        // parse through the file's each line
        while (getline(csvFile, line)) {
            if (line != "") {
                // separate the output with commas
                vector<string> row = parseLine(line);
                fileContents.push_back(row);
            }
        }

        // close the opened file
        csvFile.close();
    }
    // Error exception if the file cannot be opened
    else {
        throw FileParserError(string("Failed to open file: ").append(csvPath));
    }

    return fileContents;
}

/*
*  Function to load the coursesfrom the CSV file into BST
*/
void loadCourses(string csvPath, BinarySearchTree* bst) {
    vector<string> courseContent;
    try {
        // open and go through the file contents
        vector<vector<string>> fileContents = readFile(csvPath);

        // get the contents of the file
        for (unsigned int i = 0; i < fileContents.size(); ++i) {
            vector<string> row = fileContents.at(i);
            if (row.size() >= 2) {
                courseContent.push_back(row.at(0));
            }
        }

        //get the id and name of the courses
        for (unsigned int i = 0; i < fileContents.size(); ++i) {
            vector<string> row = fileContents.at(i);
            if (row.size() >= 2) {
                Course course;

                // assign course id as the first content of the list
                course.idCourse = row.at(0);

                // assign course name as the second content of the list
                course.nameCourse = row.at(1);

                // assign prerequisites as third and/or fourth if they are present
                for (unsigned int j = 2; j < row.size(); ++j) {
                    if (find(courseContent.begin(), courseContent.end(), row.at(j)) != courseContent.end()) {
                        course.prerequisitesList.push_back(row.at(j));
                    }
                }

                // add the course items into the BST
                bst->Insert(course);
            }
        }
    }

    // catch any errors during this proccess
    catch (FileParserError& err) {
        cerr << err.what() << endl;
    }


}


// main function for the console
int main(int argc, char* argv[])
{
    // start the BST to hold the courses' contents
    BinarySearchTree* searchTree = new BinarySearchTree();

    int choice = 0;
    cout << "Welcome to the course planner" << endl;

    // main loop to show the menu
    while (choice != 9) {
        // initialize all the variables
        string csvPath, idCourse;
        Course course;
        string number;

        switch (argc) {
        case 2:
            csvPath = argv[1];
            break;
        case 3:
            csvPath = argv[1];
            break;
        default:
            csvPath = "ABCU_Courses.csv";
        }

        // display the main menu
        cout << "\n1. Load data structure." << endl;
        cout << "2. Print Course List." << endl;
        cout << "3. Print Course. " << endl;
        cout << "9. Exit" << endl;
        cout << "What would you like to do? ";
        cin >> choice;
        switch (choice)
        {
        case 1:
            // open the CSV file and load all the courses into a BST
            searchTree = new BinarySearchTree();

            //cout << "Enter file path: ";
            //cin.ignore();
            //getline(cin, csvPath);

            loadCourses(csvPath, searchTree);
            break;
        case 2:
            // display all the courses in alphanumerical order
            searchTree->DisplayInOrder();
            break;
        case 3:
            // search for an user pecified course
            cout << "Enter course number: ";
            cin.ignore();
            getline(cin, idCourse);

            // make sure that the input is not empty
            number = idCourse;

            number.erase(remove_if(number.begin(), number.end(), isspace), number.end());

            // if input is empty
            if (number == "") {
                // display error message
                cout << "Course number cannot be empty" << endl;
            }
            // else search for the course
            else {
                course = searchTree->Search(idCourse);
                // if course found display it
                if (!course.idCourse.empty()) {
                    displayCourse(course);
                }
                //else if not found display error message
                else {
                    cout << "Course number " << idCourse << " not found." << endl;
                }
            }
            break;

        //if the option selected does not exist display error message
        default:
            if (choice != 9) {
                cout << choice << " is not a valid choice." << endl;
            }
            break;
        }
    }
    //if option 9 is selected display exit message
    cout << "Thank you for using the course planner!" << endl;
    return 0;
}