import pygame
import serial
import time
arduino = serial.Serial("COM5", 115200, timeout=1)
time.sleep(2)
pygame.init()
pygame.joystick.init()
if pygame.joystick.get_count() == 0:
    print("Gamepad not found!")
    exit()
gamepad = pygame.joystick.Joystick(0)
gamepad.init()
print(f"Connected: {gamepad.get_name()}")
while True:
    pygame.event.pump()
    x_axis = int(gamepad.get_axis(0) * 90)
    y_axis = int(gamepad.get_axis(1) * 90)
    fire_button = gamepad.get_button(0)
    command = f"{x_axis},{y_axis},{fire_button}\n"
    arduino.write(command.encode())

    time.sleep(0.05)

