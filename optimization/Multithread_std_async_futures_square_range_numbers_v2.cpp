#include <iostream>
#include <complex>
#include <fstream>
#include <vector>
#include <chrono>
#include <thread>
#include <algorithm> 
//#include <mutex>
#include <future>

//Compute square of number using multithread


int square(int num) {// num is input 
    return num * num;


}

//Function to compute squares
void computeSquares(std::vector<int>& results, int start, int end) {
    for (int i = start; i <end; ++i){//loop from start to end
            results[i] = square(i);//call function square of index i and save in results

    }

}

int main(){
    const int N= 20;//input of number of elemnts
    const int numThreads = std::thread::hardware_concurrency();//it gives the number of CPUs available

    std::vector<int> results(N);//declaration of vector results with size N

    int rangePerThread = N / numThreads;//distribute the amount of computation amogn threads

    //std::vector<std::thread> threads(numThreads);//declaration of vector threads with size numThreads
    std::vector<std::future<void>> futures;
    futures.reserve(numThreads); // Reserve space for futures
    
    //inner loop
    for (int i=0; i < numThreads; ++i){// loop over threads
        int start = i* rangePerThread;//starting index
        int end = (i == numThreads -1) ? N : (i+1)*rangePerThread; //ending index where remaining elements are considered

        //threads[i]=std::thread(computeSquares,std::ref(results), start, end);//calls computeSquares
        //here you can write over the vector threads and give a value. std::threads calls the threads function with the given arguments.

        futures.emplace_back(std::async(std::launch::async, computeSquares, std::ref(results), start, end));
    }

    // Wait for all asynchronous tasks to complete
    for (auto& future : futures) {
        future.get();
    }


    //Similar to MPI_ALLREDUCE we need to wait until all threads are finished
    //for (int i = 0; i < numThreads; ++i) {
    //threads[i].join();



    //Printout
    for (int i=0; i < N; ++i){ 
        std::cout << "Square of " << i << " is " << results[i] << std::endl;
    }
    return 0;
}