from . import *

class Input(Statement):
    """ INPUT identifier """

    def __init__(self, identifier, line=0):
        self.line = line
        self.identifier = identifier

    def execute(self):
        value = input()
        if self.identifier[-1] == '$':
            Interpreter.Objects.put(self.identifier, assert_str(value))
        else:
            Interpreter.Objects.put(self.identifier, assert_num(value))

    @staticmethod
    def parse():
        return __class__(Parser.consume(Type['word']).value, Parser.line())

    def crawl(self, stream):
        log(self.__class__.__name__ + ' ' + self.identifier, stream)

keywords.append('input')
nodes.update({'input': Input})