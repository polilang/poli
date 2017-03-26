from . import *

class ArraySearch(Statement):
    """ ARRAY.SEARCH id[{seg}], exp, <id>, exp-arith """

    def __init__(self, name, start, length, search, target, offset):
        self.line = Parser.line()
        
        self.name = name
        self.start = start
        self.length = length

        self.search = search
        self.target = target
        self.offset = offset

    def execute(self):
        array = Interpreter.Objects.get(self.name)
        index = array.segment(
            self.start.evaluate(), self.length.evaluate()
        )[self.offset.evaluate():].index(self.search.evaluate())
        Interpreter.Objects.put(self.target, index)

    @staticmethod
    def parse():
        name = Parser.consume(Type['word']).value
        start, length = segment()
        Parser.consume(',')

        search = Parser.expression()
        Parser.consume(',')

        target = Parser.consume(Type['word']).value

        if Parser.match(','):
            offset = Parser.expression()
        else:
            offset = Number()

        return __class__(name, start, length, search, target, offset)

    def crawl(self, stream):
        log(self.__class__.__name__+ ': ' + self.name + '=>' + self.target, stream)
        aind() ; log('Search', stream)
        aind() ; self.search.crawl(stream) ; aded() ; aded()
        if self.start:
            aind() ; log('Start', stream) 
            aind() ; self.start.crawl(stream) ; aded() ; aded()
        if self.length:
            aind() ; log('Length', stream)
            aind() ; self.length.crawl(stream) ; aded() ; aded()
        if self.offset:
            aind() ; log('Offset', stream)
            aind() ; self.offset.crawl(stream) ; aded() ; aded()

keywords.append('array.search')
nodes.update({'array.search': ArraySearch})