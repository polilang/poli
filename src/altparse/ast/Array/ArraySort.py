from . import *

class ArraySort(Statement):
    """ ARRAY.SORT id[{seg}] """

    def __init__(self, name, start, length):
        self.line = Parser.line()
        self.name = name
        self.start = start
        self.length = length

    def execute(self):
        array = Interpreter.Objects.get(self.name)
        sort = sorted(
            array.segment(self.start.evaluate(), self.length.evaluate())
        )
        array.overwrite(sort, self.start.evaluate())

    @staticmethod
    def parse():
        name = Parser.consume(Type['word']).value
        start, length = segment()

        return __class__(name, start, length)

    def crawl(self, stream):
        log(self.__class__.__name__+ ': ' + self.name, stream)
        if self.start:
            aind() ; log('Start', stream) 
            aind() ; self.start.crawl(stream) ; aded() ; aded()
        if self.length:
            aind() ; log('Length', stream)
            aind() ; self.length.crawl(stream) ; aded() ; aded()

keywords.append('array.sort')
nodes.update({'array.sort': ArraySort})