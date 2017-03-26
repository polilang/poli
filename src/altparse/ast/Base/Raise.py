from . import *

class Raise(Statement):
    """ RAISE expression """

    def __init__(self, expression, line=0):
        self.line = line
        self.expression = expression

    def execute(self):
        throw(BasicRuntimeError, 
            str(self.expression.evaluate()), self)

    @staticmethod
    def parse():
        return __class__(Parser.expression(string=True), Parser.line())

    def crawl(self, stream):
        log(self.__class__.__name__, stream)
        aind() ; self.expression.crawl(stream) ; aded()

keywords.append('raise')
nodes.update({'raise': Raise})