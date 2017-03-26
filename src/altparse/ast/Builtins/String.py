from . import *

''' to implement/finish:
ucode
mid$
trim$()
word$(str, i, splitch)
chr$(nexp...)
using$(<locale>, <format>, <exp>...)
ascii(str, i)
decode$
encode$
format
format_using
starts_with(sub, str, start)
'''

def decode(chs, buf): pass
def encode(chs, buf): pass
def ends_with(sub, str): return str[len(str)-len(sub):] == sub
def formatS(pattern, number): pass
def is_in(sub, base, start=0): pass #start=opt
def left(str, c): return str[:len(str)-c+1] if c>0 else str[:-c-1]
def right(str, c): return str[len(str)-c:] if c>0 else str[-c:]
#def mid(str, start, count=len(str)) count=opt
def lower(str): return str.lower()
def upper(str): return str.upper()
def replace(str, src, repl): return str.replace(src, repl)
def trim(str, char=' '): return str.strip(char)
def word(str, n, ch=' '): return str.split(ch)[n]
#def ucodeindex(str, idx=0): return str[idx].decode('utf-8')

Builtin('len', len, ['v'])

Builtin('chr$', chr, ['v']) #multi chars

Builtin('decode$', decode, ['chs', 'buf'])
Builtin('encode$', encode, ['chs', 'buf'])
Builtin('format$', formatS, ['p', 'n'])
#Builtin('ucode', ucodeindex, ['str']) #idx=opt

Builtin('ends_with', ends_with, ['sub', 'str'])
Builtin('left$', left, ['str', 'c'])
Builtin('right$', right, ['str', 'c'])
Builtin('lower$', lower, ['str'])
Builtin('upper$', upper, ['str'])
Builtin('replace$', replace, ['stc', 'src', 'repl'])
Builtin('trim$', trim, ['str']) # char=opt
Builtin('word$', word, ['str', 'n']) #ch=opt
