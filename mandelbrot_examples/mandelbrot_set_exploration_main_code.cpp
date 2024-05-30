#include <iostream>
#include <complex>
#include <fstream>
#include <vector>
#include <chrono>//usar to calculate time of execution
#include <thread>// used later for multithreading

const int WIDTH = 1000;       // Width of the image (pixels)
const int HEIGHT = 1000;      // Height of the image(pixels)
//WIDTH*HEIGHT=total number of pixeles
//Maximum number of iterations, normally it should be around 50 but the maximum number has to be defined.
const int MAX_ITER = 1000;   // Maximum number of iterations
//This values are the constants according to the set (reference books)
const double X_MIN = -2.0;   // Minimum x-axis value
const double X_MAX = 1.0;    // Maximum x-axis value
const double Y_MIN = -1.5;   // Minimum y-axis value
const double Y_MAX = 1.5;    // Maximum y-axis value

////////////////////////////////////////////////////////////////////////////////////////
// Computation of the Mandelbrot set
// Mandelbrot formula is z_n+1=(z_n)^2+c, where c is the complex constant.
// This will be applied to the each pixel
// The iteration continues until either the magnitude of z exceeds 2 or the maximum number of iteration is reached
// The number of iterations will be stored. 
// The color intensity based on the number of iterations required for divergence.
//////////////////////////////////////////////////////////////////////////////////////////

// Function to compute the Mandelbrot set
std::vector<int> computeMandelbrot() {
// This starts a vector called iterations (global) with the size equal to the total number of pixels
// image (width* height). Each element of this vector will store the number of iterations required 
// for divergence for the corresponding pixel.
    std::vector<int> iterations(WIDTH * HEIGHT);

// This corresponds to the chrono library. We start the execution time.
    double total_scaling_time = 0.0;
    double total_iteration_time = 0.0;

// outer loops runs through each row y and the inner loop runs through each column x.
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            auto start_scaling = std::chrono::high_resolution_clock::now();//start taking time
            // Scale pixel coordinate to complex plane. Normalized. x,y start in the top left corner with (0,0) until it map the whole domain.
            double real = X_MIN + (X_MAX - X_MIN) * x / (WIDTH - 1);
            double imag = Y_MIN + (Y_MAX - Y_MIN) * y / (HEIGHT - 1);

            // creation of a complex number mapped into the pixel (x,y)
            // real and imag depend on x,y 
            std::complex<double> c(real, imag);
            // z starts from zero as defined by the algorithmus.
            std::complex<double> z(0.0, 0.0);
            auto end_scaling = std::chrono::high_resolution_clock::now();//stop taking time
                        // display of the total time of this section
            total_scaling_time += std::chrono::duration_cast<std::chrono::microseconds>(end_scaling - start_scaling).count() / 1e6;
            // declaration of the iteration number: iter
            int iter;
            auto start_iteration = std::chrono::high_resolution_clock::now();//start taking time
            // Inner loop 
            // Loop up to MAX_ITER 
            for (iter = 0; iter < MAX_ITER; ++iter) {
                // the magnitude of z=2 is fixed.
                if (std::abs(z) > 2.0) break;
                //Mandelbrot set equation and conditions here!
                //We update each time z
                // c is the complex number corresponding to the pixel. This is the connection to the grid.
                z = z * z + c;
            }
            auto end_iteration = std::chrono::high_resolution_clock::now();//stop taking time
            // display of the total time of this section
            total_iteration_time += std::chrono::duration_cast<std::chrono::microseconds>(end_iteration - start_iteration).count() / 1e6;
            // Store interations count. At the respective position.
            // This index calculator y*WIDTH+x maps the 2D coordinates to the 1D vector. 
            iterations[y * WIDTH + x] = iter;
        }
    }
    // Give the the total scaling and iteration time.
    std::cout << "Total scaling time: " << total_scaling_time << " seconds" << std::endl;
    std::cout << "Total iteration time: " << total_iteration_time << " seconds" << std::endl;
    // Give the iterations counts for each pixel in the image.
    return iterations;
}

// Function to save the Mandelbrot set to a PPM image file with gradient coloring.
void saveToPPM(const std::vector<int>& iterations, const std::string& filename) {
    std::ofstream ofs(filename, std::ios::binary);

    ofs << "P6\n" << WIDTH << " " << HEIGHT << "\n255\n";

    for (int i = 0; i < WIDTH * HEIGHT; ++i) {
        int iter = iterations[i];
        // Map iteration count to a color using a smooth gradient
        unsigned char r = static_cast<unsigned char>(iter % 256);
        unsigned char g = static_cast<unsigned char>((iter * 2) % 256);
        unsigned char b = static_cast<unsigned char>((iter * 5) % 256);
        ofs << r << g << b;
    }

    ofs.close();
}



// We call the function to compute the Mandelbrot set along with time measurements. Execution begins.
int main() {
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