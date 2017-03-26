from . import *

class ArrayDims(Statement):
    """ ARRAY.DIMS array{, <array>}{, id}"""

    def __init__(self, array, target, length):
        self.line = Parser.line()
        self.array = array
        self.target = target
        self.length = length

    def execute(self):
        array = Interpreter.Objects.get(self.array)
        if self.target:
            dims = array.dim
            trg = Array([0])
            trg.extend(dims)
            Interpreter.Objects.put(self.target, trg)
        if self.length:
            length = len(array.dim)
            Interpreter.Objects.put(self.length, length)

    @staticmethod
    def parse():
        array = Parser.consume(Type['word']).value
        Parsers.match('[',']')

        length = None
        target = None

        if Parser.match(','):
            if self.match(Type['word']):
                target = Parser.get(-1).value
                Parsers.match('[',']')
        if Parser.match(','):
            if self.match(Type['word']):
                length = Parser.get(-1).value

        return __class__(array, target, length)

    def crawl(self, stream):
        log(self.__class__.__name__+': '+self.array, stream)
        if self.target:
            aind() ; log('Dims: '+self.target, stream) ; aded()
        if self.length:
            aind() ; log('Length: '+self.length, stream) ; aded()

keywords.append('array.dims')
nodes.update({'array.dims': ArrayDims})