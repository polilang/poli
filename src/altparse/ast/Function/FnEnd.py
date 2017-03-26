from . import *

class FnEnd(Statement):
    """ FN.END """

    def execute(self):
        if Interpreter.State.name('func') == 'functioncall':
            index, call = Interpreter.State.pop('func')
            Interpreter.Objects.popspace()
            Interpreter.index = index
        else:
            throw(BasicRuntimeError, 'outside function call', self)

        return 1

    @staticmethod
    def parse():
        return __class__(Parser.line())

    def crawl(self, stream):
        aded() ; log(self.__class__.__name__, stream)

keywords.append('fn.end')
nodes.update({'fn.end': FnEnd})