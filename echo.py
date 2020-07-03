#!/usr/bin/env python

import serial
import curses
from curses import wrapper

def listen(on_data, device="/dev/ttyACM0", baudrate=28800):
    s = serial.Serial(device)
    s.baudrate = baudrate
    s.flushInput()

    while True:
        b = s.readline().decode('utf-8').strip()
        on_data(b)

def handle(stdscr, data):
    stdscr.clear()
    stdscr.addstr(0, 0, '+' + data[0:10] + '+')
    for i in range(1, 21):
        stdscr.addstr(i, 0, '|' + data[i*10 : (i+1)*10] + '|')
    stdscr.addstr(21, 0, '+' + '-'*10 + '+')
    stdscr.addstr(23, 0, "bytes: " + str(len(data)))
    stdscr.refresh()
     
def run(stdscr):
    try:
        stdscr.clear()
        curses.curs_set(0) 
        stdscr.addstr(0, 0, "Ctrl-c to exit")
        stdscr.refresh()
        listen(on_data = lambda data: handle(stdscr, data))

    except:
        return

if __name__ == '__main__':
    wrapper(run)

