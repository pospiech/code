
import pygame
from pygame.locals import *
from pygametools import *

colorList = colorListSpectrum8

class WireTest():
    colorIndex: int = 1    
    count:int = 0
    countThreshold: int = 1000
    isCreateScreenshot: bool = False
    gameDelay: int = 1000
    nextIteration: bool = False
    color = colorList[0]
    stepList = []
    stepIndex = 0
    isDrawTextPosition = False

    def __init__(self, newScreen, newShape):    
        self.screen = newScreen
        self.shape = newShape        
    

    def actionNextIteration(self):
        self.count = self.count + 1        
        if self.nextIteration or self.count > self.countThreshold:
            # create screenshot first
            if self.isCreateScreenshot:                
                createScreenshot(self.screen, self.shape.nextPositionStep, self.shape.description())
            # increase and reset numbers
            if len(self.stepList) == 0:
                self.shape.nextPositionStep = self.shape.nextPositionStep + 1
            else:
                self.stepIndex = self.stepIndex + 1
                if self.stepIndex == len(self.stepList):
                    return False
                self.shape.nextPositionStep = self.stepList[self.stepIndex]
            # restart with next position step
            self.shape.restartWithNextPositionStep()

            self.colorIndex = self.colorIndex + 1     
            
            if self.colorIndex > len(colorList):
                self.colorIndex = len(colorList) - self.colorIndex
            self.color = colorList[self.colorIndex-1]
            print(self.color)
            self.nextIteration = False
            self.count = 0
            
            if self.shape.nextPositionStep > self.shape.numberOfPositions():
                return False
            pygame.time.delay(self.gameDelay)
            # if len(self.stepList) == 0:
            # self.initialiseScreen()
            if self.isDrawTextPosition:
                self.printTextStep()
            return True
        else:
            return True

    def drawLine(self, index2):
            index = self.shape.index
            (x, y) = self.shape.positionList[index-1].xy()
            (x2, y2) = self.shape.positionList[index2-1].xy()
                    
            (x, y) = transformPosition(self.screen, x,y)
            (x2, y2) = transformPosition(self.screen, x2,y2)    

            pygame.draw.line(self.screen, self.color, (x, y), (x2, y2),1) # thickness 1

    # fill screen white background
    # print nail dots -> positionList
    def initialiseScreen(self):
        color = black
        self.screen.fill(white)
        count = 1
        for entry in self.shape.positionList:                        
            realPosition = transformPosition(self.screen,entry.x, entry.y)
            drawpositionCircle(self.screen, realPosition)            
            if self.isDrawTextPosition:
                printTextPos(self.screen, str(count), entry.x, entry.y)
            count = count + 1

        # self.printTextStep()


    def highlightcircle(self, count):
        entry = self.shape.positionList[count-1]
        realPosition = transformPosition(self.screen,entry.x, entry.y)
        drawpositionCircle(self.screen, realPosition)            
        pygame.draw.circle(self.screen, blue, realPosition, 6, 2)

    def defaultcircle(self, count):
        entry = self.shape.positionList[count-1]
        realPosition = transformPosition(self.screen,entry.x, entry.y)
        drawpositionCircle(self.screen, realPosition)            
        pygame.draw.circle(self.screen, white, realPosition, 6, 2)
        pygame.draw.circle(self.screen, red, realPosition, 2, 1)


    def printTextStep(self):            
        textString = "count: " + str(self.shape.count) + ",step: " + str(self.shape.nextPositionStep)
        printTexttop(self.screen, textString)

    def nextPosition(self):
        index = self.shape.nextPosition()
        return index
    
    def setIndex(self, index):
        if index > 0:
            self.shape.index = index
        else: 
            raise ValueError('Index must be > 0')