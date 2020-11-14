import serial
import matplotlib.pyplot as plt
import datetime

bufsize = 2 * 3600 # 2 hrs of seconds

plt.ion()

serialPort = serial.Serial(port = "COM3", baudrate=115200,
                           bytesize=8, timeout=2, stopbits=serial.STOPBITS_ONE)

serialString = ""                           # Used to hold data coming over UART

x = []
y = []
d = []

while(1):

    # Wait until there is data waiting in the serial buffer
    if(serialPort.in_waiting > 0):

        # Read data out of the buffer until a carraige return / new line is found
        serialString = serialPort.readline().decode(('Ascii'))
        tokens = serialString.split()
        if len(tokens) == 2:
            d.append(datetime.datetime.now())
            d = d[-bufsize:]
            x.append(float(tokens[0]))
            x = x[-bufsize:]
            y.append(float(tokens[1]))
            y = y[-bufsize:]

            plt.gca().cla()   # clear the plot
            plt.plot(d, x)
            plt.plot(d, y)
            plt.title("Ambient vs Object IR")
            plt.legend(['Sky IR F', 'Ambient IR F'], loc='upper left')
            plt.grid()
            plt.draw()
            plt.pause(0.1)

plt.show(block=True)  # block=True lets the window stay open at the end of the animation.
