'''
  Python Multicopter class

  Uses UDP sockets to communicate with MulticopterSim

  Copyright(C) 2019 Simon D.Levy

  MIT License
'''

from threading import Thread
import socket
import numpy as np
import cv2
from sys import stdout
import time


class Multicopter(object):
    '''
    Represents a Multicopter object communicating with MulticopterSim via UDP
    socket calls.
    '''

    # 12-dimensional state vector (Bouabdallah 2004)
    STATE_SIZE = 12

    # Image size: should match whatever is being sent by sim
    IMAGE_ROWS = 480
    IMAGE_COLS = 640

    def __init__(self, host='127.0.0.1', motorPort=5000, telemetryPort=5001,
                 imagePort=5002, motorCount=4, timeout=.1):
        '''
        Creates a Multicopter object.
        host - name of host running MulticopterSim
        motorPort - port over which this object will send motor commands to
                    host
        telemeteryPort - port over which this object will receive telemetry
                         from host
        motorCount - number of motors in vehicle running in simulator on host
        '''

        self.host = host
        self.motorPort = motorPort
        self.motorCount = motorCount

        # Telemetry in and motors out run on their own thread
        self.motorClientSocket = Multicopter._make_udpsocket()
        self.telemetryServerSocket = Multicopter._make_udpsocket()
        self.telemetryServerSocket.bind((host, telemetryPort))
        self.telemetryThread = Thread(target=self._run_telemetry)

        # Imaging runs on its own thread
        self.imageServerSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.imageServerSocket.bind((host, imagePort))
        self.imageServerClient = None
        self.imageThread = Thread(target=self._run_imaging)

        # Telemetry contains time value followed by state vector
        self.telemSize = self.STATE_SIZE + 1

        self.motorVals = np.zeros(motorCount)
        self.telem = np.zeros(self.telemSize)

        self.timeout = timeout
        self.ready = False
        self.done = False

    def start(self):
        '''
        Begins communication with simulator running on host.
        '''

        Multicopter.debug('Hit the start button ... ')
        self.telemetryThread.start()
        self.imageThread.start()

    def isReady(self):

        try:
            if self.ready:
                self.telemetryServerSocket.settimeout(self.timeout)
            return self.ready
        except Exception:
            self.done = True

        return self.ready

    def isDone(self):

        return self.done

    def getTime(self):
        '''
        Returns current time from sim
        '''

        return self.telem[0]

    def getState(self):
        '''
        Returns current vehicle state from sim
        '''

        return self.telem[1:]

    def getImage(self):
        '''
        Returns current image as an RxCx4 numpy array
        '''
        return self.image

    def setMotors(self, motorVals):
        '''
        Sets motor values between 0 and 1.
        '''

        self.motorVals = np.copy(motorVals)

    @staticmethod
    def debug(msg):

        print(msg)
        stdout.flush()

    @staticmethod
    def _make_udpsocket():

        sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, True)

        return sock

    def _run_telemetry(self):

        self.done = False
        running = False

        while True:

            try:
                data, _ = self.telemetryServerSocket.recvfrom(8*self.telemSize)
            except Exception:
                self.done = True
                break

            self.telem = np.frombuffer(data)

            self.ready = True

            if not running:
                Multicopter.debug('Running')
                running = True

            if self.telem[0] < 0:
                self.motorClientSocket.close()
                self.telemetryServerSocket.close()
                self.done = True
                break

            self.motorClientSocket.sendto(np.ndarray.tobytes(self.motorVals),
                                    (self.host, self.motorPort))

    def _run_imaging(self):

        return
        self.imageServerSocket.listen(1)
        conn, _ = self.imageServerSocket.accept()
        conn.settimeout(1)

        while True:

            try:
                imgbytes = conn.recv(self.ROWS*self.COLS*4)

            except Exception:  # likely a timeout from sim quitting
                pass