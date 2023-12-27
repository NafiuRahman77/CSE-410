import matplotlib.pyplot as plt
import numpy as np

def mandelbrot_set(width, height, x_min, x_max, y_min, y_max, max_iter):
    x, y = np.linspace(x_min, x_max, width), np.linspace(y_min, y_max, height)
    X, Y = np.meshgrid(x, y)
    Z = X + 1j * Y
    img = np.zeros(Z.shape, dtype=int)

    for i in range(max_iter):
        mask = np.abs(Z) < 1000  # Escape radius
        Z[mask] = Z[mask] ** 2 + X[mask] + 1j * Y[mask]
        img += mask

    return img

def display_mandelbrot_set(img):
    plt.imshow(img, cmap='hot', extent=(x_min, x_max, y_min, y_max))
    plt.title('Mandelbrot Set')
    plt.xlabel('Re')
    plt.ylabel('Im')
    plt.show()

if __name__ == "__main__":
    width, height = 800, 800
    x_min, x_max = -2, 2
    y_min, y_max = -2, 2
    max_iter = 100

    mandelbrot_img = mandelbrot_set(width, height, x_min, x_max, y_min, y_max, max_iter)
    display_mandelbrot_set(mandelbrot_img)
