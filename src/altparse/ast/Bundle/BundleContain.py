from . import *

class BundleContain(Statement):
    """ BUNDLE.CONTAIN identifier, expression, <identifier> """

    def __init__(self, name, key, target, line=0):
        self.line = line

        self.name = name
        self.key = key
        self.target = target

    def execute(self):
        if type(Interpreter.Objects.get(self.name)) == dict:
            Interpreter.Objects.put(
                self.target,
                Interpreter.Objects.contains(self.name, self.key.evaluate())
            )
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

keywords.append('bundle.contain')
nodes.update({'bundle.contain': BundleContain})