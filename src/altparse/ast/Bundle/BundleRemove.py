from . import *

class BundleRemove(Statement):
    """ BUNDLE.REMOVE identifier, expression """

    def __init__(self, name, key, line=0):
        self.line = line

        self.name = name
        self.key = key

    def execute(self):
        Interpreter.Objects.remove(self.name, key=self.key.evaluate())

    @staticmethod
    def parse():
        name = Parser.consume(Type['word']).value
        Parser.consume(',')
        key = Parser.expression()

        return __class__(name, key, Parser.line())


    def crawl(self, stream):
        log(self.__class__.__name__ + ': ' + self.name, stream)
        aind()
        
        log('Key', stream)
        aind() ; self.key.crawl(stream) ; aded()
        
        aded()

keywords.append('bundle.remove')
nodes.update({'bundle.remove': BundleRemove})