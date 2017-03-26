from . import *

class ListRemove(Statement):
    """ LIST.REMOVE identifier, expression """

    def __init__(self, name, index, line=0):
        self.line = line

        self.name = name
        self.index = index

    def execute(self):
        Interpreter.Objects.remove(self.name, index=self.index.evaluate())

    @staticmethod
    def parse():
        name = Parser.consume(Type['word']).value
        Parser.consume(',')
        index = Parser.expression()

        return __class__(name, index, Parser.line())


    def crawl(self, stream):
        log(self.__class__.__name__, stream)
        aind()
        
        log(self.name, stream)
        aind()

        log('Index', stream)
        aind() ; self.index.crawl(stream) ; aded()

        aded()
        aded()

keywords.append('list.remove')
nodes.update({'list.remove': ListRemove})