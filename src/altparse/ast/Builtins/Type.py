from . import *

def hexN(v): return int(v, 16)
def octN(v): return int(v, 8)
def binN(v): return int(v, 2)

Builtin('hex$', hex, ['v'])
Builtin('hex', hexN, ['v'])
Builtin('int', int, ['v'])
Builtin('int$', int, ['v'])
Builtin('oct$', oct, ['v'])
Builtin('oct', octN, ['v'])
Builtin('bin$', bin, ['v'])
Builtin('bin', binN, ['v'])
Builtin('val', float, ['v'])
Builtin('str$', str, ['v'])
