""" toobasic abstract syntax tree

    Copyright (C) 2016  Daniels Kursits (evolbug)

    tooBASIC is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    tooBASIC is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with tooBASIC.  If not, see <http://www.gnu.org/licenses/>.
"""


from .Core import Expression
from ..interpreter import Interpreter
from ..parser import log
from ..error import throw, BasicRuntimeError

def assert_int(v, name=''):
    if type(v) == Variable:
        name = Variable.name
    if type(v) == Number:
        v = v.evaluate()
    if round(float(v)) == float(v):
        return int(v)
    throw(BasicRuntimeError, "Not an integer: "+(name if name else v),
        Interpreter.ast[Interpreter.index])

def assert_num(v, name=''):
    if type(v) == Variable:
        name = Variable.name
    if type(v) == Number:
        v = v.evaluate()
    if type(v) in (int, float):
        return float(v)
    try:
        return float(v)
    except:
        throw(BasicRuntimeError, "Not a number: "+(name if name else v),
            Interpreter.ast[Interpreter.index])

def assert_str(v, name=''):
    if type(v) == Variable:
        name = Variable.name
    if type(v) == String:
        v = v.evaluate()
    if type(v) == str:
        return str(v)
    throw(BasicRuntimeError, "Not a string: "+(name if name else v),
        Interpreter.ast[Interpreter.index])

def sametype(a, b):
    at = type(a) ; bt = type(b)
    if at in (int, float) and bt in (int, float):
        return True
    elif at == bt:
        return True
    return False


class Value(Expression):
    def __init__(self, value):
        self.value = value

    def evaluate(self):
        return self.value

    def crawl(self, stream):
        log(self.__class__.__name__ + " '" + str(self.value) + "'", stream)


class String(Value):
    def __init__(self, value=''):
        self.value = str(value)

class Number(Value):
    def __init__(self, value=0):
        self.value = float(value)

class Variable(Value):
    def __init__(self, name, value=0, line=0):
        self.line = line
        self.name = name

        if name[-1] == '$': value = ''
        self.value = value

    def evaluate(self):
        if not Interpreter.Objects.contains(self.name):
            Interpreter.Objects.put(self.name, self.value)

        return Interpreter.Objects.get(self.name)

    def crawl(self, stream):
        log(self.__class__.__name__ + ' ' + self.name, stream)
