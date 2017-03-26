from . import *

class ArrayFill(Statement):
    """ ARRAY.FILL id [ {exp-num , exp-num} ], exp """

    def __init__(self, name, fill, start=0, length=0, line=0):
        self.line = line

        self.name = name
        self.fill = fill
        self.start = start
        self.length = length

    def execute(self):
        Interpreter.Objects.get(self.name).fill(
            self.fill.evaluate(),
            self.start.evaluate(),
            self.length.evaluate()
        )

    @staticmethod
    def parse():
        name = Parser.consume(Type['word']).value
        start, length = segment()
        Parser.consume(',')
        fill = Parser.expression()
        return __class__(name, fill, start, length, Parser.line())

    def crawl(self, stream):
        log(self.__class__.__name__ + ': ' + self.name, stream)
        if self.start:
            aind() ; log('Start', stream) 
            aind() ; self.start.crawl(stream) ; aded() ; aded()
        if self.length:
            aind() ; log('Length', stream)
            aind() ; self.length.crawl(stream) ; aded() ; aded()

keywords.append('array.fill')
nodes.update({'array.fill': ArrayFill})