from . import *

class ListToArray(Statement):
    """ LIST.TOARRAY id, <array> """
    def __init__(self, src, trg):
        self.line = Parser.line()
        self.src = src
        self.trg = trg

    def execute(self):
        from ..Array.Types import Array
        
        array = Array([0])
        array.extend(Interpreter.Objects.get(self.src))

        Interpreter.Objects.put(
            self.trg, array
            # create array, extend with list data, store
        )

    @staticmethod
    def parse():
        src = Parser.consume(Type['word']).value
        Parser.consume(',')
        trg = Parser.consume(Type['word']).value
        Parser.match('[', ']')

        return __class__(src, trg)

    def crawl(self, stream):
        log(self.__class__.__name__, stream)
        aind() ; log('Target: ' + self.trg, stream) ; aded()
        aind() ; log('Source: ' + self.src, stream) ; aded()

keywords.append('list.toarray')
nodes.update({'list.toarray': ListToArray})
