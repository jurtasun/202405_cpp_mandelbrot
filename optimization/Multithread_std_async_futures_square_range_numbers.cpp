# include <iostream>
# include <vector>
# include <thread>
# include <future>// Needs to be included to be able to used std::async and std::future

// Compute square of a number

int square(int num){
    return num * num;

}

int main() {
    constexpr int N= 20;
    const int numThreads = std::thread::hardware_concurrency();

    std::vector<int> results(N); // the results will be stored in this vector

    int rangePerThread = N / numThreads;

    std::vector<std::future<void>> futures; // vector needs to be created to store futures to use std::async

    for (int i= 0; i < numThreads; ++i) { // Loop to launch tasks for each thread
         int start = i * rangePerThread; // Compute the starting index for this thread
         int end = (i == numThreads -1) ? N : (i +1) * rangePerThread;// Compute the last index

         // Launch the task using std::async

         futures.push_back(std::async(std::launch::async, [=, &results]() {

            for (int j = start; j < end; ++j) {

                results[j] = square(j); // Compute the square of j and store it in the results vector 
            }
         }));

    }

    // All asynchronous tasks have to be completed
    for (auto& future : futures) {
        future.wait();

    }


    // Output of the code
    for (int i = 0; i < N; ++i) {
        std::cout << "Square of " << i << " is " << results[i] << std::endl; 
    }
    return 0;

}