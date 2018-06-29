import sys
import time
sys.path.append("../balancer")
from pole import Pole
from balancer import Balancer

balancer = Balancer()

pole = balancer.pole

balancer.start()

while True:
    position = pole.position
    # balancer.write('test')
    # pole.reset()
    time.sleep(2)