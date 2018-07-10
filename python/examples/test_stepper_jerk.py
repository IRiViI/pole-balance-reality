import sys
import time
sys.path.append("../balancer")
from pole import Pole
from balancer import Balancer

balancer = Balancer(
    port='/dev/ttyACM0')

stepper_motor = balancer.stepper_motor

balancer.start()
jerk = 1
stepper_motor.stop()
time.sleep(0.1)
stepper_motor.set_speed(0.5)
while True:
    # balancer.write('test')
    # pole.reset()
    time.sleep(1)
    stepper_motor.set_jerk(0)
    time.sleep(1)
    stepper_motor.set_jerk(-jerk)
    time.sleep(1)
    stepper_motor.set_jerk(0)
    time.sleep(1)
    stepper_motor.set_jerk(jerk)    