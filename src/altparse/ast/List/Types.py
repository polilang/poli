from . import *

class List(Value):
    """ identifier [] """
    def __init__(self, name, line=0):
        self.line = line
        self.name = name

    def evaluate(self):
        if not Interpreter.Objects.contains(self.name):
            Interpreter.Objects.put(self.name, list())

        return Interpreter.Objects.get(self.name)

    def crawl(self, stream):
        log(self.__class__.__name__ + ': ' + self.name, stream)

class ListVariable(Value):
    """ identifier [ expression ] """

    def __init__(self, name, index, line=0):
        self.line = line

        self.name = name
        self.index = index

    def evaluate(self):
        return Interpreter.Objects.get(self.name, index=self.index.evaluate())

    @staticmethod # unused due to arrays
    def parse():
        name = Parser.get(-2).value
        if Parser.match(']'):
            return List(name, Parser.line())
        else:
            exp = Parser.expression()
            Parser.consume(']')
            return __class__(name, exp, Parser.line())


    def crawl(self, stream):
        log(self.__class__.__name__+ ': ' + self.name, stream)
        aind()

        log('Index', stream)
        aind() ; self.index.crawl(stream) ; aded()

        aded()
