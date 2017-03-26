from . import *

class ArrayStdDev(Statement):
    """ ARRAY.STD_DEV <id>, id[{seg}] """

    def __init__(self, dev, name, start, length):
        self.line = Parser.line()
        self.dev = dev
        self.name = name
        self.start = start
        self.length = length

    def execute(self):
        from statistics import stdev
        array = Interpreter.Objects.get(self.name)
        deviation = stdev(
            array.segment(self.start.evaluate(), self.length.evaluate())
        )
        Interpreter.Objects.put(self.dev, deviation)

    @staticmethod
    def parse():
        dev = Parser.consume(Type['word']).value
        Parser.consume(',')
        name = Parser.consume(Type['word']).value
        start, length = segment()

        return __class__(dev, name, start, length)

    def crawl(self, stream):
        log(self.__class__.__name__+ ': ' + self.name + '=>' + self.dev, stream)
        if self.start:
            aind() ; log('Start', stream) 
            aind() ; self.start.crawl(stream) ; aded() ; aded()
        if self.length:
            aind() ; log('Length', stream)
            aind() ; self.length.crawl(stream) ; aded() ; aded()

keywords.append('array.std_dev')
nodes.update({'array.std_dev': ArrayStdDev})