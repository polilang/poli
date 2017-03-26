"""
    --- String to parse

    foo = a, b, c:
        print a, b + c



    --- Tokens

    ident assign ident comma ident comma ident colon
    {   ident ident comma ident binop ident}



    --- Expected AST

    Assign
        identifier = foo
        expression = FunctionDefine
            parameters = a, b, c
            body = {
                FunctionCall
                    arguments = {
                        expression = identifier = a
                        expression = sum
                            left = identifier = b
                            right = identifier = c
                    }
"""


from .tokenizer import Token, Type, dkey
from .error import throw, BasicParseError


class Parser:
    tokens = list()
    position = 0
    ast = list()

    def __init__(self, tokens, position = 0):
        Parser.tokens = tokens
        Parser.position = position

    def parse(self):
        """ parses tokens matching against spool """
        from .ast import spool
        spool = spool()

        while not Parser.get(1).type == Type['EOF']:
            if not Parser.match(Type['EOL']):
                for sig in spool:
                    if not type(sig) == tuple:
                        if Parser.match(sig):
                            Parser.ast.append(spool[sig].parse())
                            break
                    else:
                        if Parser.match(sig[0], sig[1]):
                            Parser.ast.append(spool[sig].parse())
                            break
                else:
                    throw(BasicParseError,
                        'Parse: unexpected token', Parser.get(0)
                    )

        return Parser.ast

    @staticmethod
    def expression(string=False):
        """ gets an expression

        atom {operator expression}
        """
        from .ast.Base import Operator
        from .ast.Base import String

        bpow = { # precedence/binding power
            0: [';',','],
            1: ['|', '||', '&&', '&'],
            2: ['<','>','<=','>=','=','!=','<>'],
            3: ['+','-'],
            4: ['*','/'],
            5: ['!']
        }

        explist = []

        if Parser.match('!'):
            explist.append('!')

        explist.append(Parser.atom())
        expectop = True #dirty flag, operator expected next

        if Parser.get(0).type in (Type['word'], Type['number'], Type['string'])\
        and expectop:
            throw(BasicParseError, "Expression: unexpected token", Parser.get(0))

        if string:
            while Parser.match(Type['operator']):
                if Parser.get(-1).value == '+':
                    explist.append(';')
                else:
                    explist.append(Parser.get(-1).value)
                expectop = False

                if Parser.get(-1).value == ',' and Parser.match(Type['EOL']):
                    # print exp ,
                    explist.append(String())
                    break

                explist.append(Parser.atom())
                expectop = True
        else:
            while not Parser.get(0).value in (',', ';') \
            and Parser.match(Type['operator']):
                explist.append(Parser.get(-1).value)
                expectop = False
                explist.append(Parser.atom())
                expectop = True

        if Parser.get(0).type in (Type['word'], Type['number'], Type['string']) and expectop:
            throw(BasicParseError, "Expression: unexpected token", Parser.get(0))

        for i in (5, 4, 3, 2, 1, 0):
            index = -1
            while any((op in explist for op in bpow[i])):
                index += 1
                if explist[index] in bpow[i]:
                    if explist[index] == '!': # NOT operator - special case
                        explist = explist[:index] \
                            + [Operator(
                                explist[index+1], explist[index],
                                line=Parser.line()
                              )] \
                            + explist[index+2:]
                        index = -1
                    else:
                        explist = explist[:index-1] \
                            + [Operator(
                                explist[index-1], explist[index],
                                explist[index+1], Parser.line()
                              )] \
                            + explist[index+2:]
                        index = -1

        return explist[0]

    @staticmethod
    def atom():
        """ atom collector

        identifier[ ' expression ' ]
        identifier{ ' expression ' }
        identifier
        number
        string
        ( expression )
        """
        from .ast.Types import Variable, String, Number
        from .ast.Function.FnCall import FunctionCall
        from .ast.Array.Types import ArrayVariable
        from .ast.Bundle.Types import BundleVariable
        from .ast.Base import Operator

        if Parser.match('-'):
            return Operator(Number(), '-', Parser.atom())

        if Parser.match(Type['word'], '['):
            return ArrayVariable.parse()

        if Parser.match(Type['word'], '{'):
            return BundleVariable.parse()

        elif Parser.match(Type['word'], '('):
            return FunctionCall.parse()

        elif Parser.match(Type['word']):
            return Variable(Parser.get(-1).value, line=Parser.line())

        elif Parser.match(Type['number']):
            return Number(Parser.get(-1).value)

        elif Parser.match(Type['string']):
            return String(Parser.get(-1).value)

        elif Parser.match('('):
            exp = Parser.expression()
            Parser.consume(')')
            return exp

        throw(BasicParseError, 'Atom: unexpected token', Parser.get(0))

    @staticmethod
    def match(type1, type2=None):
        """ match a token or word against a token """
        if Parser.get(0).type == Type['continue']:
            Parser.position += 1
            if type1 in (',', Type['operator']):
                if not type2:
                    return True
                return Parser.match(type2)
            else:
                return Parser.match(type1, type2)

        if type(type1) == int:
            if not Parser.get(0).type == type1: return False
        else:
            if not Parser.get(0).value == type1: return False

        if type2:
            if type(type2) == int:
                if not Parser.get(1).type == type2: return False
            else:
                if not Parser.get(1).value == type2: return False

            Parser.position += 1

        Parser.position += 1
        return True

    @staticmethod
    def consume(thing):
        """ consume a matching token or word """

        if type(thing) == int: token = dkey(thing)
        else: token = Token(thing, Type['word'])

        if not Parser.match(thing):
            throw(BasicParseError,
                'Consume: missing expected token', Parser.get(0), token)

        return Parser.get(-1)

    @staticmethod
    def line():
        return Parser.get(-1).line

    @staticmethod
    def get(offset):
        """ get a token """

        if Parser.position+offset >= len(Parser.tokens):
            return Token("",Type['EOF'])

        return Parser.tokens[Parser.position+offset]


