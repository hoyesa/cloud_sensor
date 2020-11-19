from selenium import webdriver
import time
import pandas as pd

url = 'http://192.168.8.236'

refreshrate = 5  # sample rate in seconds
driver = webdriver.Chrome('C:/Users/hoyes/Documents/libs/chromedriver.exe')
driver.get(url)

rowList = []
for _ in range(20):
    time.sleep(refreshrate)
    driver.refresh()
    table = pd.read_html(driver.page_source,header=0)[0]
    rowDict = {col:val[0] for col,val in table.iteritems()}
    rowDict['DateTime'] = pd.Timestamp.now()
    rowList.append(rowDict)

df = pd.DataFrame(rowList)

print(df)
