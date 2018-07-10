class StepperMotor():
    """ Pole of the balancer

    The pole is the swinging part of the balancer. This class includes the
    methods to interact with the pole and read the values of the pole.
    """

    def __init__(self, balancer):
        """Initiate pole

        Arguments:
            balancer {Balancer} -- Add the balancer of the pole
        """

        self.speed = 0
        self.balancer = balancer

    def stop(self):
        """ Send reset message """

        self.balancer.write("mb")

    def rotate(self):
        """ Send reset message """

        self.balancer.write("md")

    def set_speed(self, speed):
        if speed > 1:
            raise ValueError("speed can not exceed 1")
        if speed < -1:
            raise ValueError("speed can not be lower than 1")
        byteValue = chr(int((speed + 1)/ 2 * 126 ) )
        self.balancer.write("ms{}".format(byteValue))

    def set_jerk(self, jerk):
        "-63 < jerk < 63"
        byteValue = chr(int((jerk + 63)))
        self.balancer.write("mj{}".format(byteValue))

