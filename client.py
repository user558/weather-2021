#!/Users/elizavetamalyseva/opt/anaconda3/bin/python3
import json
import requests
import matplotlib.pyplot as plt
from tkinter import *
from time import sleep
from threading import Timer


def refresh():
	holst.delete("all")
	holst.create_line(3, 3, 301, 3, width=8, fill="black")
	holst.create_line(301, 3, 301, 501, width=8, fill="black")
	holst.create_line(301, 501, 3, 501, width=8, fill="black")
	holst.create_line(3, 501, 3, 3, width=8, fill="black")
	holst.create_rectangle(6, 100, 297, 400, fill="white")
	j = json.loads(requests.get('http://localhost:3000/raw').text)
	city = j['name']
	weather = j['weather']
	temp = str(j['temp'])
	holst.create_text(150, 50, text=city, font=("bold", 30))
	holst.create_text(150, 80, text=weather, font="15")
	holst.create_text(150, 250, text=(temp + "°C"), font=('bold', 50))
	holst.pack()
    
def refresh1():
    holst.create_rectangle(6, 100, 297, 400, fill="black")
    holst.pack()
    
def timing1():
    timing = time.Time()
    while True:
        if time.time() - timing > 3.0:
            timing = time.time()
            refresh()
            
def timing2():
    time.sleep(3)
    refresh()

def left_click(event):
    holst.create_rectangle(6, 100, 297, 400, fill="black")
    refresh1()

def left1_click(event):
    refresh()
   

root = Tk()
holst = Canvas(root, width=300, height=500, bg="light sky blue")
refresh()
holst.bind("<Button-1>", left_click)
holst.bind("<Double-Button-1>", left1_click)
root.mainloop()



