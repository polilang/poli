from . import *

class FnDef(Statement):
    """ FN.DEF identifier ( arguments )
          { block }
          { FN.RTN expression }
        FN.END
    """

    def __init__(self, name, args, line=0):
        self.line = line

        self.name = name
        self.args = args

    def execute(self):
        from .FnEnd import FnEnd
        
        if Interpreter.State.name('loop') in ('forloop', 'doloop', 'whileloop'):
            throw(BasicRuntimeError,
                'cannot declare a function in a loop', self)
        elif Interpreter.State.name('func') == 'functioncall':
            throw(BasicRuntimeError,
                'cannot nest function definitions', self)

        index = Interpreter.index

        while not type(Interpreter.ast[index]) == FnEnd:
            index += 1

        FunctionDeclare(self.name, self.args, Interpreter.index+1)
        Interpreter.index = index

    @staticmethod
    def parse():
        from .FnEnd import FnEnd

        tmpstate = []
        for i in Parser.ast:
            if type(i) == FnDef: tmpstate.append('fndef')
            if type(i) == FnEnd: tmpstate.pop()
        if 'fndef' in tmpstate:
            throw(BasicParseError,
                'FnDef: cannot nest function definitions', Parser.get(-1)
            )

        fname = Parser.consume(Type['word']).value
        Parser.consume('(')
        args = list()
        while not Parser.match(')'):
            args.append(Parser.consume(Type['word']).value)
            Parser.match('[', ']')
            Parser.match(',')

        return __class__(fname, args, Parser.line())


    def crawl(self, stream):
        log(self.__class__.__name__+ ' ' + self.name + str(self.args), stream)
        aind()

class FunctionDeclare:
    def __init__(self, name, args, start, builtin=False, syscall=None):
        self.name = name
        self.args = args
        self.start = start
        if name not in Interpreter.Functions:
            Interpreter.Functions[name] = {
                'args': args,
                'start': start,
                'builtin': builtin,
                'syscall': syscall
            }

    def crawl(self, stream):
        log(self.__class__.__name__, stream)
        aind() ; log(self.name + ': ' + str(self.args), stream) ; aded()

keywords.append('fn.def')
nodes.update({'fn.def': FnDef})