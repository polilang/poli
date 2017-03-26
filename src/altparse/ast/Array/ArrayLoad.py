from . import *

class ArrayLoad(Statement):
    """ ARRAY.LOAD identifier, expression* """

    def __init__(self, name, args, line=0):
        self.line = line

        self.name = name
        self.args = args

    def execute(self):
        if self.name[-1] == '$':
            array = StrArray(self.name, ArrayDim([Number()])).evaluate()
        else:
            array = NumArray(self.name, ArrayDim([Number()])).evaluate()

        for a in self.args:
            array.extend([a.evaluate()])

    @staticmethod
    def parse():
        name = Parser.consume(Type['word']).value
        Parser.match('[', ']')
        args = list()
        while not Parser.match(Type['EOL']) and Parser.match(','):
            args.append(Parser.expression())

        return __class__(name, args, Parser.line())

    def crawl(self, stream):
        log(self.__class__.__name__ + ': ' + self.name, stream)
        aind()

        for arg in self.args:
            arg.crawl(stream)

        aded()

keywords.append('array.load')
nodes.update({'array.load': ArrayLoad})