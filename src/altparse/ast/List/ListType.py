from . import *

class ListType(Statement):
    """ LIST.TYPE id, <id> """

    def __init__(self, name, target):
        self.line = Parser.line()
        self.name = name
        self.target = target

    def execute(self):
        ls = Interpreter.Objects.get(self.name)
        if type(ls) == list:
            cs = [] ; cn = []
            for i in ls:
                if type(i) == str: cs.append(i)
                else: cn.append(i)
    
            if len(cn) > len(cs):
                Interpreter.Objects.put(self.target, 'N')
            else:
                Interpreter.Objects.put(self.target, 'S') 
        else:
            throw(BasicRuntimeError, 
                "object '" + self.name + "' is not a list", self)

    @staticmethod
    def parse():
        name = Parser.consume(Type['word']).value
        Parser.consume(',')
        target = Parser.consume(Type['word']).value
        return __class__(name, target)


    def crawl(self, stream):
        log(self.__class__.__name__ + ': ' + self.name, stream)
        log('Variable: ' + self.target, stream) ; aded()

keywords.append('list.type')
nodes.update({'list.type': ListType})