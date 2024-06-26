#include <iostream>
#include <complex>
#include <fstream>
#include <vector>
#include <chrono>
#include <thread>
#include <algorithm> 
//#include <mutex>
#include <future>


constexpr int X = 2000;       // X of the image
constexpr int Y = 2000;      // Y of the image
constexpr int MAX_ITER = 1000;    // Maximum number of iterations
constexpr double X_MIN = -2.0;    // Minimum x-axis value
constexpr double X_MAX = 1.0;     // Maximum x-axis value
constexpr double Y_MIN = -1.5;    // Minimum y-axis value
constexpr double Y_MAX = 1.5;     // Maximum y-axis value


void computeMandelbrotRows(std::vector<int>& iterations, int startRow, int endRow) {
    for (int y = startRow; y < endRow; ++y) {
        for (int x = 0; x < X; ++x) {
            double real = X_MIN + (X_MAX - X_MIN) * x / (X - 1);
            double imag = Y_MIN + (Y_MAX - Y_MIN) * y / (Y - 1);
            std::complex<double> c(real, imag);
            std::complex<double> z(0.0, 0.0);

            int iter;
            for (iter = 0; iter < MAX_ITER; ++iter) {
                if (std::abs(z) > 2.0) break;
                z = z * z + c;
            }

            iterations[y * X + x] = iter;
        }
    }
}

// Function to compute the Mandelbrot set using multithreading
std::vector<int> computeMandelbrotParallel() {
    std::vector<int> iterations(X * Y);

    int numThreads = std::thread::hardware_concurrency();
    
    std::vector<std::future<void>> futures(numThreads);
    // std::vector<std::future<void>> futures;
    // futures.reserve(numThreads); // Reserve space for futures
    //std::vector<std::thread> threads(numThreads);
    //
    int rowsPerThread = Y / numThreads;

    for (int i = 0; i < numThreads; ++i) {
        int startRow = i * rowsPerThread;
        int endRow = (i == numThreads - 1) ? Y : (i + 1) * rowsPerThread;
        //replaced with emplace_back function
        //threads[i] = std::thread(computeMandelbrotRows, std::ref(iterations), startRow, endRow);
        //futures.emplace_back(computeMandelbrotRows, std::ref(iterations), startRow, endRow);// Add new threads to the vector as you create them
        //futures.emplace_back(std::async(std::launch::async, computeMandelbrotRows,std::ref(iterations),startRow, endRow));
        futures[i] = std::async(std::launch::async, computeMandelbrotRows, std::ref(iterations), startRow, endRow);
    }

    for (auto& future : futures) {
        future.get();
    }

    return iterations;
}

// Function to save the Mandelbrot set to a PPM image file with gradient coloring
void saveToPPM(const std::vector<int>& iterations, const std::string& filename) {
    std::ofstream ofs(filename, std::ios::binary);

    ofs << "P6\n" << X << " " << Y << "\n255\n";

    for (int i = 0; i < X * Y; ++i) {
        int iter = iterations[i];
        // Map iteration count to a color using a smooth gradient
        unsigned char r = static_cast<unsigned char>(iter % 256);
        unsigned char g = static_cast<unsigned char>((iter * 2) % 256);
        unsigned char b = static_cast<unsigned char>((iter * 5) % 256);
        ofs << r << g << b;
    }

    ofs.close();
}

int main() {
    std::cout << "Width of the image:" << X << std::endl;
    std::cout << "Height of the image: " << Y << std::endl;


    auto start_compute = std::chrono::high_resolution_clock::now();
    std::vector<int> iterations = computeMandelbrotParallel();
    auto end_compute = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_compute = end_compute - start_compute;
    std::cout << "Total time taken to compute the Mandelbrot set:" << elapsed_compute.count() << " seconds" << std::endl;

    int num_cpus = std::thread::hardware_concurrency();
    std::cout << "Number of CPUs avaialble: " << num_cpus << std::endl;

    auto start_save = std::chrono::high_resolution_clock::now();
    saveToPPM(iterations, "mandelbrot_gradient.ppm");
    auto end_save = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_save = end_save - start_save;
    std::cout << "Total time taken to save the Mandelbrot set to file: " << elapsed_save.count() << " seconds" << std::endl;

    std::cout << "Mandelbrot set with gradient coloring has been written to mandelbrot_gradient.ppm" << std::endl;

    return 0;
}