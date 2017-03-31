from . import *

class ArrayMin(Statement):
    """ ARRAY.MIN <id>, id[{seg}] """

    def __init__(self, trg, src, start, length):
        self.line = Parser.line()
        self.trg = trg
        self.src = src
        self.start = start
        self.length = length

    def execute(self):
        array = Interpreter.Objects.get(self.src)
        start = self.start.evaluate()
        length = self.length.evaluate()
        
        Interpreter.Objects.put(self.trg, min(array.segment(start, length)))

    @staticmethod
    def parse():
        trg = Parser.consume(Type['word']).value
        Parser.consume(',')
        src = Parser.consume(Type['word']).value
        start, length = segment()

        return __class__(trg, src, start, length)

    def crawl(self, stream):
        log(self.__class__.__name__+': '+self.trg+' <= '+self.src, stream)
        if self.start:
            aind() ; log('Start', stream) 
            aind() ; self.start.crawl(stream) ; aded() ; aded()
        if self.length:
            aind() ; log('Length', stream)
            aind() ; self.length.crawl(stream) ; aded() ; aded()

keywords.append('array.min')
nodes.update({'array.min': ArrayMin})