from selenium import webdriver
import time
import pandas as pd
import matplotlib.pyplot as plt

url = 'http://192.168.8.236'

plt.ion()
x = []
y = []
d = []
dif = []
pltBufSize = 5000

refreshrate = 5  # sample rate in seconds
driver = webdriver.Chrome('C:/Users/hoyes/Documents/libs/chromedriver.exe')
driver.get(url)

rowList = []
for _ in range(6000):
    time.sleep(refreshrate)
    driver.refresh()
    table = pd.read_html(driver.page_source,header=0)[0]
    rowDict = {col:val[0] for col,val in table.iteritems()}
    rowDict['Diff'] = rowDict['Ambient_F'] - rowDict['Object_F']
    rowDict['DateTime'] = pd.Timestamp.now()
    rowList.append(rowDict)

    ## charting
    if rowDict['Ambient_F'] < 150 and rowDict['Object_F'] < 150:
        d.append(rowDict['DateTime'])
        d = d[-pltBufSize:]
        x.append(rowDict['Ambient_F'])
        x = x[-pltBufSize:]
        y.append(rowDict['Object_F'])
        y = y[-pltBufSize:]
        dif.append(rowDict['Diff'])
        dif = dif[-pltBufSize:]

        plt.gca().cla()  # clear the plot
        plt.plot(d, y)
        plt.plot(d, x)
        plt.plot(d,dif)
        plt.title("Ambient vs Object IR")
        plt.legend(['Sky IR F', 'Ambient IR F', 'Diff'], loc='upper left')
        plt.grid()
        plt.draw()
        plt.pause(0.1)

plt.show(block=True)  # block=True lets the window stay open at the end of the animation.
df = pd.DataFrame(rowList)

print(df)
