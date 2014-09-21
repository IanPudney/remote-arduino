from serial import Serial
import time

class SerialException(Exception): pass
class PortNotOpenException(SerialException): pass
class Arduino(object):
    numAnalogPins = 6
    numDigitalPins = 14

    INPUT = "i"
    OUTPUT = "o"
    INPUT_PULLUP = "p"

    HIGH = True
    LOW = False

    class Pin(object):
        def __init__(self, number, arduino):
            self.number = number
            self._arduino = arduino
            self._value = 0
        def write(self, value):
            raise NotImplementedError()
        def read(self):
            raise NotImplementedError()
        def setMode(self, mode):
            self._arduino._setMode(self.number, mode)

        pass
    class DigitalPin(Pin):
        def write(self, value):
            self._arduino._writeDigital(self.number, value)
        def read(self):
            self._arduino._readDigital()
            return self._value
        pass
    class AnalogPin(Pin):
        def write(self, value):
            self._arduino._writeAnalog(self.number, value)
        def read(self):
            self._arduino._readAnalog()
            return self._value
        pass

    def __init__(self, port=0, baud=115200):
        self.baud = baud
        self.sconn = Serial(port, baud)
        if(not self.sconn.isOpen()):
            raise PortNotOpenException()

        self.port = self.sconn.getPort()
        self.analog = [Arduino.AnalogPin(x, self) for x in range(0, self.numAnalogPins)]
        self.digital = [Arduino.DigitalPin(x, self) for x in range(0, self.numDigitalPins)]


    def _readDigital(self):
        self.sconn.write('r d\r')
        values = self.sconn.readline().split()[1:]
        for i in range(2, self.numDigitalPins):
            self.digital[i]._value = int(values[i-2])

    def _writeDigital(self, pin, value):                                                                                                                          
        self.sconn.write('w d ' + str(pin) + ' ' + ("1" if value else "0") + '\r')
        time.sleep(.01)

    def _setMode(self, pin, mode):
        self.sconn.write('m ' + str(pin) + " " + mode + '\r')
        time.sleep(.01)

    def _readAnalog(self):
        self.sconn.write('r a\r')
        values = self.sconn.readline().split()[1:]
        for i in range(0, self.numDigitalPins):
            self.analog[i]._value = int(values[i-2])
        pass

    def _writeAnalog(self, pin, value):
        self.sconn.write('w a ' + str(pin) + ' ' + str(value) + '\r')
        time.sleep(.01)
        pass
