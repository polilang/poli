from . import *

class Do(Statement):
    """ DO
          { block }
        UNTIL expression
    """

    def __init__(self, line=0):
        self.line = line
        self.running = False

    def execute(self):
        if not self.running:
            self.running = True
            Interpreter.State.push('loop', 'doloop', [self, Interpreter.index])

    @staticmethod
    def parse():
        return __class__(Parser.line())

    def crawl(self, stream):
        aind() ; log(self.__class__.__name__, stream)

keywords.append('do')
nodes.update({'do': Do})