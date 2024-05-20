# Cpp visualization of the Mandelbrot set
# Jesús Urtasun, Imperial College London - May 2024
# Visualizing the Mandelbrot set
print("Visualizing the Mandelbrot set")

import numpy as np
import matplotlib.pyplot as plt

def mandelbrot(c, max_iter):
    z = 0
    n = 0
    while abs(z) <= 2 and n < max_iter:
        z = z*z + c
        n += 1
    return n

# Image size (pixels)
width, height = 800, 800

# Plot window
re_min, re_max = -2.0, 1.0
im_min, im_max = -1.5, 1.5

# Maximum number of iterations
max_iter = 256

# Create a grid of complex numbers
re, im = np.linspace(re_min, re_max, width), np.linspace(im_min, im_max, height)
X, Y = np.meshgrid(re, im)
C = X + 1j * Y

# Compute the mandelbrot set
mandelbrot_set = np.frompyfunc(lambda c: mandelbrot(c, max_iter), 1, 1)(C).astype(np.float64)

# Plot the result
plt.figure(dpi=100)
plt.imshow(mandelbrot_set, extent=[re_min, re_max, im_min, im_max], cmap="hot")
plt.colorbar()
plt.title("Mandelbrot Set")
plt.xlabel("Re")
plt.ylabel("Im")
plt.show()