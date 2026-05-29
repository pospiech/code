import pygame, sys
from pygame.locals import *
from pygametools import *
import os
from screeninfo import get_monitors
import sys
from pathlib import Path

# Add the current directory to the path to resolve local imports
sys.path.insert(0, str(Path(__file__).parent))

from WireTest import *
from WireCircle import *
from WireTriangle import *
from WireCircleNonEqualSteps import *

def main():
    # set window center
    window_width = 800
    window_height = 800
    # Get the bounds of the users monitors, and select the first one
    monitors = get_monitors() # Get the resolution of all of the users monitors
    screen_width = monitors[0].width # Get width of first monitor found
    screen_height = monitors[0].height # Get height of first monitor found    
    pos_x = screen_width * 8/10 - window_width / 2
    pos_y = screen_height - window_height
    os.environ['SDL_VIDEO_WINDOW_POS'] = '%i,%i' % (pos_x,pos_y)
    os.environ['SDL_VIDEO_CENTERED'] = '0'
    

    pygame.init()
    pygame.display.set_caption("Test Wire Images")
    pygame.font.init()     
    screen = pygame.display.set_mode((window_width,window_height))
    screen.fill(white)

    circleShape = WireCircleNonEqualSteps(6)
    circleShape.createPositions()    
    circleShape.startIndex2Offset = 24
    wireTest = WireTest(screen, circleShape)
    wireTest.stepList = [0, 1, 2, 3, 4, 5, 6, 7]
    wireTest.shape.nextPositionStep = wireTest.stepList[0]
    wireTest.shape.restartWithNextPositionStep()
    wireTest.initialiseScreen()
    
    count = 0
    while True:
        for event in pygame.event.get():
            if event.type == QUIT:
                pygame.quit()
                sys.exit()
        
        # increase nextPositionStep, break on error
        if not wireTest.actionNextIteration():
            break        
        
        index2 = wireTest.nextPosition()
        wireTest.highlightcircle(index2)
        count = count + 1
        # if next = start
        if wireTest.shape.testLastIndex():
            wireTest.nextIteration = True                
            count = 0

        # print(wireTest.shape.index)
        # print(index2)        

        wireTest.drawLine(index2)
        wireTest.setIndex(index2)        

        # createScreenshotCount(wireTest.screen, wireTest.shape.nextPositionStep,count, wireTest.shape.description())
        pygame.display.flip()
        pygame.time.delay(10)
        wireTest.defaultcircle(index2)
        
    createScreenshotCount(wireTest.screen, wireTest.shape.nextPositionStep,count, wireTest.shape.description())
        
if __name__ == "__main__":
    main()

