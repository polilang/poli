from . import *

class ElseIf(Statement):
    """ ELSEIF|ELSE IF expression {THEN}
            block
        ENDIF
    """

    def __init__(self, condition, line=0):
        self.line = line
        self.condition = condition

    def execute(self):
        from .Else import Else
        from .EndIf import EndIf

        if not 'ifblock' == Interpreter.State.name('if'):
            throw(BasicRuntimeError, 'outside and IF block', self)

        if Interpreter.State.contains('if','trueif'):
            while not type(Interpreter.ast[Interpreter.index]) == EndIf:
                Interpreter.index += 1
            Interpreter.index-=1
            return

        Interpreter.index += 1

        if not self.condition.evaluate():
            while not type(Interpreter.ast[Interpreter.index]) \
            in (Else, ElseIf, EndIf):
                Interpreter.index += 1

            if type(Interpreter.ast[Interpreter.index]) in (ElseIf, Else):
                Interpreter.index -= 1
        else:
            Interpreter.State.extend('if', 'trueif', index=0)
            Interpreter.index -= 1

    @staticmethod
    def parse():
        condition = Parser.expression()
        Parser.match('then')
        if not Parser.match(Type['EOL']):
            throw(BasicParseError, 'ElseIf: single line ELSEIF', Parser.get(0))

        return __class__(condition, Parser.line())

    def crawl(self, stream):
        aded() ; log(self.__class__.__name__, stream) ; aind()
        self.condition.crawl(stream)

keywords.append('elseif')
nodes.update({'elseif': ElseIf})
nodes.update({('else', 'if'): ElseIf})