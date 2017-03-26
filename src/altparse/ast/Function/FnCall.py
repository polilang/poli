from . import *

class FunctionCall(Value):
    """ identifier ( arguments ) """

    def __init__(self, name, args, line=0):
        self.line = line

        self.name = name
        self.args = args

    def evaluate(self):
        if self.name in Interpreter.Functions:
            takecount = len(Interpreter.Functions[self.name]['args'])
            givecount = len(self.args)

            if not takecount == givecount:
                throw(BasicRuntimeError,
                    self.name + ': takes ' + str(takecount) + ' arguments' \
                    + ', ' + str(givecount) + ' given', self
                )

            vals = []

            for i in self.args:
                vals.append(i.evaluate())

            if Interpreter.Functions[self.name]['builtin']:
                try:
                    return Interpreter.Functions[self.name]['syscall'](*vals)
                except:
                    throw(BasicRuntimeError, 
                        "Undefined error in '"+self.name+"'", self
                    )

            Interpreter.State.push('func','functioncall', [Interpreter.index, self])
            Interpreter.Objects.setspace(self.name)

            lastindex = Interpreter.index
            Interpreter.index = Interpreter.Functions[self.name]['start']

            for i in range(len(self.args)):
                Interpreter.Objects.put(
                    Interpreter.Functions[self.name]['args'][i], vals[i]
                )

            while Interpreter.index < len(Interpreter.ast)-1 and \
                not type(Interpreter.ast[Interpreter.index]) in (FnEnd, FnRtn)\
                and Interpreter.State.running:

                Interpreter.ast[Interpreter.index].execute()
                Interpreter.index += 1

            if Interpreter.index >= len(Interpreter.ast)-1:
                throw(BasicRuntimeError, self.name + ": Fn.End not found",self)

            if not Interpreter.State.running:
                return 1

            index = Interpreter.index
            Interpreter.index = lastindex

            return Interpreter.ast[index].execute()

        else:
            throw(BasicRuntimeError, "'" + self.name + "' not declared", self)

    def execute(self):
        self.evaluate()

    @staticmethod
    def parse():
        fname = Parser.get(-2).value
        args = list()
        if Parser.match(')'):
            return __class__(fname, args, Parser.line())
    
        args.append(Parser.expression())
        while Parser.match(','):
            args.append(Parser.expression())
        Parser.consume(')')
        return __class__(fname, args, Parser.line())


    def crawl(self, stream):
        log(self.__class__.__name__ + ': ' + self.name, stream)
        aind()
        for a in self.args:
            a.crawl(stream)
        aded()

class Call(Statement):
    """ CALL function """

    @staticmethod
    def parse():
        Parser.consume(Type['word'])
        Parser.consume('(')
        return FunctionCall.parse()

keywords.append('call')
nodes.update({
    'call': Call,
    (Type['word'], '('): FunctionCall,
})