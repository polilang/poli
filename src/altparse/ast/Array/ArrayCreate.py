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

class ArrayCreate(Statement):
    """ DIM id-array {, id-array }* """

    def __init__(self, name, dim, line=0):
        self.line = line
        self.name = name
        self.dim = dim

    def execute(self):
        if self.name[-1] == '$':
            StrArray(self.name, self.dim).evaluate()
        else:
            NumArray(self.name, self.dim).evaluate()

    @staticmethod
    def parse():
        if not Parser.match(Type['word']):
            throw(BasicParseError,
                "ArrayCreate: No name specified", Parser.get(0)
            )

        name = Parser.get(-1).value
        Parser.match('[')
        dim = ArrayDim.parse()

        if Parser.get(0).value == ',':
            Parser.tokens[Parser.position] = Token(
                'dim', Type['keyword'], 
                Parser.get(0).line,
                Parser.get(0).position
            )

        return __class__(name, dim, Parser.line())

    def crawl(self, stream):
        log(self.__class__.__name__ + ': ' + self.name, stream)
        aind() ; log('Dimensions', stream)
        aind() ; self.dim.crawl(stream) ; aded() ; aded()

keywords.append('dim')
nodes.update({'dim': ArrayCreate})