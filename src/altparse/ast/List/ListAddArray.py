from . import *

class ListAddArray(Statement):
    """ LIST.ADD.ARRAY id, id{[{seg}]}"""

    def __init__(self, trg, src, start, length):
        self.line = Parser.line()
        self.trg = trg

        self.src = src
        self.start = start
        self.length = length

    def execute(self):
        Interpreter.Objects.put(
            self.trg,
            Interpreter.Objects.get(self.src).copy(
                self.start.evaluate(), self.length.evaluate()
            ).data
            #get array, copy a segment, extract data
        )

    @staticmethod
    def parse():
        trg = Parser.consume(Type['word']).value

        Parser.consume(',')
        src = Parser.consume(Type['word']).value

        from ..Array.Types import segment
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

keywords.append('list.add.array')
nodes.update({'list.add.array': ListAddArray})