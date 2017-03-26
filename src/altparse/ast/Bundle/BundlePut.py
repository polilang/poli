from . import *

class BundlePut(Statement):
    """ BUNDLE.PUT identifier, expression, expression """

    def __init__(self, name, key, value, line=0):
        self.line = line

        self.name = name
        self.key = key
        self.value = value

    def execute(self):
        Interpreter.Objects.put(
            self.name,
            self.value.evaluate(),
            key=self.key.evaluate()
        )

    @staticmethod
    def parse():
        name = Parser.consume(Type['word']).value
        Parser.consume(',')
        key = Parser.expression()
        Parser.consume(',')
        value = Parser.expression()

        return __class__(name, key, value, Parser.line())

    def crawl(self, stream):
        log(self.__class__.__name__ + ': ' + self.name, stream)
        aind()

        log('Key', stream)
        aind() ; self.key.crawl(stream) ; aded()

        log('Value', stream)
        aind() ; self.value.crawl(stream) ; aded()
        
        aded()

keywords.append('bundle.put')
nodes.update({'bundle.put': BundlePut})