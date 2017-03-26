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

from ..parser import Parser, log

class Node:
    def __init__(self, line=0):
        self.line = line
    def crawl(self, stream):
        log(self.__class__.__name__, stream)
    def __str__(self):
        return str(self.__module__.split('.')[-1] + '.'+ self.__class__.__name__)


class Statement(Node):
    def execute(self): pass

class Expression(Node):
    def evaluate(self): pass
