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

from main import __version__

from ...interpreter import Interpreter
from ...error import throw, BasicRuntimeError, BasicParseError, get_line, lprint

from ..Function import FunctionDeclare
from ..collector import keywords

from .Types import Builtin

import math
import random
import uptime
import time

nodes = {}
pool = [
    'Math',
    'String',
    'Type',
    'Time',
    'Other',
]

for module in pool:
    try:
        exec('from .{} import *'.format(module))
    except:
        print('WARNING: missing command class {}'.format(module))
        pass
