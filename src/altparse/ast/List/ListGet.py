from . import *

class ListGet(Statement):
    """ LIST.GET identifier, expression, <identifier> """

    def __init__(self, name, index, target, line=0):
        self.line = line

        self.name = name
        self.index = index
        self.target = target

    def execute(self):
        if type(Interpreter.Objects.get(self.name)) == list:
            Interpreter.Objects.put(
                self.target,
                Interpreter.Objects.get(self.name, index = self.index.evaluate())
            )
        else:
            throw(BasicRuntimeError, 
                "object '" + self.name + "' is not a list", self)

    @staticmethod
    def parse():
        name = Parser.consume(Type['word']).value
        Parser.consume(',')
        index = Parser.expression()
        Parser.consume(',')
        target = Parser.consume(Type['word']).value
        return __class__(name, index, target, Parser.line())


    def crawl(self, stream):
        log(self.__class__.__name__ + ': ' + self.name, stream)
        aind() ; log('Index', stream)
        aind() ; self.index.crawl(stream) ; aded()
        log('Variable: ' + self.target, stream) ; aded()

keywords.append('list.get')
nodes.update({'list.get': ListGet})