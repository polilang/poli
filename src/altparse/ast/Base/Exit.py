from . import *

class Exit(Statement):
    """ EXIT """

    def execute(self):
        Interpreter.State.running = False

    @staticmethod
    def parse():
        return __class__(Parser.line())

keywords.append('exit')
nodes.update({'exit': Exit})