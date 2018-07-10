import serial
import threading
import json
from pole import Pole
from stepperMotor import StepperMotor


class Balancer():
    """ balancer class

    This class containing all the parts of the balancer and performs the
    actions needed to keep everything running smoothly
    """

    def __init__(self,
                 port='/dev/ttyACM0',
                 baud=9600,
                 byte_length=3
                 ):
        """ Initate balancer

        Keyword Arguments:
            port {str} -- serial port used for the arduino
                (default: {'/dev/ttyACM0'})
            baud {number} -- serial port number used for the arduino
                (default: {9600})
            byte_length {number} -- Length of messages to the arduino
                (default: {3})
        """

        # Length of messages to the arduino
        self.byte_length = byte_length

        # Serial object
        self.ard = serial.Serial(port, baud)
        # Pole object
        self.pole = Pole(self)
        # Pole object
        self.stepper_motor = StepperMotor(self)

        # Private variables
        self._running = False

    def run(self):
        """ Run the balancer object while loop

        Run all the processes to keep the balancer alive and well
        """

        # While the balancer is still running
        while self._running:
            self.single_run()

    def single_run(self):
        """ Perform one round of all the balancer processes """

        self._read_all()

    def start(self):
        """ Start the balancer

        Initiate and run all the balancer processes

        Raises:
            RuntimeError -- [description]
        """

        # Check if the balancer is already running
        if self._running is True:
            raise RuntimeError('Balancer already running')

        # Set the balancer to running
        self._running = True

        # Create and start the running loop thread
        self.thread = threading.Thread(target=self.run)
        self.thread.start()

    def stop(self):
        """ Stop the balancer """

        self._running = False

    def write(self, text):
        """ Send text message to the arduino

        Send a message with the correct length to the arduino. Shorter messages
        Will be elongaded until they reached the correct length.

        Arguments:
            text {[type]} -- [description]
        """

        # Make sure the text has the correct length
        while len(text) < self.byte_length:
            text += ' '

        # Check if the message is too long
        if len(text) > self.byte_length:
            raise ValueError("Length of text is too long")

        # Send message to the Arduino
        self.ard.write(text.encode('utf-8'))

    def _read_all(self):
        """ Wait for incomming message and handle them propperly"""

        # Translate the message
        raw = self.ard.readline()
        if not raw:
            raise RuntimeError("something")


        try :
            # Convert the message
            msg = json.loads(raw.decode().replace('\r\n', ''))
            # Handle the message
            pole_agle = msg[0]
            print(msg)
        except:
            print('not done', raw)
            return


        # Set pole anlge
        self.pole.set_angle(pole_agle)
