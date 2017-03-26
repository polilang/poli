from . import *

class ArrayLength(Statement):
    """ ARRAY.LENGTH <id>, id[{exp-num , exp-num}]"""

    def __init__(self, target, array, start, length, line=0):
        self.line = line
        self.target = target
        self.array = array
        self.start = start
        self.length = length

    def execute(self):
        length = Interpreter.Objects.get(self.array).size(
            self.start.evaluate(),
            self.length.evaluate()
        )

        Interpreter.Objects.put(self.target, length)

    @staticmethod
    def parse():
        target = Parser.consume(Type['word']).value
        Parser.consume(',')

        array = Parser.consume(Type['word']).value

        start, length = segment()

        return __class__(target, array, start, length, Parser.line())

    def crawl(self, stream):
        log(self.__class__.__name__+': '+self.array+' => '+self.target, stream)
        if self.start:
            aind() ; log('Start', stream) 
            aind() ; self.start.crawl(stream) ; aded() ; aded()
        if self.length:
            aind() ; log('Length', stream)
            aind() ; self.length.crawl(stream) ; aded() ; aded()


keywords.append('array.length')
nodes.update({'array.length': ArrayLength})