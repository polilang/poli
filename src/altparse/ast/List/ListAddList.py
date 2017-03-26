from . import *

class ListAddList(Statement):
    """ LIST.ADD.LIST identifier, identifier """

    def __init__(self, target, source, line=0):
        self.line = line

        self.target = target
        self.source = source

    def execute(self):
        Interpreter.Objects.put(
            self.target, Interpreter.Objects.get(self.source), extend=True
        )

    @staticmethod
    def parse():
        target = Parser.consume(Type['word']).value
        Parser.consume(',')
        source = Parser.consume(Type['word']).value

        return __class__(target, source, Parser.line())


    def crawl(self, stream):
        log(self.__class__.__name__, stream)
        aind() ; log('Target: ' + self.target, stream) ; aded()
        aind() ; log('Source: ' + self.source, stream) ; aded()

keywords.append('list.add.list')
nodes.update({'list.add.list': ListAddList})