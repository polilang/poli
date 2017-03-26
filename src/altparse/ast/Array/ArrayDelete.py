from . import *

class ArrayDelete(Statement):
    """ ARRAY.DELETE identifier {[]} """

    def __init__(self, name, line=0):
        self.line = line
        self.name = name

    def execute(self):
        Interpreter.Objects.remove(self.name)

    @staticmethod
    def parse():
        name = Parser.consume(Type['word']).value
        Parser.match('[',']')

        if Parser.get(0).value == ',':
            Parser.tokens[Parser.position] = Token(
                'undim', Type['keyword'], 
                Parser.get(0).line,
                Parser.get(0).position
            )

        return __class__(name, Parser.line())

    def crawl(self, stream):
        log(self.__class__.__name__+ ': ' + self.name, stream)

keywords.append('array.delete')
keywords.append('undim')
nodes.update({
    'array.delete': ArrayDelete,
    'undim': ArrayDelete
})