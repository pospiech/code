from WireClassBase import *
from pygametools import *
from math import sin, cos, radians, sqrt, pow

class WireTriangle(WireShape):
    count = 21 # PerEdge
    centerSeparation = 150 * 2
    length = 300 * 2
    isUp:bool = True
    countFirstIndexReached:int = 0
    def __init__(self):
        pass

    def description(self):
        return "WireTriangle"

    def createPositions(self):
        self.positionList = []
        d = self.centerSeparation
        l = self.length
        count = self.count + 1
        step = l/count
        positionList = []
        x = 0
        y = -d
        # first edge
        angle = 30
        phi = radians(angle)
        for i in range(0, count):
            r = i*step
            xn = x + r * sin(phi)
            yn = y + r * cos(phi)                    
            self.positionList.append(position(xn,yn))        
        # second edge    
        x = xn # restore start position    
        y = yn
        angle = - 90
        phi = radians(angle)
        for i in range(1, count):
            r = i*step
            xn = x + r * sin(phi)
            yn = y + r * cos(phi)                    
            self.positionList.append(position(xn,yn))
        # third edge    
        x = xn # restore start position    
        y = yn
        angle = 150
        phi = radians(angle)
        for i in range(1, count-1):
            r = i*step
            xn = x + r * sin(phi)
            yn = y + r * cos(phi)                    
            self.positionList.append(position(xn,yn))

        return self.positionList

    def nextPosition(self):
        if self.isUp:
            positionNext =  self.index + self.count +  self.nextPositionStep
        else:
            positionNext =  self.index - self.count #+  self.nextPositionStep
        
        if positionNext <= 0:
            positionNext = self.numberOfPositions() + positionNext

        self.isUp = not self.isUp
        
        max = self.numberOfPositions()
        if positionNext > max:
            positionNext = positionNext - max
        if  positionNext == 1:
            self.countFirstIndexReached = self.countFirstIndexReached + 1
        return positionNext

    def testLastIndex(self):
        if self.countFirstIndexReached > 1:
            self.countFirstIndexReached = 0
            return True
