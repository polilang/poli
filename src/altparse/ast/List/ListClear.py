from . import *

class ListClear(Statement):
    """ LIST.CLEAR identifier """

    def __init__(self, name, line=0):
        self.line = line
        self.name = name

    def execute(self):
        Interpreter.Objects.put(self.name, [])

    @staticmethod
    def parse():
        name = Parser.consume(Type['word']).value
        return __class__(name, Parser.line())


    def crawl(self, stream):
        log(self.__class__.__name__+ ': ' + self.name, stream)

keywords.append('list.clear')
nodes.update({'list.clear': ListClear})