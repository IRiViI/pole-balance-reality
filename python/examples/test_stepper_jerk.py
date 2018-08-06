import sys
import time
sys.path.append("../balancer")
from pole import Pole
from balancer import Balancer

balancer = Balancer(
    port='/dev/ttyACM0')

stepper_motor = balancer.stepper_motor

balancer.start()
jerk = 10
stepper_motor.stop()
time.sleep(0.1)
stepper_motor.set_speed(0.5)
step = 0.1

balancer.onstate = lambda state: print('yo mamma', state)


while True:
    # balancer.write('test')
    # pole.reset()
    time.sleep(step)
    stepper_motor.set_jerk(0)
    time.sleep(step)
    stepper_motor.set_jerk(-jerk)
    time.sleep(step)
    stepper_motor.set_jerk(0)
    time.sleep(step)
    stepper_motor.set_jerk(jerk)    

    balancer.get_state()
