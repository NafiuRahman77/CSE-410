import turtle

def fibonacci(t, n, angle, scale):
    a, b = 0, 1
    for _ in range(n):
        t.forward(scale * b)
        t.left(angle)
        a, b = b, a + b

def calculate_scale(n):
    # Adjust this function based on your desired scale
    return 3 / (n)

bob = turtle.Turtle()
turtle.setup(800, 800)
bob.speed(0)
bob.pensize(2)
bob.pu()
bob.goto(0, 0)
bob.pd()

iterations = 100
angle = 25
scale = calculate_scale(iterations)

fibonacci(bob, iterations, angle, scale)

turtle.mainloop()
