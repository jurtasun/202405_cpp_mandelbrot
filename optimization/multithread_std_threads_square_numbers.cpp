# include <iostream>
# include <vector>
# include <thread>
using namespace std;

// Compute square of number using multithread

// Compute cpu number and divide in chunks
// Compile from Linux / Mac Os -  make sure C++ version 11 or higher
// g++ -std=c++11 Multithread_std_threads_square_range_numbers.cpp -o Multithread_std_threads_square_range_numbers

int square(int num) { // num is input 
    return num * num;
}

// Function to compute squares
void computeSquares(std::vector<int>& results, int start, int end) {

    for (int i = start; i <end; ++i) { // loop from start to end
            results[i] = square(i); // call function square of index i and save in results
    }

}

int main() {

    const int N = 20; // input of number of elemnts
    const int numThreads = std::thread::hardware_concurrency(); // it gives the number of CPUs available

    std::vector<int> results(N); // declaration of vector results with size N

    int rangePerThread = N / numThreads; // distribute the amount of computation amogn threads

    std::vector<std::thread> threads(numThreads); // declaration of vector threads with size numThreads

    // inner loop
    for (int i = 0; i < numThreads; ++i) { // loop over threads

        int start = i* rangePerThread; // starting index
        int end = (i == numThreads -1) ? N : (i+1)*rangePerThread; // ending index where remaining elements are considered
        threads[i]=std::thread(computeSquares,std::ref(results), start, end); // calls computeSquares
        // here you can write over the vector threads and give a value. std::threads calls the threads function with the given arguments

    }

    // Similar to MPI_ALLREDUCE we need to wait until all threads are finished
    for (int i = 0; i < numThreads; ++i) {
        threads[i].join();
    }

    // Print out
    for (int i = 0; i < N; ++i) {
        std::cout << "Square of " << i << " = " << results[i] << std::endl;
    }

    return 0;
    
}