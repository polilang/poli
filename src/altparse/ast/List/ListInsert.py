from . import *

class ListInsert(Statement):
    """ LIST.INSERT identifier, expression, expression """

    def __init__(self, name, index, value, line=0):
        self.line = line

        self.name = name
        self.index = index
        self.value = value

    def execute(self):
        Interpreter.Objects.put(
            self.name,
            self.value.evaluate(),
            index=self.index.evaluate(),
            insert=True
        )

    @staticmethod
    def parse():
        name = Parser.consume(Type['word']).value
        Parser.consume(',')
        index = Parser.expression()
        Parser.consume(',')
        value = Parser.expression()

        return __class__(name, index, value, Parser.line())

    def crawl(self, stream):
        log(self.__class__.__name__ + ': ' + self.name, stream)
        aind()

        log('Index', stream)
        aind() ; self.index.crawl(stream) ; aded()

        log('Value', stream)
        aind() ; self.value.crawl(stream) ; aded()
        
        aded()

keywords.append('list.insert')
nodes.update({'list.insert': ListInsert})