from . import *

def Operator(left, op, right=None, line=0):
    if op == '-':
        return Subtract(left, right, line)
    elif op == '+':
        if String in (type(left), type(right)):
            return Concat(left, right, line)
        return Add(left, right, line)
    elif op == ';':
        return Concat(left, right, line)
    elif op == ',':
        return Concat(left, Concat(String(', '), right, line), line)
    elif op == '/':
        return Divide(left, right, line)
    elif op == '*':
        return Multiply(left, right, line)

    elif op == '!':
        return Not(left, line)
    elif op == '>':
        return Greater(left, right, line)
    elif op == '<':
        return Lesser(left, right, line)
    elif op == '=':
        return Equals(left, right, line)
    elif op in ('!=', '<>'):
        return NotEquals(left, right, line)
    elif op in ('>=', '=>'):
        return GreaterEquals(left, right, line)
    elif op in ('<=', '=<'):
        return LesserEquals(left, right, line)
    elif op in ('&&', '&'):
        return And(left, right, line)
    elif op in ('||', '|'):
        return Or(left, right, line)

class Oper(Expression):
    def __init__(self, left, right, line=0):
        self.line = line
        self.left = left
        self.right = right

    def crawl(self, stream):
        log(self.__class__.__name__, stream)
        aind() ; self.left.crawl(stream)
        self.right.crawl(stream) ; aded()

class Subtract(Oper):
    def evaluate(self):
        left = assert_num(self.left.evaluate(), 'left')
        right = assert_num(self.right.evaluate(), 'right')
        return left - right

class Add(Oper):
    def evaluate(self):
        left = self.left.evaluate()
        right = self.right.evaluate()
        if not sametype(left, right):
            throw(BasicRuntimeError, 'unable to implicitly cast type', self)
        try:
            return left + right
        except:
            return str(left) + str(right)

class Concat(Oper):
    def evaluate(self):
        left = self.left.evaluate()
        right = self.right.evaluate()
        return str(left) + str(right)

class Divide(Oper):
    def evaluate(self):
        left = assert_num(self.left.evaluate(), 'left')
        right = assert_num(self.right.evaluate(), 'right')
        return left / right

class Multiply(Oper):
    def evaluate(self):
        left = assert_num(self.left.evaluate(), 'left')
        right = assert_num(self.right.evaluate(), 'right')
        return left * right

class Not(Oper):
    def __init__(self, left, line=0):
        self.left = left
        self.line = line

    def evaluate(self):
        return not assert_int(self.left.evaluate())

    def crawl(self, stream):
        log(self.__class__.__name__, stream)
        aind() ; self.left.crawl(stream) ; aded()

class Greater(Oper):
    def evaluate(self):
        left = self.left.evaluate()
        right = self.right.evaluate()
        if not sametype(left, right):
            throw(BasicRuntimeError, 'cannot compare different types', self)
        try:
            return left > right
        except:
            return str(left) > str(right)

class Lesser(Oper):
    def evaluate(self):
        left = self.left.evaluate()
        right = self.right.evaluate()
        if not sametype(left, right):
            throw(BasicRuntimeError, 'cannot compare different types', self)
        try:
            return left < right
        except:
            return str(left) < str(right)

class Equals(Oper):
    def evaluate(self):
        left = self.left.evaluate()
        right = self.right.evaluate()
        if not sametype(left, right):
            throw(BasicRuntimeError, 'cannot compare different types', self)
        try:
            return left == right
        except:
            return str(left) == str(right)

class NotEquals(Oper):
    def evaluate(self):
        left = self.left.evaluate()
        right = self.right.evaluate()
        if not sametype(left, right):
            throw(BasicRuntimeError, 'cannot compare different types', self)
        try:
            return not left == right
        except:
            return not str(left) == str(right)

class GreaterEquals(Oper):
    def evaluate(self):
        left = self.left.evaluate()
        right = self.right.evaluate()
        if not sametype(left, right):
            throw(BasicRuntimeError, 'cannot compare different types', self)
        try:
            return left >= right
        except:
            return str(left) >= str(right)

class LesserEquals(Oper):
    def evaluate(self):
        left = self.left.evaluate()
        right = self.right.evaluate()
        if not sametype(left, right):
            throw(BasicRuntimeError, 'cannot compare different types', self)
        try:
            return left <= right
        except:
            return str(left) <= str(right)

class And(Oper):
    def evaluate(self):
        left = assert_num(self.left.evaluate(), 'left')
        right = assert_num(self.right.evaluate(), 'right')
        return left and right

class Or(Oper):
    def evaluate(self):
        left = assert_num(self.left.evaluate(), 'left')
        right = assert_num(self.right.evaluate(), 'right')
        return left or right
