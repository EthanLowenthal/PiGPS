from display import Display
import time

display = Display()


display.clear()

current_heading = 0

while True:
    # display.clear()
    start = time.time_ns()

    display.draw_speed()
    display.draw_compass(int(current_heading))

    end = time.time_ns()

    fps = 1e+9 / (end - start)

    display.fps(fps)
    display.flip()

    current_heading += 0.1
    current_heading %= 360

# display.surface.write_to_png("surface.png")