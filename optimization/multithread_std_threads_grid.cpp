// C++ function to explain the use of the Multithread variation 1 using std::thread
// Computation of the sum of each row of a 2D (X*Y) array with threads

// Vector initialization to store the results
// std::vector<int> iterations(X*Y);

// Thread initialization, it gives the number of hardware threads
// int numThreads = std::thread::hardware_concurrency();
// It creatas a vector to store the thread objects
// std::vector<stad::thread> threads(numThreads)

// Division of works, it gets how many rows each thread will process
// int rowsPerThread = HEIGHT/ numThreads;

// We need to create a loop that iterates over the number of threads
// Each thread will be assigned a range of rows to process a function set up by the user

// Another loop is needed to call join() on each thread to wait for all threads to complete the execution

// built-in commands of the thread library are:
// std::thread::hardware_concurrency()
// std::thread()
// std::thread::join()
// std::ref()

#include <iostream>
#include <vector>
#include <thread>
//#include <mutex>

const int X = 5;
const int Y = 10;

void ComputeRowSums(std::vector<int>& iterations, int startRow, int endRow) {

    for (int row = startRow; row < endRow; ++row) {
        int sum = 0; // we initialize with zero
        for (int col = 0; col < X; ++col) {
            sum += row * X + col; // arithmetic sum of indices
        }
        iterations[row] = sum;
    }
}

// This contains the Multithread function
std::vector<int> computeFunction() { // returns vector

    std::vector<int> iterations(Y); // vector iteration with height as elements

    // We count the number of available CPUs to know how to divide the work
    int numThreads = std::thread::hardware_concurrency();
    std::vector<std::thread> threads(numThreads); // vector "threads" with a size of the number of CPUS (numThreads)
    int rowsPerThread = Y / numThreads; // Calculation of the number of rows that each thread will take
    for (int i = 0; i< numThreads; ++i) { // iteration of the the number of threads
    
        // thread vector definition
        // Range of rows that each thread will process
        int startRow = i*rowsPerThread;
        int endRow = (i == numThreads-1) ? Y : (i +1)* rowsPerThread; // endRow will be assigned HEIGHT if the condition is met otherwise (i +1)* rowsPerThread 

        // We create the threads here
        threads[i] = std::thread(ComputeRowSums, std::ref(iterations), startRow, endRow); // threads as a function of i

    }
    for (int i = 0; i < numThreads; ++i) {
        threads[i].join(); // Wait for each thread to finish execution
    }

    return iterations;

}

int main() {

    std::vector<int>result = computeFunction(); // calls function. Values stored in result.
    
    for (int i = 0; i < result.size(); ++i) { // loops over result
        std::cout << "Sum of row " << i << " = " << result[i] << std::endl; // output print
    }

  return 0;

}

