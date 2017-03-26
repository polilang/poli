from . import *

class Bundle(Value):
    """ identifier {} """
    def __init__(self, name, line=0):
        self.line = line
        self.name = name

    def evaluate(self):
        if not Interpreter.Objects.contains(self.name):
            Interpreter.Objects.put(self.name, dict())

        return Interpreter.Objects.get(self.name)

    def crawl(self, stream):
        log(self.__class__.__name__ + ': ' + self.name, stream)

class BundleVariable(Value):
    """ identifier [ expression ] """

    def __init__(self, name, key, line=0):
        self.line = line

        self.name = name
        self.key = key

    def evaluate(self):
        return Interpreter.Objects.get(self.name, key=self.key.evaluate())

    @staticmethod
    def parse():
        name = Parser.get(-2).value
        if Parser.match('}'):
            return Bundle(name, Parser.line())
        else:
            exp = Parser.expression()
            Parser.consume('}')
            return __class__(name, exp, Parser.line())


    def crawl(self, stream):
        log(self.__class__.__name__+ ': ' + self.name, stream)
        aind()

        log('Key', stream)
        aind() ; self.key.crawl(stream) ; aded()
        
        aded()
