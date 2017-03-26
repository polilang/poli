from . import *

class Repeat(Statement):
    """ REPEAT """

    def execute(self):
        if Interpreter.State.name('loop') == 'whileloop':
            loopd = Interpreter.State.get('loop')[0]
            if loopd.expression.evaluate():
                Interpreter.index = Interpreter.State.get('loop')[1]
            else:
                loopd.running = False
                Interpreter.State.pop('loop')
        else:
            throw(BasicRuntimeError, 'REPEAT outside of a WHILE loop', self)

    @staticmethod
    def parse():
        return __class__(Parser.line())

    def crawl(self, stream):
        aded() ; log(self.__class__.__name__, stream)

keywords.append('repeat')
nodes.update({'repeat': Repeat})