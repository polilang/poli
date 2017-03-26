from . import *

class ListAdd(Statement):
    """ LIST.ADD identifier, expression* """

    def __init__(self, name, args, line=0):
        self.line = line

        self.name = name
        self.args = args

    def execute(self):
        for a in self.args:
            Interpreter.Objects.put(self.name, a.evaluate(), append=True)

    @staticmethod
    def parse():
        name = Parser.consume(Type['word']).value
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

keywords.append('list.add')
nodes.update({'list.add': ListAdd})