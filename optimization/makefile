# All executables to be generated
all: multi_thread multithread_std_threads_square_numbers multithread_std_threads_grid

# Compilation intstructions
multi_thread: multi_thread.cpp
	g++ multi_thread.cpp -o  multi_thread

multithread_std_threads_square_numbers: multithread_std_threads_square_numbers.cpp
	g++ -std=c++11 multithread_std_threads_square_numbers.cpp -o multithread_std_threads_square_numbers

multithread_std_threads_grid: multithread_std_threads_grid.cpp
	g++ -std=c++11 multithread_std_threads_grid.cpp -o multithread_std_threads_grid

# Clean instructions
clean:
	rm multi_thread
	rm multithread_std_threads_square_numbers
	rm multithread_std_threads_grid