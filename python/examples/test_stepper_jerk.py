import sys
import time
sys.path.append("../balancer")
from pole import Pole
from balancer import Balancer
import random

balancer = Balancer(
    port='/dev/ttyACM0')

stepper_motor = balancer.stepper_motor

balancer.start()
jerk = 30
stepper_motor.stop()
time.sleep(0.5)
# time.sleep(0.1)
stepper_motor.set_speed(0.5)
step = 1

current_state = [0,0,0,0,0]

def handle_state(state):
    global current_state
    current_state = state
    print(state)

balancer.onstate = handle_state


while True:
    # balancer.write('test')
    # pole.reset()
    time.sleep(step)
    # stepper_motor.set_jerk(0)
    # time.sleep(step)
    stepper_motor.set_jerk(-jerk)
    time.sleep(step)
    stepper_motor.set_jerk(jerk)
    # stepper_motor.set_jerk(0)
    # time.sleep(step)
    # if current_state[2] > 0:
    #     stepper_motor.set_jerk(-jerk)    
    # elif current_state[2] < 0 :
    #     stepper_motor.set_jerk(jerk)    
    # # else: 
    # if random.choice([True, False]):
    #     stepper_motor.set_jerk(-jerk)
    # else: 
    #     stepper_motor.set_jerk(jerk)
    balancer.get_state()
