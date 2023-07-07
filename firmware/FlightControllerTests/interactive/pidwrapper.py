import ctypes


class PidControllerState(ctypes.Structure):
     _fields_ = [
             ("kp", ctypes.c_float),
             ("ki", ctypes.c_float),
             ("kd", ctypes.c_float),
             ("int_max", ctypes.c_float),
             ("int_min", ctypes.c_float),
             ("last_error", ctypes.c_float),
             ("has_last_error", ctypes.c_ubyte),
             ("status", ctypes.c_ubyte),
             ]


class PidControllerWrapper():
     
    def __init__(self, kp, ki, kd):
        self.pid = ctypes.CDLL("PidController.so")
        self.pid.RunPidController.restype = ctypes.c_float
        print(f" loaded pid libarary {self.pid}")

        self.state = PidControllerState(ctypes.c_float(kp),
                                    ctypes.c_float(ki),
                                    ctypes.c_float(kd),
                                    ctypes.c_float(10.),
                                    ctypes.c_float(-10.),
                                    ctypes.c_float(0.), 
                                    ctypes.c_ubyte(0),
                                    ctypes.c_ubyte(0))
        print(f" initial state {self.state}")

    def run(self, error, dt):
        correction = self.pid.RunPidController(ctypes.pointer(self.state), ctypes.c_float(error), ctypes.c_float(dt))
        return correction

