from . import *

class FnRtn(Statement):
    """ FN.RTN expression """

    def __init__(self, expression, line=0):
        self.line = line
        self.expression = expression

    def execute(self):
        if Interpreter.State.name('func') == 'functioncall':
            rtn = self.expression.evaluate()
            index, call = Interpreter.State.pop('func')
            Interpreter.Objects.popspace()
            Interpreter.index = index

        else:
            throw(BasicRuntimeError, 'outside function call', self)

        return rtn

    def evaluate(self):
        return self.execute()

    @staticmethod
    def parse():
        return __class__(Parser.expression(), Parser.line())

    def crawl(self, stream):
        log(self.__class__.__name__, stream)
        aind() ; self.expression.crawl(stream) ; aded()

keywords.append('fn.rtn')
nodes.update({'fn.rtn': FnRtn})