""" toobasic tokenizer
"""

from .error import throw, BasicTokeniseError, get_line


token_type = {
    'word': 1,
    'keyword': 11,
    'number': 21,
    'string': 22,
    'label': 4,
    'operator': 5,
    'assign': 50,
    'paren': 8,
    'function':9,
    'EOL': 10,
    'EOF': 0,
    'continue': -1,
}

tokenize_state = {
    'default': 0,
    'word': 1,
    'number': 2,
    'string': 3,
    'comment': 4,
    'operator': 5,
    'continue': 6
}

Type = token_type
State = tokenize_state

def dkey(type):
    """ get the key name from value
    used for debugging the token stream
    """
    return str(list(Type.keys())[ list(Type.values()).index(type) ])

class Token:
    """ Defines a single token """

    def __init__(self, value, type, line=0, pos=0):
        self.value = value
        self.type = type
        self.line = line # used for debugging
        self.position = pos # used for debugging

    def __str__(self):
        """ conversion to string form
        used for debugging the token stream
        """
        key = dkey(self.type)
        if dkey(self.type) == 'EOL':
            return '.'

        return key[:1].upper() + '' + str(self.value) + '  '


def tokenize(source):
    """ tokenizes source code for use with the parser

    returns token stream
    """

    tokens = []
    token = '' # current token
    state = State['default']

    operators = ('=', '+', '-', '*', '/', '<', '>', ';', ',', '&', '|', '!', '^')
    compassign = ('*=', '/=', '+=', '-=')
    boolean = ('<>', '!=', '<=', '>=', '=>', '=<', '||', '&&')

    from .ast import kwords
    keywords = kwords()

    line = 1 # line tracker
    pos = -1 # position in line

    def p(token, pos):
        return pos-len(token)

    linebeg = True
    cmt = ''

    for i, c in enumerate(source):
        pos += 1
        if not c in (' ', '!', '\n') and linebeg:
            linebeg = False
        elif c == '!' and linebeg and not cmt == 'double':
            state = State['comment']
            if source[i-1] == '!': # an ugly block comment hack
                cmt = 'double'
            continue

        if state == State['continue'] and not c == '\n':
            throw(BasicTokeniseError,
                'expected newline', line, pos, c)

        if state == State['default']:
            if c.isalpha() or c in ('_', '@', '$', '?'):
                token = c
                state = State['word']
            elif c.isdigit() or c == '.':
                token = c
                state = State['number']

            elif c in ('(', ')', '[', ']', '{', '}'):
                tokens.append(Token(c, Type['paren'], line, pos))
            elif c == '"':
                state = State['string']
            elif c == '%':
                token += c
                state = State['comment']
            elif c == '!':
                token += c
                state = State['operator']
            elif c in operators:
                token = c
                state = State['operator']

            elif c == '~':
                if not tokens[-1].value == ',':
                    tokens.append(Token('~', Type['continue'], line, pos))
                    pos+=1
                state = State['continue']

            elif c not in (' ', '\n', ',', ':'):
                throw(BasicTokeniseError,
                    'unexpected character', line, pos, c)

        elif state == State['operator']:
            if c in operators:
                token += c
                if len(token)>1 \
                and token not in compassign+boolean:
                    tokens.append(
                        Token(token[0], Type['operator'], line, p(token, pos)))
                    token = token[1:]
            else:
                if token in compassign:
                    tokens.append(
                        Token(token, Type['assign'], line, p(token, pos))
                    )
                else:
                    #if c.isdigit() and token == '-':
                    #    token += c
                    #    state = State['number']
                    #    continue
                    #else:
                        tokens.append(
                            Token(token, Type['operator'], line, p(token, pos))
                        )
                if c.isdigit():
                    token = c
                    state = State['number']
                elif c.isalpha() or c in ('_', '@', '$'):
                    token = c
                    state = State['word']
                elif c == '(':
                    tokens.append(
                        Token(c, Type['paren'], line, pos)
                    )
                    token = ''
                    state = State['default']
                elif c == '"':
                    token = ''
                    state = State['string']
                else:
                    token = ''
                    state = State['default']

        elif state == State['word']:
            if c.isalnum() or c in ('_','@','$', '.'):
                token += c
            else:
                ptoken = token.lower()
                token = ''
                state = State['default']

                if not c == ':' and not ptoken == 'rem' \
                and not ptoken in keywords: # not a label/comment/keyword
                    tokens.append(
                        Token(
                            ptoken, Type['word'],line, p(ptoken, pos)
                        )
                    )

                if ptoken in keywords:
                    tokens.append(
                        Token(
                            ptoken, Type['keyword'],
                            line, p(ptoken, pos)
                        )
                    )

                if c == ':' and ptoken in keywords:
                    throw(BasicTokeniseError, 'keyword cannot be a label',
                        line, p(ptoken, pos), ptoken)


                if c == ':': # if line is a label
                    tokens.append(
                        Token(
                            ptoken, Type['label'], line, p(ptoken, pos)
                        )
                    )
                    continue

                elif ptoken == 'rem' or c == '%': # comment
                    state = State['comment']

                elif c in operators:
                    token = c
                    state = State['operator']

                elif c in ('(', ')', '[', ']', '{', '}'):
                    tokens.append(Token(c, Type['paren'], line, pos))

                elif c == '"':
                    state = State['string']

                elif c not in (' ', '\n', ','):
                    throw(BasicTokeniseError,
                        'unexpected character', line, pos, c)

        elif state == State['number']:
            if c.isdigit() or c == '.':
                token += c
            else:
                tokens.append(
                    Token(token, Type['number'], line, p(token, pos))
                )
                if not c == ' ':
                    token = c
                else:
                    token = ''
                state = State['default']

                if c in (')', ']', '}'):
                    tokens.append(Token(c, Type['paren'], line, pos))
                elif c in operators:
                    state = State['operator']
                elif c.isalpha() or c in ('_', '@', '$'):
                    state = State['word']
                elif c == '%':
                    state = State['comment']
                elif c == '~':
                    tokens.append(Token(',', Type['operator'], line, p(',', pos)))
                    state = State['continue']
                elif c not in (' ', '\n', ',') + operators:
                    throw(BasicTokeniseError,
                        'unexpected character', line, pos, c)

        elif state == State['string']:
            if not c == '"':
                token += c
            else:
                tokens.append(
                    Token(token, Type['string'], line, p(token, pos-1))
                )
                token = ''
                state = State['default']

        elif state == State['comment']:
            #contains ugly block comment workarounds
            if c == '\n' and not cmt == 'double':
                state = State['default']
            elif c == '\n':
                tokens.append(Token('', Type['EOL'], line, pos))
                line+=1
                linebeg = True
                pos=-1
            if c == '!' and source[i-1] == '!' and cmt == 'double':
                cmt = ''
                state = State['default']

        if c in ('\n', ':') and not state in (State['string'], State['comment']):
            if not state == State['continue']:
                tokens.append(Token('', Type['EOL'], line, pos))
            if not c == ':':
                line += 1
            pos = -1
            state = State['default']
            linebeg = True

    return tokens
