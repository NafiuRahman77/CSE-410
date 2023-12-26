import turtle

def hilbert_curve(t, order, size, angle=90):
    if order == 0:
        return
    t.right(angle)
    hilbert_curve(t, order - 1, size, -angle)
    t.forward(size)
    t.left(angle)
    hilbert_curve(t, order - 1, size, angle)
    t.forward(size)
    hilbert_curve(t, order - 1, size, angle)
    t.left(angle)
    t.forward(size)
    hilbert_curve(t, order - 1, size, -angle)
    t.right(angle)


bob = turtle.Turtle()
bob.speed(0)
bob.pensize(2)

order = 5  # You can adjust the order for more or fewer iterations
size = 10

bob.penup()
bob.goto(-size / 2, -size / 2)
bob.pendown()

hilbert_curve(bob, order, size)

turtle.mainloop()
