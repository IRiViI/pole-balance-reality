import sys
import time
sys.path.append("../balancer")
from pole import Pole
from balancer import Balancer

balancer = Balancer(
    port='/dev/ttyACM0')

stepper_motor = balancer.stepper_motor

balancer.start()
waiting_time = 0.3
speed = 0.2
while True:
    time.sleep(waiting_time)
    stepper_motor.set_speed(0)
    time.sleep(waiting_time)
    stepper_motor.set_speed(speed)
    time.sleep(waiting_time)
    stepper_motor.set_speed(0)
    time.sleep(waiting_time)
    stepper_motor.set_speed(-speed)    