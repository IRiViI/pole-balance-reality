#! /usr/bin/env python
# -*- coding: utf-8 -*-

import sys
import mepy
import time
sys.path.append("../balancer")
from pole import Pole
from balancer import Balancer

balancer = Balancer(
    port='/dev/ttyACM1')

stepper_motor = balancer.stepper_motor

balancer.start()
stepper_motor.stop()   

# Program settings
_id = '5b432cbfa9247b4ce6394f61'
key = 'L2t9gFlzl3x2hZo'


def handle_project(project):
    """Process newly obtained project

    Add handling to newly obtained projects

    Arguments:
        project {Project} -- project to be handled
    """

    text = 'You joined project {}'.format(project.name)
    print(text)


def handle_remote_program(remote_program):
    """ Handle programs of the project """

    text ='You are connected with program {}'.format(remote_program.name)
    print(text)


def handle_send_jerk_message(message):
    """Handle a send jerkmessage"""

    # connection = message.connection
    jerk = float(message.body)
    if jerk == 0:
        stepper_motor.stop()
    else:
        stepper_motor.set_jerk(jerk*10)


if __name__ == '__main__':

    # Create program object
    program = mepy.Program(
        _id=_id,
        key=key)

    # Add message handlers
    program.on_send_message('jerk', handle_send_jerk_message)

    # Handle newly connected programs
    program.on_remote_program(handle_remote_program)

    # Handle newly obtained projects
    program.on_project(handle_project)

    # Start program
    program.start()

    print('Hello {}'.format(program.name))

    # Keep it on
    while True:
        try:
            time.sleep(0.1)
        except KeyboardInterrupt:
            print('Bye bye')
            sys.exit(0)
