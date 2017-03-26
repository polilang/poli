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


from ...interpreter import Interpreter
from ...error import throw, BasicRuntimeError, BasicParseError
from ...parser import Parser, log, aind, aded
from ...tokenizer import Type

from ..Core import Statement, Expression
from ..Types import Value, Variable

from ..collector import keywords


nodes = {}
pool = [
    'FnDef', 'FnEnd', 'FnRtn', 'FnCall',
]

for module in pool:
    try:
        exec('from .{} import *'.format(module))
    except:
        print('WARNING: missing command class {}'.format(module))
        pass
