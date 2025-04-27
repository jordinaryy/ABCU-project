// Jordan Krueger
// Project 2 
// CS 300
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
using namespace std;

// A simple structure to hold data for a single course
struct Course {
    string courseId;               
    string title;                 // Course title
    vector<string> prerequisites; // List of prerequisite course IDs
};

// A node in the Binary Search Tree
struct Node {
    Course course;
    Node* left;
    Node* right;

    // Constructor for node
    Node(Course aCourse) {
        course = aCourse;
        left = nullptr;
        right = nullptr;
    }
};

// Binary Search Tree class for storing all courses
class CourseBST {
private:
    Node* root;

    // Recursive helper to add a course to the BST
    void addNode(Node*& node, Course course) {
        if (node == nullptr) {
            node = new Node(course); // insert here
        }
        else if (course.courseId < node->course.courseId) {
            addNode(node->left, course); // go left
        }
        else {
            addNode(node->right, course); // go right
        }
    }

    // Recursive in-order traversal to print all courses
    void inOrder(Node* node) {
        if (node != nullptr) {
            inOrder(node->left); // visit left
            cout << node->course.courseId << ": " << node->course.title << endl;
            inOrder(node->right); // visit right
        }
    }

    // Recursive search to find a course by ID
    Course* search(Node* node, string courseId) {
        if (node == nullptr) return nullptr; // not found
        if (node->course.courseId == courseId) return &node->course; // found

        if (courseId < node->course.courseId) {
            return search(node->left, courseId);
        }
        else {
            return search(node->right, courseId);
        }
    }

public:
    // Constructor
    CourseBST() {
        root = nullptr;
    }

    // Insert a course into the tree
    void insert(Course course) {
        addNode(root, course);
    }

    // Print all courses in sorted (alphanumeric) order
    void printAllCourses() {
        inOrder(root);
    }

    // Find a course by ID
    Course* findCourse(string courseId) {
        return search(root, courseId);
    }
};

// Removes whitespace
string trim(string str) {
    size_t start = str.find_first_not_of(" \t\r\n");
    size_t end = str.find_last_not_of(" \t\r\n");
    return (start == string::npos) ? "" : str.substr(start, end - start + 1);
}

// Load courses from a tab-separated file
void loadCourses(string filename, CourseBST& tree) {
   cout << "Trying to open file: " << filename << endl;


    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: couldn't open file." << endl;
        return;
    }

    string line;
    // Read each line of the file
    while (getline(file, line)) {
        vector<string> fields;
        stringstream ss(line);
        string value;

        // Split the line using tabs
        while (getline(ss, value, ',')) {
            fields.push_back(trim(value));
            
        }

        // Make sure we at least have course ID and title
        if (fields.size() >= 2) {
            Course course;
            course.courseId = fields[0];
            course.title = fields[1];

            // If more fields do exist, they will be considered prerequisites 
            for (size_t i = 2; i < fields.size(); ++i) {
                course.prerequisites.push_back(fields[i]);
            }

            // Insert course into the BST
            tree.insert(course);
            cout << "Loaded: " << course.courseId << endl;
        }
    }

    file.close();
    cout << "Courses loaded successfully." << endl;
}

// Print info about one course
void printCourseDetails(CourseBST& tree, string courseId) {
    Course* course = tree.findCourse(courseId);

    if (course == nullptr) {
        cout << "Course not found." << endl;
        return;
    }

    // Print course ID and title
    cout << course->courseId << ": " << course->title << endl;

    // Print prerequisites if any
    if (course->prerequisites.empty()) {
        cout << "Prerequisites: None" << endl;
    }
    else {
        cout << "Prerequisites: ";
        for (size_t i = 0; i < course->prerequisites.size(); ++i) {
            cout << course->prerequisites[i];
            if (i < course->prerequisites.size() - 1) {
                cout << ", ";
            }
        }
        cout << endl;
    }
}

// Display menu options
void showMenu() {
    cout << endl;
    cout << "====== Course Planner Menu ======" << endl;
    cout << "1. Load Courses" << endl;
    cout << "2. Print Course List" << endl;
    cout << "3. Print Course Info" << endl;
    cout << "9. Exit" << endl;
    cout << "=================================" << endl;
    cout << "Enter your choice: ";
}

// Main program
int main() {
    CourseBST tree;
    string filename;
    string courseId;
    int choice = 0;

    while (choice != 9) {
        showMenu();           // Show user menu
        cin >> choice;        // Get user input
        cin.ignore();         // Clear input buffer

        if (choice == 1) {
            // Load data from file
            cout << "Enter file name:";
            getline(cin, filename);
            cout << endl;

            loadCourses(filename, tree);
        }

        else if (choice == 2) {
            // Print sorted list of courses
            cout << "\nCourse List:\n";
            tree.printAllCourses();
        }
        else if (choice == 3) {
            // Search for a course
            cout << "Enter course ID (example: CSCI400): ";
            getline(cin, courseId);
            printCourseDetails(tree, courseId);
        }
        else if (choice == 9) {
            // Exit the program
            cout << "Goodbye!" << endl;
        }
        else {
            // Invalid input
            cout << "Invalid choice. Try again." << endl;
        }
    }

    return 0;
}

