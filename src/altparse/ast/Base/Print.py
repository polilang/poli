from . import *

class Print(Statement):
    """ PRINT expression """

    def __init__(self, expression, line=0):
        self.line = line
        self.expression = expression

    def execute(self):
        string = str(self.expression.evaluate())
        if string[-len('\\<\\cont\\>\\'):] == '\\<\\cont\\>\\':
            print(string[:-len('\\<\\cont\\>\\')], end='')
        else:
            print(string)

    @staticmethod
    def parse():
        return __class__(
            Parser.expression(string=True) if not Parser.match(Type['EOL']) \
            else String(), Parser.line()
        )

    def crawl(self, stream):
        log(self.__class__.__name__, stream)
        aind() ; self.expression.crawl(stream) ; aded()

keywords.append('print')
keywords.append('?')
nodes.update({'print': Print, '?': Print})