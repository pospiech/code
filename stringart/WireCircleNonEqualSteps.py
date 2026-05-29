from WireClassBase import *
from pygametools import *
from math import sin, cos, radians, sqrt, pow

class WireCircleNonEqualSteps(WireShape):
    radius:int = 150*2
    count:int = 24*2
    isLastIndex:bool = False
    index1:int = 0
    index2:int = 0
    startIndex1:int = 0
    startIndex2:int = 0
    step1:int = 2   
    step2:int = 1
    iteration:int = 0    
    predefinedPartial:int = 4
    startIndex2Offset:int = 0
    
    def __init__(self, partial):
        # numberofpositions is not valid at constructor!
        self.predefinedPartial = partial
        self.setOffsetAsPartial(self.predefinedPartial)        

    def description(self):
        return "WireCircleNonEqualSteps-" " Offset " + str(self.startIndex2Offset) + "-Step1 " + str(self.step1) + " Step2 " + str(self.step2) + " -"        

    def correctIndex(self, index):
        max = self.numberOfPositions()        
        if max > 0:
            if index > max:
                index = index - max 
            if index < 1:
                index = 1 # index starts with 1, so if index is 0, then next index is 1, not 0            
            # print("index corrected: " + str(index))
        return int(index)

    def setOffsetAsPartial(self, partial, multiplyer = 0):
        # position start is defined by offset defined by partial
        self.startIndex1 = self.correctIndex(int(self.count/partial*multiplyer) + 1)
        # second start index is half of the circle + index1 with defined offset
        self.startIndex2 = self.correctIndex(self.startIndex1 + int((self.count/2) +  self.startIndex2Offset))
        # access WireShape index to correct start position
        self.index = self.startIndex1

    def createPositions(self):
        self.positionList = []
        print ("angle step: " + str(360/self.count) )
        for i in range(0, self.count):
            phi = radians(360/self.count*i)
            x = self.radius * sin(phi)
            y = self.radius * cos(phi)
            
            self.positionList.append(position(x,y))
        return self.positionList


    def restartWithNextPositionStep(self):
        multiplier = self.nextPositionStep
        self.setOffsetAsPartial(self.predefinedPartial, multiplier)
        self.index1 = 0
        self.index2 = 0
        self.iteration = 0
        self.isLastIndex = False

    def nextPosition(self):
        self.iteration +=1
        if self.iteration == 1:
            self.index2 = self.startIndex2 # froward, typically half of the circle
            self.index1 = self.startIndex1 # back, start with index 0, with offset defined by partial
            # return next position for first iteration,
            # first position is defined by start index, which is defined by offset and partial
            return self.index2
        
        # alternate between step1 and step2
        # Step 1 (mod 0) is back, step 2 is forward
        if self.iteration % 2 == 0:
            positionNext = self.correctIndex(self.index1 + self.step1)
            self.index1 = positionNext
            # next position must not be the previous position
            if positionNext == self.index2:
                self.isLastIndex = True
        else:
            positionNext = self.correctIndex(self.index2 + self.step2)
            self.index2 = positionNext
            # next position must not be the previous position
            if positionNext == self.index1:
                self.isLastIndex = True
        
        return positionNext

    def testLastIndex(self):
        return self.isLastIndex