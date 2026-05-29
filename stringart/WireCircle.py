from WireClassBase import *
from pygametools import *
from math import sin, cos, radians, sqrt, pow

class WireCircle(WireShape):
    radius = 150*2
    count = 24*2
    isLastIndex:bool = False
    def __init__(self):
        pass

    def description(self):
        return "WireCircle"        

    def createPositions(self):
        self.positionList = []
        print ("angle step: " + str(360/self.count) )
        for i in range(0, self.count):
            phi = radians(360/self.count*i)
            x = self.radius * sin(phi)
            y = self.radius * cos(phi)
            
            self.positionList.append(position(x,y))
        return self.positionList

    def indexMirrorPosition(self):
        max = self.numberOfPositions()
        halfmax = int(max/2)
        # mirror position is half of the circle
        # + last index position
        positionMirror = halfmax + self.index
        if positionMirror > max:
            positionMirror = positionMirror - max
        return positionMirror

    def nextPosition(self):
        positionNext = self.indexMirrorPosition()
        positionNext = positionNext  + self.nextPositionStep
        max = self.numberOfPositions()        
        if positionNext > max:
            positionNext = positionNext - max
        if positionNext == 1:
            self.isLastIndex = True
        else:
            self.isLastIndex = False
        return positionNext

    def testLastIndex(self):
        return self.isLastIndex