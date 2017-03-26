from . import *

class ArrayVariance(Statement):
    """ ARRAY.VARIANCE <id>, id[{seg}] """

    def __init__(self, var, name, start, length):
        self.line = Parser.line()
        self.var = var
        self.name = name
        self.start = start
        self.length = length

    def execute(self):
        from statistics import variance
        array = Interpreter.Objects.get(self.name)
        var = variance(
            array.segment(self.start.evaluate(), self.length.evaluate())
        )
        Interpreter.Objects.put(self.var, var)

    @staticmethod
    def parse():
        var = Parser.consume(Type['word']).value
        Parser.consume(',')
        name = Parser.consume(Type['word']).value
        start, length = segment()

        return __class__(var, name, start, length)

    def crawl(self, stream):
        log(self.__class__.__name__+ ': ' + self.name + '=>' + self.var, stream)
        if self.start:
            aind() ; log('Start', stream) 
            aind() ; self.start.crawl(stream) ; aded() ; aded()
        if self.length:
            aind() ; log('Length', stream)
            aind() ; self.length.crawl(stream) ; aded() ; aded()

keywords.append('array.variance')
nodes.update({'array.variance': ArrayVariance})