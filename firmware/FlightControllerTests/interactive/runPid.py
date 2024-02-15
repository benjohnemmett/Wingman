import numpy as np
import time
from matplotlib import pyplot as plt
from pidwrapper import PidControllerWrapper

class SimplePlane:
    def __init__(self, x, y):
        self.position = (x, y)
        self.directionFromXAxisRadians = 0

    def update(self):
        new_x = self.position[0] + np.cos(self.directionFromXAxisRadians)
        new_y = self.position[1] + np.sin(self.directionFromXAxisRadians)
        self.position = (new_x, new_y)


if __name__ == "__main__":
    print("Running PID test")

    plane1 = SimplePlane(0, 5)
    plane2 = SimplePlane(0, 5)
    pid1 = PidControllerWrapper(10, 0.0, 0.00)
    pid2 = PidControllerWrapper(10, 0.00, 50.0)
    delta_t = 1.0

    target_y_value = 10

    errors1 = np.zeros((100,1))
    errors2 = np.zeros((100,1))
    output1 = np.zeros_like(errors1)
    output2 = np.zeros_like(errors1)
    plane_x = np.zeros_like(errors1)
    plane_y = np.zeros_like(errors1)

    plt.figure()
    plt.grid(color='0.95')
    for i in range(100):
        errors1[i] = target_y_value - plane1.position[1]
        errors2[i] = target_y_value - plane2.position[1]
        output1[i] = pid1.run(errors1[i], delta_t)
        output2[i] = pid2.run(errors2[i], delta_t)

        plane1.directionFromXAxisRadians = output1[i] * np.pi / 180
        plane2.directionFromXAxisRadians = output2[i] * np.pi / 180
        plane1.update()
        plane2.update()
        plt.plot(plane1.position[0], plane1.position[1], 'bx')
        plt.plot(plane2.position[0], plane2.position[1], 'ro')
        print(f" dir {output2[i]} pos ({plane1.position[0]},{plane1.position[1]}) target_y {target_y_value} error {errors2[i]}")
        plt.pause(0.01)

    plt.show()
