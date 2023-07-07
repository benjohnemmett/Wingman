from pidwrapper import PidControllerWrapper

if __name__ == "__main__":
    print("Running PID test")

    pid = PidControllerWrapper(0.5, 0.1, 0)
    delta_t = 1.0

    for error in range(-10, 10, 1):
        correction = pid.run(error, delta_t)
        print(f" {error} -> {correction}")