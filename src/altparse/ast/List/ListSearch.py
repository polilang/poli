from . import *

class ListSearch(Statement):
    """ LIST.SEARCH identifier, exp-arith, <identifier> ', exp-arith' """

    def __init__(self, name, search, index, offset=Number(0), line=0):
        self.line = line

        self.name = name
        self.search = search
        self.index = index
        self.offset = offset

    def execute(self):
        if type(Interpreter.Objects.get(self.name)) == list:
            Interpreter.Objects.put(
                self.index,
                Interpreter.Objects.search(
                    self.name, self.search.evaluate(), self.offset.evaluate()
                )
            )
        else:
            throw(BasicRuntimeError, 
                "object '" + self.name + "' is not a list", self)

    @staticmethod
    def parse():
        offset=Number(0)
        name = Parser.consume(Type['word']).value

        Parser.consume(',')
        search = Parser.expression()

        Parser.consume(',')
        index = Parser.consume(Type['word']).value

        if Parser.match(','):
            offset = Parser.expression()

        return __class__(name, search, index, offset, Parser.line())

    def crawl(self, stream):
        log(self.__class__.__name__ + ': ' + self.name, stream)
        aind()

        log('Search', stream)
        aind() ; self.search.crawl(stream) ; aded()

        log('Return: ' + self.index, stream)

        if self.offset:
            log('Offset', stream)
            aind() ; self.offset.crawl(stream) ; aded()

keywords.append('list.search')
nodes.update({'list.search': ListSearch})