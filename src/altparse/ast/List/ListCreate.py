from . import *

class ListCreate(Statement):
    """ LIST.CREATE N|S, <identifier> """

    def __init__(self, name, type=None, line=0):
        self.line = line
        self.type = type
        self.name = name

    def execute(self):
        List(self.name, self.type).evaluate()

    @staticmethod
    def parse():
        if not (Parser.match('n') or Parser.match('s')):
            throw(BasicParseError,
                "ListCreate: No type specified", Parser.get(0)
            )

        type = Parser.get(-1).value
        Parser.consume(',')

        if not Parser.match(Type['word']):
            throw(BasicParseError,
                "ListCreate: No name specified", Parser.get(0)
            )

        name = Parser.get(-1).value
        return __class__(name, type, Parser.line())

    def crawl(self, stream):
        log(self.__class__.__name__ + ': ' + self.name, stream)

keywords.append('list.create')
nodes.update({'list.create': ListCreate})