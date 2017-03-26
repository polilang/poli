from . import *

class End(Statement):
    """ END expression """

    def __init__(self, expression, line=0):
        self.line = line
        self.expression = expression

    def execute(self):
        print(str(self.expression.evaluate()))
        Interpreter.State.running = False

    @staticmethod
    def parse():
        return __class__(
            Parser.expression(string=True) if not Parser.match(Type['EOL']) \
            else String(), Parser.line()
        )

    def crawl(self, stream):
        log(self.__class__.__name__, stream)
        aind() ; self.expression.crawl(stream) ; aded()

keywords.append('end')
nodes.update({'end': End})