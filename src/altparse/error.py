""" toobasic error manager

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

from .interpreter import Interpreter
import sys
import colorama
import msvcrt

Back = colorama.Back
exit = sys.exit

def get_line(lineno):
    """ returns a line from source given it's number """

    return [l for l in Interpreter.data.splitlines()][lineno - 1]

def highlight(line, p1, p2):
    return line[:p1] + Back.RED + line[p1:p2] + Back.RESET + line[p2:]


def lprint(lineno):
    _ = '{}|{}'.format(
        str(lineno) + ' '*(6-len(str(lineno))),
        get_line(lineno)
    )
    print(_)

def BasicTokeniseError(lineno, pos, evalue):
    """ raises a tokenizer error
    
    TokeniserError: <message>
    <line_number> |<line_with_highlighted_error>
    """
    line = highlight(get_line(lineno), pos, pos+len(evalue))
    msg = '{} |{}'.format(
        str(lineno) + ' '*(6-len(str(lineno))), line
    )
    print(msg)
    msvcrt.getch()
    exit()

def BasicParseError(token = 0, token2 = 0):
    """ raises a parser error
    
    ParseError: <message>

    Token: <token>
    or
    Got: <token> expected: <token>
    """

    from .tokenizer import Type
    offset = 0
    if token.type == Type['string']: offset = 2 # character offset for strings

    line = highlight(
        get_line(token.line), token.position,
        token.position+len(token.value)+offset
    )
    msg = ''
    if token2:
        msg = 'Got: {} expected: {}'.format(token, token2)
    elif token:
        msg = 'Token: ' + str(token)

    print('{}|{}'.format(str(token.line) + ' '*(6-len(str(token.line))), line))
    print(msg)
    msvcrt.getch()
    exit()

def BasicRuntimeError(statement):
    """ raises a Interpreter error
    
    InterpreterError: <message>
    <line_number> |<statement>
    """
    print('Traceback (most recent call last):')
    for i in range(len(Interpreter.State.state['func'])):
        if Interpreter.State.name('func', i) in ('functioncall', 'gosub'):
            lprint(Interpreter.State.get('func', i)[1].line)
    lprint(statement.line)
    msvcrt.getch()
    exit()

def throw(error, message, *args):
    """ throws an error, using a function passed in 'error' """
    prefix=''
    if error == BasicRuntimeError:
        prefix = str(args[0].__class__.__name__) + ': '
    print(error.__name__[5:] + ': ' + prefix + message)
    error(*args)