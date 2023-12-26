import turtle

def draw_sierpinski(t, order, size):
    if order == 0:
        for _ in range(3):
            t.forward(size)
            t.left(120)
    else:
        size /= 2
        draw_sierpinski(t, order - 1, size)
        t.forward(size)
        draw_sierpinski(t, order - 1, size)
        t.backward(size)
        t.left(60)
        t.forward(size)
        t.right(60)
        draw_sierpinski(t, order - 1, size)
        t.left(60)
        t.backward(size)
        t.right(60)



blob = turtle.Turtle()
blob.speed(0)
blob.pensize(2)
blob.color("black")

order = 2  # Adjust the order for more or fewer iterations
size = 300  # Adjust the initial size of the triangle

blob.penup()
blob.goto(-size / 2, -size / 2)
blob.pendown()

draw_sierpinski(blob, order, size)

turtle.mainloop()



