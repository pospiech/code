import pygame
from pygame.locals import *
from pygametools import *
import math

colorList =colorListSpectrum8

# Initialize pygame
pygame.init()

# Screen settings
WIDTH = 800
HEIGHT = 800
screen = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("Color Wheel")
clock = pygame.time.Clock()

# Color wheel settings
CENTER_X = WIDTH // 2
CENTER_Y = HEIGHT // 2
WHEEL_RADIUS = 250
COLOR_RADIUS = 30

# Draw color wheel
def draw_color_wheel(screen, colors):
    num_colors = len(colors)
    for i, color in enumerate(colors):
        angle = (i / num_colors) * 2 * math.pi
        x = CENTER_X + WHEEL_RADIUS * math.cos(angle)
        y = CENTER_Y + WHEEL_RADIUS * math.sin(angle)
        pygame.draw.circle(screen, color, (int(x), int(y)), COLOR_RADIUS)

# Main loop
running = True
while running:
    clock.tick(60)
    screen.fill(white)
    
    # Draw color wheel
    draw_color_wheel(screen, colorList)
    
    # Draw center circle
    pygame.draw.circle(screen, black, (CENTER_X, CENTER_Y), 10)
    
    for event in pygame.event.get():
        if event.type == QUIT:
            running = False
    
    pygame.display.flip()

pygame.quit()

