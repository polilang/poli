from . import *

class EndIf(Statement):
    """ ENDIF """

    def execute(self):
        if not 'ifblock' == Interpreter.State.name('if'):
            throw(BasicRuntimeError, 'outside and IF block', self)

        Interpreter.State.pop('if')

    @staticmethod
    def parse():
        return __class__(Parser.line())

    def crawl(self, stream):
        aded() ; log(self.__class__.__name__, stream)

keywords.append('endif')
nodes.update({'endif': EndIf})
nodes.update({('end', 'if'): EndIf})