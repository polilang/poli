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


from . import *

def mul(ls):
    return (ls[0] * mul(ls[1:]) if ls else 1) if type(ls) == list else ls

def segment():
    start = Number(1)
    length = Number()

    if Parser.match('['):
        if not Parser.match(']'):
            if not Parser.match(','):
                start = Parser.expression()
            if Parser.match(',') or Parser.get(-1).value == ',':
                length = Parser.expression()
            Parser.consume(']')

    return (start,length)

class Array:
    def __init__(self, dim, fill=0):
        self.dim = dim
        self.data = [fill for _ in range(int(assert_num(mul(dim))))]

    def __str__(self):
        return str(self.data)[1:-1]

    def __index(self, index):
        if not type(index) == list:
            raise RuntimeError("index must be a list")

        idx = 0
        for i in range(len(self.dim)):
            idx += mul(self.dim[:-i])*(index[i]-1)

        return int(assert_num(idx, 'index'))

    def __getitem__(self, index):
        return self.data[self.__index(index)]

    def __setitem__(self, index, value):
        self.data[self.__index(index)] = value

    def fill(self, fill, start=1, length=0):
        start = int(assert_num(start, 'start'))
        length = int(assert_num(length, 'length'))
        for i in range(0, length if length else len(self.data)-start+1):
            self.data[start+i-1] = fill

    def size(self, start=0, length=0):
        start = int(assert_num(start, 'start'))
        length = int(assert_num(length, 'length'))
        return len(
            self.data[start-1:][:length] if start and length else \
            self.data[start-1:] if start else \
            self.data[:length] if length else \
            self.data
        )

    def copy(self, start, length):
        start = int(assert_num(start, 'start'))
        length = int(assert_num(length, 'length'))
        new = __class__([mul(self.dim)])
        new.data = \
            self.data[start-1:][:length] if start and length else \
            self.data[start-1:] if start else \
            self.data[:length] if length else \
            self.data

        return new

    def segment(self, start, length):
        start = int(assert_num(start, 'start'))
        length = int(assert_num(length, 'length'))
        return self.data[start-1:][:length] if start and length else \
            self.data[start-1:] if start else \
            self.data[:length] if length else \
            self.data

    def overwrite(self, data, offset=1):
        offset = int(assert_num(offset, 'offset'))
        for i, v in enumerate(data):
            self.data[offset+i-1] = v

    def extend(self, data):
        if len(self.dim) > 1:
            throw(BasicRuntimeError, "cannot extend multidimension arrays", self)
        self.data.extend(data)
        self.dim[0] = len(self.data)

    def prepend(self, data):
        if len(self.dim) > 1:
            throw(BasicRuntimeError, "cannot extend multidimension arrays", self)
        for i in data:
            self.data.insert(0, i)
        self.dim[0] = len(self.data)

class NumArray(Value):
    """ identifier [] """
    def __init__(self, name, dim = None, line=0):
        self.line = line
        self.name = name
        self.dim = dim

    def evaluate(self):
#        if not Interpreter.Objects.contains(self.name):
        Interpreter.Objects.put(self.name, Array(self.dim.evaluate()))

        return Interpreter.Objects.get(self.name)

    def crawl(self, stream):
        log(self.__class__.__name__ + ': ' + self.name, stream)
        aind() ; self.dim.crawl(stream) ; aded()



class StrArray(NumArray):
    """ identifier$ [] """
    def evaluate(self):
#        if not Interpreter.Objects.contains(self.name):
        Interpreter.Objects.put(self.name, Array(self.dim.evaluate(), ''))

        return Interpreter.Objects.get(self.name)



class ArrayDim(Value):
    """ [ exp-arith,* ] """

    def __init__(self, dim, line=0):
        self.line = line
        self.dim = dim

    def evaluate(self):
        dim = list()
        for i in self.dim:
            dim.append(i.evaluate())
        return dim

    @staticmethod
    def parse():
        if Parser.match(']'):
            return __class__([], Parser.line())
        else:
            dim = [Parser.expression()]
            while Parser.match(','):
                dim.append(Parser.expression())

            Parser.consume(']')
            #dim.reverse()

            return __class__(dim, Parser.line())

    def crawl(self, stream):
        for i in self.dim:
            i.crawl(stream)



class ArrayVariable(Value):
    """ identifier [ exp-arith,* ] """

    def __init__(self, name, dim, line=0):
        self.line = line

        self.name = name
        self.dim = dim

    def evaluate(self):
        return Interpreter.Objects.get(
            self.name, dim=self.dim.evaluate() if self.dim else None
        )

    @staticmethod
    def parse():
        name = Parser.get(-2).value
        dim = ArrayDim.parse()
        return __class__(name, dim, Parser.line())

    def crawl(self, stream):
        log(self.__class__.__name__+ ': ' + self.name, stream)
        if self.dim:
            aind() ; log('Dim', stream)
            aind() ; self.dim.crawl(stream) ; aded() ; aded()
