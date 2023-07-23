from pidwrapper import PidControllerWrapper

import tkinter as tk
import turtle

class App:
    def __init__(self, master):
        self.master = master
        self.master.title("Raw Turtle")
        self.canvas = tk.Canvas(master)
        self.canvas.config(width=600, height=200)
        self.canvas.pack(side=tk.LEFT)
        self.button = tk.Button(self.master, text="Press me", command=self.press)
        self.button.pack()

    def do_stuff(self):
        for color in ["red", "yellow", "green"]:
            self.my_lovely_turtle.color(color)
            self.my_lovely_turtle.right(120)

    def press(self):
        self.do_stuff()





if __name__ == "__main__":
    print("Running PID test")

    pid = PidControllerWrapper(0.5, 0.1, 0)
    delta_t = 1.0

    for error in range(-10, 10, 1):
        correction = pid.run(error, delta_t)
        print(f" {error} -> {correction}")

    root = tk.Tk()
    app = App(root)
    root.mainloop()