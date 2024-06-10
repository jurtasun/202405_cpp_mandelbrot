#include <iostream>
#include <thread>
using namespace std;

// Function 1 - write "+" symbol two hundred times
void function1() {
    for (int i = 0; i < 200; i++) {
        cout << "+";
    }
}
// Function 2 - write "-" symbol two hundred times
void function2() {
    for (int i = 0; i < 200; i++) {
        cout << "-";
    }
}

// Main function
int main() {

    // Call functions
    // function1();
    std::thread worker1(function1);
    // function2();
    std::thread worker2(function2);

    return 0;

}