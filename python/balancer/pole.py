class Pole():
    """ Pole of the balancer

    The pole is the swinging part of the balancer. This class includes the
    methods to interact with the pole and read the values of the pole.
    """

    def __init__(self, balancer):
        """Initiate pole

        Arguments:
            balancer {Balancer} -- Add the balancer of the pole
        """

        self.angle = 0
        self.balancer = balancer

    def reset(self):
        """ Send reset message """

        self.balancer.write("p0")

    def set_angle(self, angle):
        self.angle = angle
