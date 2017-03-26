from . import *

class ListSize(Statement):
    """ LIST.SIZE identifier, <identifier> """

    def __init__(self, name, target, line=0):
        self.line = line

        self.name = name
        self.target = target

    def execute(self):
        if type(Interpreter.Objects.get(self.name)) == list:
            Interpreter.Objects.put(
                self.target, len(Interpreter.Objects.get(self.name))
            )
        else:
            throw(BasicRuntimeError, 
                "object '" + self.name + "' is not a list", self)

    @staticmethod
    def parse():
        name = Parser.consume(Type['word']).value
        Parser.consume(',')
        target = Parser.consume(Type['word']).value

        return __class__(name, target, Parser.line())

    def crawl(self, stream):
        log(self.__class__.__name__ + ': ' + self.name, stream)
        aind() ; log('Variable: ' + self.target, stream) ; aded()

keywords.append('list.size')
nodes.update({'list.size': ListSize})