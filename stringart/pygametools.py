from turtle import color

import pygame

white = (255, 255, 255)
red = (255, 0, 0)
blue = (0, 0, 255)
green = (0, 255, 0)
black = (0, 0, 0)

# colorList = [red, blue, green]

color_Red = (255, 0, 0)
color_Red_Orange =(255, 85, 0)
color_Orange_Yellow =(255, 170, 0)
color_Yellow = (255, 255, 0)
color_Yellow_Green = (170, 255, 0)
color_Green_Yellow = (85, 255, 0)
color_Green = (0, 255, 0)
color_Cyan_Green = (0, 255, 85)
color_Cyan = (0, 255, 255)
color_Blue = (0, 0, 255)
color_Magenta =	(255, 0, 255)
color_Red_Magenta =	(255, 0, 85)

colorList1 = [color_Red, color_Red_Orange, color_Orange_Yellow, color_Yellow, color_Yellow_Green, color_Green_Yellow, color_Green, color_Cyan_Green, color_Cyan, color_Blue, color_Magenta, color_Red_Magenta]
colorList2= [color_Red, color_Orange_Yellow, color_Yellow, color_Green_Yellow, color_Cyan_Green, color_Blue, color_Red_Magenta]
colorList3= [color_Red, color_Yellow, color_Green, color_Cyan, color_Blue, color_Red_Magenta]

# 8 visually distinct colors from deep blue toward red with roughly equal perceived brightness
colorListSpectrum8 = [
    (117, 117, 255),
    (10, 150, 255),
    (0, 163, 140),
    (0, 173, 50),
    (47, 163, 0),
    (122, 143, 0),
    (204, 117, 0),
    (255, 92, 92),
]

colorListSpectrum = [
    (105, 52, 157), # deep purple
    (107, 82, 250), # purple
    (67, 104, 249), # deep blue
    (52, 137, 249), # blue
    (52, 173, 249), # cyan
    (87, 137, 124), # dark green
    (112, 176, 86), # green
    (171, 220, 77), # light green
    (249, 239, 57), # yellow
    (237, 203, 49), # orange
    (255, 148, 39), # red orange
    (212, 97, 31), # red
    (212, 65, 26), # deep red
    (145, 68, 166), # magenta
]

colorListSpectrum8 = [
    (105, 52, 157), # deep purple
    (107, 82, 250), # purple
    (67, 104, 249), # deep blue
    # (52, 137, 249), # blue
    (52, 173, 249), # cyan
    # (87, 137, 124), # dark green
    (112, 176, 86), # green
    # (171, 220, 77), # light green
    # (249, 239, 57), # yellow
    (237, 203, 49), # orange
    (255, 148, 39), # red orange
    # (212, 97, 31), # red
    (212, 65, 26), # deep red
    # (145, 68, 166), # magenta
]


class position:    
    def __init__(self, x = 0, y = 0):
        self.x = x
        self.y = y
        
    def xy(self):
        return (self.x, self.y)

def transformPosition(screen, x,y):
    ORIGINX = screen.get_width()/2
    ORIGINY = screen.get_height()/2
    x += ORIGINX
    y += ORIGINY
    return (x,y)

def createScreenshot(screen, index, imagePreText):
    screenshot = pygame.Surface(screen.get_size())
    screenshot.blit(screen, (0, 0))
    imagename = imagePreText + str(index) + ".png"
    pygame.image.save(screenshot, imagename)

def createScreenshotCount(screen, index, count, imagePreText):
    screenshot = pygame.Surface(screen.get_size())
    screenshot.blit(screen, (0, 0))
    imagename = imagePreText + str(index) +"-"+ str(count) + ".png"
    pygame.image.save(screenshot, imagename)


# print text to top of screen
def printTexttop(screen, textString):
    my_font = pygame.font.SysFont('Frutiger', 30)
    text_surface = my_font.render(textString, False, (0, 0, 0))
    screen.blit(text_surface, (0,0))

def printTextPos(screen, textString, x, y):
    my_font = pygame.font.SysFont('Frutiger', 30)
    text_surface = my_font.render(textString, False, (0, 0, 0))
    (x,y) = transformPosition(screen, x,y)
    screen.blit(text_surface, (x,y))


def drawpositionCircle(screen, realPosition, color = red):
    pygame.draw.circle(screen, color, realPosition, 2, 1)
