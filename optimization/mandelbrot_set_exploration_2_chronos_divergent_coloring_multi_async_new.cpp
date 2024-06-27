#include <iostream>
#include <complex>
#include <fstream>
#include <vector>
#include <chrono>//usar to calculate time of execution
#include <thread>// used later for multithreading
#include <future>
#include <algorithm>

constexpr int X  = 5000;       // X  of the image (pixels)
constexpr int Y  = 5000;      // Y  of the image(pixels)
//X *Y =total number of pixeles
//Maximum number of iterations, normally it should be around 50 but the maximum number has to be defined.
constexpr int MAX_ITER = 1000;   // Maximum number of iterations
//This values are the constants according to the set (reference books)
constexpr double X_MIN = -2.0;   // Minimum x-axis value
constexpr double X_MAX = 1.0;    // Maximum x-axis value
constexpr double Y_MIN = -1.5;   // Minimum y-axis value
constexpr double Y_MAX = 1.5;    // Maximum y-axis value

////////////////////////////////////////////////////////////////////////////////////////
// Computation of the Mandelbrot set
// Mandelbrot formula is z_n+1=(z_n)^2+c, where c is the complex constant.
// This will be applied to the each pixel
// The iteration continues until either the magnitude of z exceeds 2 or the maximum number of iteration is reached
// The number of iterations will be stored. 
// The color intensity based on the number of iterations required for divergence.
//////////////////////////////////////////////////////////////////////////////////////////

// This function computes a single row of the Mandelbrot set
// Takes the integer y representing the row index and iterates over x
// It calculates the corresponding complex number c
// This is only for one row of pixels. Async task will handle this.
// Each pixel computation is independent of others computing multiple rows concurrently.
// Each std::async will return a std::future. 
// No need to split the task among the available CPUs.

// We compute the row indepently.
std::vector<int> computeMandelbrotPixelRow(int y){// return a vector where each component is the pixel
    std::vector<int> row(X); // vector row(x) is initialized. This vector will store the iteration count.
    for (int x = 0; x < X; ++x){// Loop over each pixel in the row x is the index. X is a const.
        //inner loop
        //Complex number computation
        //Same mapping
        double real = X_MIN + (X_MAX - X_MIN) * x / (X -1);// 
        double imag = Y_MIN + (Y_MAX - Y_MIN) * y / (Y -1);
        std::complex<double>c(real,imag); // real and imag depends on x and y i.e. real(x), imag(y)

        std::complex<double> z(0.0, 0.0); // initial value. Z is current value in the iteration. It starts at 0 + 0i

        int iter; //iter keeps track of the number of iterations
        // Innter loop
        for (iter = 0; iter < MAX_ITER; ++iter){// Iteration where z will be updated
            if (std::abs(z) > 2.0) break; // stop looping if z exceeds 2.0.
            z = z * z + c; // Mandelbrot iteration
        }
            row[x] = iter; //store in row the iterations    
    }
    return row;
}


// This function will include the std::async options.
std::vector<int> computeMandelbrot(){// return vector of iterations
    std::vector<int> iterations(X * Y);// iterations vector is created. The overall iterations per pixel will be stored here.
    std::vector<std::future<std::vector<int>>> futures;// vector futures is created.
//each std::future object will store the result of an async computations.

// Asynchronous tasks for each row y
    for (int y = 0; y < Y; ++y){ //loop over each row in the image. y is row index.
//For each row, std::launch:async is used to run takss asynchronously For each y the function computeMandelbrot is called. 
//all is stored in the futures vector. std::future returned to std::async with future.push_back. 
        futures.push_back(std::async(std::launch::async, computeMandelbrotPixelRow, y));
    }

// Collection
    for (int y = 0; y < Y; ++y){//same loop as in the previous loop
        std::vector<int> row = futures [y].get();// retrieve result of the row computation.
        std::copy(row.begin(), row.end(), iterations.begin() + y * X);// copy row elements into iterations vector. 
    }
    return iterations;
}


// This does not changes. Sames as with std::thread.
// Function to save the Mandelbrot set to a PPM image file with gradient coloring.
void saveToPPM(const std::vector<int>& iterations, const std::string& filename) {
    std::ofstream ofs(filename, std::ios::binary);

    ofs << "P6\n" << X  << " " << Y  << "\n255\n";

    for (int i = 0; i < X  * Y ; ++i) {
        int iter = iterations[i];
        // Map iteration count to a color using a smooth gradient
        unsigned char r = static_cast<unsigned char>(iter % 256);
        unsigned char g = static_cast<unsigned char>((iter * 2) % 256);
        unsigned char b = static_cast<unsigned char>((iter * 5) % 256);
        ofs << r << g << b;
    }

    ofs.close();
}


// Sames as with std::thread no changes.
// We call the function to compute the Mandelbrot set along with time measurements. Execution begins.
int main() {

    std::cout << "Width of the image:" << X << std::endl;
    std::cout << "Height of the image: " << Y << std::endl;

    auto start_compute = std::chrono::high_resolution_clock::now();
    std::vector<int> iterations = computeMandelbrot();
    auto end_compute = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_compute = end_compute - start_compute;
    std::cout << "Total time taken to compute the Mandelbrot set: " << elapsed_compute.count() << " seconds" << std::endl;

    int num_cpus = std::thread::hardware_concurrency();
    std::cout << "Number of CPUs used: " << num_cpus << std::endl;

    auto start_save = std::chrono::high_resolution_clock::now();
    //Save results
    //We call the function to save iterations in PPM format.
    saveToPPM(iterations, "mandelbrot_gradient.ppm");
    auto end_save = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_save = end_save - start_save;
    std::cout << "Total time taken to save the Mandelbrot set to file: " << elapsed_save.count() << " seconds" << std::endl;

    std::cout << "Mandelbrot set with gradient coloring has been written to mandelbrot_gradient.ppm" << std::endl;

    return 0;
}