from . import *

class BundleType(Statement):
    """ BUNDLE.TYPE identifier, expression, <identifier> """

    def __init__(self, name, key, target, line=0):
        self.line = line

        self.name = name
        self.key = key
        self.target = target

    def execute(self):
        if type(Interpreter.Objects.get(self.name)) == dict:
            obj = Interpreter.Objects.get(self.name, key=self.key.evaluate())
            if type(obj) in (int, float):
                Interpreter.Objects.put(self.target, 'N')
            elif type(obj) == str:
                Interpreter.Objects.put(self.target, 'S')
            elif type(obj) == dict:
                Interpreter.Objects.put(self.target, 'B')
            elif type(obj) == list:
                Interpreter.Objects.put(self.target, 'L')
            elif type(obj) == Array:
                Interpreter.Objects.put(self.target, 'A')
            else:
                throw(BasicRuntimeError, 
                    "object '" + self.name + "' has undefined type", self)

        else:
            throw(BasicRuntimeError, 
                "object '" + self.name + "' is not a bundle", self)

    @staticmethod
    def parse():
        name = Parser.consume(Type['word']).value
        Parser.consume(',')
        key = Parser.expression()
        Parser.consume(',')
        target = Parser.consume(Type['word']).value
        return __class__(name, key, target, Parser.line())


    def crawl(self, stream):
        log(self.__class__.__name__ + ': ' + self.name, stream)
        aind() ; log('Key', stream)
        aind() ; self.key.crawl(stream) ; aded()
        log('Variable: ' + self.target, stream) ; aded()

keywords.append('bundle.type')
nodes.update({'bundle.type': BundleType})