from . import *

class BundleCreate(Statement):
    """ BUNDLE.CREATE <identifier> """

    def __init__(self, name, line=0):
        self.line = line
        self.name = name

    def execute(self):
        from .Types import Bundle
        Bundle(self.name).evaluate()

    @staticmethod
    def parse():
        if not Parser.match(Type['word']):
            throw(BasicParseError,
                "BundleCreate: No name specified", Parser.get(0)
            )

        name = Parser.get(-1).value
        return __class__(name, Parser.line())

    def crawl(self, stream):
        log(self.__class__.__name__ + ': ' + self.name, stream)

keywords.append('bundle.create')
nodes.update({'bundle.create': BundleCreate})