from . import *

'''to implement/finish
round(exp, {{count}, {mode}})
randomize({seed})
'''

def band(a, b): return a&b
def cbin(v): return int(v, 2)
def bnot(v): return ~v
def bor(a, b): return a|b
def bxor(a, b): return a^b
def cbrt(v): return -(-v)**(1./3.)
def exp(v): return e**v
def frac(v): return v-int(v)
def isnum(v): return v.replace(".", "", 1).isdigit()
def mod(a, b): return a%b
def pi(): return math.pi
def sign(v): return 1 if v>0 else 0 if v==0 else -1 if v<0 else None
def shift(v, b): return int(v)<<abs(b) if b<=0 else int(v)>>b


Builtin('acos', math.acos, ['v'])
Builtin('asin', math.asin, ['v'])
Builtin('atan', math.atan, ['v'])
Builtin('atan2', math.atan2, ['y', 'x'])
Builtin('hypot', math.hypot, ['x', 'y'])
Builtin('cos', math.cos, ['v'])
Builtin('cosh', math.cosh, ['v'])
Builtin('sin', math.sin, ['v'])
Builtin('sinh', math.sinh, ['v'])
Builtin('tan', math.tan, ['v'])
Builtin('todegrees', math.degrees, ['v'])
Builtin('toradians', math.radians, ['v'])

Builtin('band', band, ['a', 'b'])
Builtin('bin', cbin, ['v'])
Builtin('bnot', bnot, ['v'])
Builtin('bor', bor, ['a', 'b'])
Builtin('bxor', bxor, ['a', 'b'])

Builtin('abs', abs, ['v'])
Builtin('cbrt', cbrt, ['v'])
Builtin('sqr', math.sqrt, ['v'])
Builtin('exp', exp, ['v'])
Builtin('log', math.log, ['v'])
Builtin('log10', math.log10, ['v'])

Builtin('ceil', math.ceil, ['v'])
Builtin('floor', math.floor, ['v'])
#Builtin('round', round, ['v', 'c', 'm']) c, m =opt

Builtin('frac', frac, ['v'])
Builtin('is_number', isnum, ['v'])
Builtin('max', max, ['a', 'b'])
Builtin('min', min, ['a', 'b'])
Builtin('mod', mod, ['a', 'b'])
Builtin('pi', pi, [])
Builtin('pow', pow, ['a', 'b'])
Builtin('rnd', random.random, [])
Builtin('sgn', sign, ['v'])
Builtin('shift', shift, ['v', 'b'])
#Builtin('randomize', random.seed, ['v']) v=opt
