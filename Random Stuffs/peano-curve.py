from turtle import *

def peano(level, length):
    if level == 0:
        forward(length)
    else:
        peano(level-1, length/3)

        right(90)
        peano(level-1, length/3)

        left(90)
        peano(level-1, length/3)
        left(90)
        peano(level-1, length/3)
        left(90)
        peano(level-1, length/3)

        right(90)
        peano(level-1, length/3)
        right(90)
        peano(level-1, length/3)
        right(90)
        peano(level-1, length/3)

        left(90)
        peano(level-1, length/3)

# Starting position and angle to fill our window
#speed
speed()
penup()
goto(-220, 220)
pendown()
right(45)

peano(1, 600)

exitonclick()