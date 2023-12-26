import matplotlib.pyplot as plt
import numpy as np

def barnsley_fern(iterations):
    x, y = 0, 0
    points_x, points_y = [x], [y]

    for _ in range(iterations):
        r = np.random.rand()

        if r < 0.01:
            x, y = 0, 0.16 * y
        elif r < 0.86:
            x = 0.85 * x + 0.04 * y
            y = -0.04 * x + 0.85 * y + 1.6
        elif r < 0.93:
            x = 0.2 * x - 0.26 * y
            y = 0.23 * x + 0.22 * y + 1.6
        else:
            x = -0.15 * x + 0.28 * y
            y = 0.26 * x + 0.24 * y + 0.44

        points_x.append(x)
        points_y.append(y)

    return points_x, points_y

def main():
    iterations = 100000
    points_x, points_y = barnsley_fern(iterations)

    plt.scatter(points_x, points_y, s=1, c='green', marker='.')
    plt.title('Barnsley Fern')
    plt.show()

if __name__ == "__main__":
    main()
