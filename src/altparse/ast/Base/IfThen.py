from . import *

class IfThen(Statement):
    @staticmethod
    def parse():
        condition = Parser.expression()
        Parser.match('then')
        if Parser.match(Type['EOL']):
            return IfThenBlock(condition, Parser.line())

        return IfThenSingle(condition, Parser.line())


class IfThenSingle(Statement):
    """ IF expression THEN statement """

    def __init__(self, condition, line=0):
        self.line = line
        self.condition = condition

    def execute(self):
        from .Else import Else

        if not self.condition.evaluate():
            Interpreter.index += 1
            if len(Interpreter.ast) > Interpreter.index+1:
                if type(Interpreter.ast[Interpreter.index+1]) == Else:
                    Interpreter.index += 1

        Interpreter.singleif=True # dirty single-line if flag

    def crawl(self, stream):
        log(self.__class__.__name__, stream)
        aind() ; self.condition.crawl(stream) ; aded()

class IfThenBlock(Statement):
    """ IF expression {THEN} 
            block
      { ELSE
            block }
      { ELSEIF|ELSE IF expression {THEN}
            block }
        ENDIF
    """

    def __init__(self, condition, line=0):
        self.line = line
        self.condition = condition

    def execute(self):
        from .Else import Else
        from .ElseIf import ElseIf
        from .EndIf import EndIf
        
        Interpreter.State.push('if', 'ifblock', 0)
 
        if not self.condition.evaluate():

            while not type(Interpreter.ast[Interpreter.index]) \
            in (Else, ElseIf, EndIf):

                Interpreter.index += 1
                if type(Interpreter.ast[Interpreter.index]) == IfThenSingle:
                    Interpreter.index += 1
                    if type(Interpreter.ast[Interpreter.index+1]) == Else:
                        Interpreter.index += 1

                elif type(Interpreter.ast[Interpreter.index]) == IfThenBlock:
                    while not type(Interpreter.ast[Interpreter.index]) == EndIf:
                        Interpreter.index += 1
                    Interpreter.index += 1
                    
            Interpreter.index -= 1

        else:
            Interpreter.State.extend('if', 'trueif')

    def crawl(self, stream):
        log(self.__class__.__name__, stream)
        aind() ; self.condition.crawl(stream)

keywords.append('if')
keywords.append('then')
nodes.update({'if': IfThen})