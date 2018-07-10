import sys
import time
sys.path.append("../balancer")
from pole import Pole
from balancer import Balancer

balancer = Balancer(
    port='/dev/ttyACM0')


balancer.start()

while True:
    # balancer.write('test')
    # pole.reset()
    time.sleep(0.5)
    print('pole angle', balancer.pole.angle)