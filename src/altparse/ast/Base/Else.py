from . import *

class Else(Statement):
    """ ELSE 
            block
        ENDIF
    """

    def execute(self):
        from .ElseIf import ElseIf
        from .EndIf import EndIf

        if Interpreter.singleif: # dirty single-line if flag
            Interpreter.singleif = False
            Interpreter.index+=1
            return

        if not 'ifblock' == Interpreter.State.name('if'):
            throw(BasicRuntimeError, 'outside and IF block', self)

        if Interpreter.State.contains('if','trueif'):

            while not type(Interpreter.ast[Interpreter.index]) == EndIf:
                Interpreter.index += 1

                if type(Interpreter.ast[Interpreter.index]) == IfThenBlock:
                    while not type(Interpreter.ast[Interpreter.index]) == EndIf:
                        Interpreter.index += 1
                    Interpreter.index += 1

                if type(Interpreter.ast[Interpreter.index]) == ElseIf:
                    throw(BasicRuntimeError, 'ELSEIF after ELSE',
                        Interpreter.ast[Interpreter.index])
            Interpreter.index -= 1

        else:
            idx = Interpreter.index
            while not type(Interpreter.ast[idx]) == EndIf:
                idx += 1

                if type(Interpreter.ast[idx]) == ElseIf:
                    throw(BasicRuntimeError, 'ELSEIF after ELSE',
                        Interpreter.ast[idx])

    @staticmethod
    def parse():
        from .ElseIf import ElseIf
        if not Parser.match('if'):
            return __class__(Parser.line())
        return ElseIf.parse()

    def crawl(self, stream):
        aded() ; log(self.__class__.__name__, stream) ; aind()

keywords.append('else')
nodes.update({'else': Else})
