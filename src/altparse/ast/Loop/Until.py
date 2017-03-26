from . import *

class Until(Statement):
    """ UNTIL expression """

    def __init__(self, expression, line=0):
        self.line = line
        self.expression = expression

    def execute(self):
        if Interpreter.State.name('loop') == 'doloop':
            loopd = Interpreter.State.get('loop')[0]
            if not self.expression.evaluate():
                Interpreter.index = Interpreter.State.get('loop')[1]
            else:
                loopd.running = False
                Interpreter.State.pop('loop')
        else:
            throw(BasicRuntimeError, 'UNTIL outside of a DO loop', self)

    @staticmethod
    def parse():
        return __class__(Parser.expression(), Parser.line())

    def crawl(self, stream):
        aded() ; log(self.__class__.__name__, stream)
        aind() ; self.expression.crawl(stream) ; aded()

keywords.append('until')
nodes.update({'until': Until})