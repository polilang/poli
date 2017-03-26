from . import *

from .Repeat import Repeat

class While(Statement):
    """ WHILE expression
          { block }
        REPEAT
    """

    def __init__(self, expression, line=0):
        self.line = line
        self.expression = expression
        self.running = False

    def execute(self):
        if not self.running:
            if not self.expression.evaluate():
                while not type(Interpreter.ast[Interpreter.index]) == Repeat:
                    Interpreter.index += 1
                #Interpreter.index += 1
                return

            self.running = True
            Interpreter.State.push('loop','whileloop', [self, Interpreter.index])

    @staticmethod
    def parse():
        return __class__(Parser.expression(), Parser.line())

    def crawl(self, stream):
        log(self.__class__.__name__, stream)
        aind() ; self.expression.crawl(stream)

keywords.append('while')
nodes.update({'while': While})