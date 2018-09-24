#!/usr/bin/env python

# Thread Implementaion:
# From: http://wiki.wxpython.org/LongRunningTasks

import os
import sys
import time
import wx
import socket
import struct
from threading import *

ID_START = wx.NewId()
ID_STOP  = wx.NewId()
EVT_RESULT_ID = wx.NewId()

def EVT_RESULT(win, func):
    win.Connect(-1, -1, EVT_RESULT_ID, func)

class ResultEvent(wx.PyEvent):
    def __init__(self, data):
        wx.PyEvent.__init__(self)
        self.SetEventType(EVT_RESULT_ID)
        self.data = data

class WorkerThread(Thread):
    def __init__(self, notify_window, freq):
        Thread.__init__(self)
        self._notify_window = notify_window
        self.running = False
        self._want_abort = 0
        self._freq = freq
        self.start()
    def is_running(self):
        return self.running
    def run(self):
        self.running = True
        try:
            s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        except socket.error, e:
            sys.exit(e)

        # SiTCP RBCP Packet.
        # Version/Type: 0xff
        # Command:      0x80 (write)
        # Packet Id:    1
        # Length:       1
        # Address:      0x0
        # Data:         0x1
        request_packet = struct.pack('>BBBBIB', 0xff, 0x80, 1, 1, 0x0, 1)

        while True:
            s.sendto(request_packet, ('192.168.10.16', 4660))
            time.sleep(1/float(self._freq))
            if self._want_abort:
                wx.PostEvent(self._notify_window, ResultEvent(None))
                self.running = False
                return
        wx.PostEvent(self._notify_window, ResultEvent(10))
        self.running = False
    def abort(self):
        self._want_abort = 1

class Trigger(wx.Frame):
    def __init__(self):
        wx.Frame.__init__(self, None, title = 'Trigger Panel', size = (400, 150))
        panel = wx.Panel(self, -1)
        self.CreateStatusBar()

        label_hz = wx.StaticText(panel, -1, 'Trigger Hz', (10,10))
        self.hz = wx.TextCtrl(panel, -1, '10', (180, 10), (120, -1), style = wx.ALIGN_RIGHT)

        putStart = wx.Button(panel, ID_START,   'Start', (180, 50), (80, 30))
        putStop  = wx.Button(panel, ID_STOP ,   'Stop',  (260, 50), (80, 30))
        exit     = wx.Button(panel, wx.ID_EXIT, '',      (260, 90), (80, 30))
        self.Bind(wx.EVT_BUTTON, self.OnStart, id = putStart.GetId())
        self.Bind(wx.EVT_BUTTON, self.OnStop,  id = putStop.GetId())
        self.Bind(wx.EVT_BUTTON, self.OnExit,  id = wx.ID_EXIT)
        self.status = wx.StaticText(self, -1, '', pos=(0,100))

        EVT_RESULT(self, self.OnResult)
        self.worker = None

        self.Show(True)

    def OnStart(self, event):
        freq = self.hz.GetValue()
        if not self.worker:
            self.SetStatusText('sending trigger packet')
            self.worker = WorkerThread(self, freq)

    def OnStop(self, event):
        if self.worker:
            self.worker.abort()
            self.SetStatusText('stop trigger packet')
    
    def OnExit(self, event):
        #if self.worker:
        #    self.SetStatusText('Still sending trigger packet.  Please click stop button.')
        #    return
        if self.worker:
            self.worker.abort()
        while self.worker != None and self.worker.is_running():
            time.sleep(0.01)
            # print 'sleeping'
        self.Close()
    
    def OnResult(self, event):
        if event.data is None:
            self.status.SetLabel("aborted")
        else:
            self.status.SetLaben("Result: ")
        self.worker = None


def main():
    app = wx.App()
    Trigger()
    app.MainLoop()

if __name__ == '__main__':
    main()
