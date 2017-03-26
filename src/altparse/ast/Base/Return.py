from . import *

class Return(Statement):
    """ RETURN """

    def execute(self):
        if Interpreter.State.name('func') == "gosub":
            Interpreter.index, node = Interpreter.State.pop('func')
        else:
            throw(BasicRuntimeError, "without GOSUB", self)

    @staticmethod
    def parse():
        return __class__(Parser.line())

keywords.append('return')
nodes.update({'return': Return})
