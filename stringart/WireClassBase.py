from abc import ABC, abstractmethod

class WireShape(ABC):

    positionList =[]
    index = 1
    nextPositionStep = 1

    @abstractmethod
    def createPositions(self, count, parameter):
        pass

    @abstractmethod
    def restartWithNextPositionStep(self):
        pass
    
    def nextPosition(self):
        pass

    def numberOfPositions(self):
        return len(self.positionList)
    
    def testLastIndex(self):
        pass

    def description(self):
        pass