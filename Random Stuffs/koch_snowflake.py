# draw koch snowflake

import turtle

def koch(t, length):
    if length < 3:
        t.fd(length)
        return
    koch(t, length/3)
    t.lt(60)
    koch(t, length/3)
    t.rt(120)
    koch(t, length/3)
    t.lt(60)
    koch(t, length/3)

def snowflake(t, length):
    for i in range(3):
        koch(t, length)
        t.rt(120)

bob = turtle.Turtle()
#set window size
turtle.setup(800, 800)
bob.speed(0)
bob.pensize(2)
bob.pu()
bob.goto(0, 0)
bob.pd()
snowflake(bob, 200)

turtle.mainloop()

