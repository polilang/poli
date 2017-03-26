from . import *

class Label(Value):
    def __init__(self, value, index, line=0):
        self.line = line
        self.value = value

        if value not in Interpreter.Labels:
            Interpreter.Labels[value] = index
        else:
            throw(BasicRuntimeError, 'duplicate at line: ' \
                + str(Parser.ast[Interpreter.Labels[value]].line), self
            )

    def execute(self):
        pass

    @staticmethod
    def parse():
        return __class__(Parser.get(-1).value, len(Parser.ast), Parser.line())

nodes.update({Type['label']: Label})