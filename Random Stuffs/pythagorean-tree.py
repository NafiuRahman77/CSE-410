import turtle
import math

def pythagorean_tree(t, depth, length, angle):
    if depth == 0:
        return
    t.forward(length)
    t.left(angle)
    pythagorean_tree(t, depth-1, 0.7*length, angle)
    t.right(2 * angle)
    pythagorean_tree(t, depth-1, 0.7*length, angle)
    t.left(angle)
    t.backward(length)


blob = turtle.Turtle()
blob.speed(0)
blob.pensize(2)
blob.color("green")

blob.penup()
blob.goto(0, -200)
blob.pendown()
blob.left(90)
pythagorean_tree(blob, 7, 150, 30)

turtle.mainloop()