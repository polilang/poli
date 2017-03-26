from . import *

class Assignment(Statement):
    """ identifier op-assign expression
        identifier [ expression ] op-assign expression
        identifier { expression } op-assign expression
    """

    def __init__(self, left, right, line=0, dim=None, key=None):
        self.line = line

        self.left = left
        self.right = right
        self.dim = dim
        self.key = key

    def execute(self):
        if self.left[-1] == '$':
            right = assert_str(self.right.evaluate())
            Interpreter.Objects.put(
                self.left, right,
                dim=self.dim and self.dim.evaluate(),
                key=self.key and self.key.evaluate()
            )
        else:
            right = assert_num(self.right.evaluate())
            Interpreter.Objects.put(
                self.left, right,
                dim=self.dim and self.dim.evaluate(),
                key=self.key and self.key.evaluate()
            )
        
    @staticmethod
    def parse():
        left = Parser.get(-2).value
        dim = None
        key = None

        if Parser.get(-1).value == '[': # array assignment
            if Parser.match(']'):
                throw(BasicParseError, 'array index not specified:', name)
            dim = ArrayDim.parse()
            operator = Parser.consume('=').value

        if Parser.get(-1).value == '{': # bundle assignment
            if Parser.match('}'):
                throw(BasicParseError, 'bundle key not specified:', name)
            key = Parser.expression()
            Parser.consume('}')
            operator = Parser.consume(Type['assign']).value

        operator = Parser.get(-1).value
        right = Parser.expression()

        if operator == '=':
            return __class__(left, right, Parser.line(), dim)

        elif operator in ('+=', '-=', '/=', '*='):
            from . import Operator

            if dim:
                cleft = ArrayVariable(left, dim, Parser.line())
            elif key:
                cleft = BundleVariable(left, key, Parser.line())
            else:
                cleft = Variable(left, line=Parser.line())

            return __class__(
                left,
                Operator(cleft, operator[0], right),
                Parser.line(),
                dim=dim, key=key
            )
        else:
            throw(BasicParseError, 'undefined assignment operator:', operator)


    def crawl(self, stream):
        log(self.__class__.__name__, stream)
        aind() ; log('Variable '+ self.left, stream)
        if self.dim:
            log('Index', stream)
            aind() ; self.dim.crawl(stream) ; aded()
        self.right.crawl(stream) ; aded()

nodes.update({
    (Type['word'], Type['assign']): Assignment,
    (Type['word'], '='): Assignment,
    (Type['word'], '['): Assignment,
    (Type['word'], '{'): Assignment,
    'Let': Assignment,
})