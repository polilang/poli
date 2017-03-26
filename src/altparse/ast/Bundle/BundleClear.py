from . import *

class BundleClear(Statement):
    """ BUNDLE.CLEAR identifier """

    def __init__(self, name, line=0):
        self.line = line
        self.name = name

    def execute(self):
        Interpreter.Objects.remove(self.name)

    @staticmethod
    def parse():
        name = Parser.consume(Type['word']).value
        return __class__(name, Parser.line())


    def crawl(self, stream):
        log(self.__class__.__name__+ ': ' + self.name, stream)

keywords.append('bundle.clear')
nodes.update({'bundle.clear': BundleClear})