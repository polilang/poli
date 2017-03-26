from . import *

class Gosub(Statement):
    """ GOSUB label """

    def __init__(self, label, line=0):
        self.line = line
        self.label = label

    def execute(self):
        if self.label in Interpreter.Labels:
            Interpreter.State.push('func', "gosub", [Interpreter.index, self])
            Interpreter.index = Interpreter.Labels[self.label]
        else:
            throw(BasicRuntimeError,
                "label '" + self.label + "' not declared", self)

    @staticmethod
    def parse():
        if Parser.match(Type['word']) or Parser.match(Type['label']):
            label = Parser.get(-1).value
        else:
            throw(BasicParseError, 
                "unexpected token", Parser.get(0)
            )

        return __class__(label, Parser.line())

    def crawl(self, stream):
        log(self.__class__.__name__ + " '" + self.label + "'", stream)

keywords.append('gosub')
nodes.update({'gosub': Gosub})
