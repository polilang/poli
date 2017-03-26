from . import *
from . import __version__

def background(): pass
def geterror(): pass
def version(): return __version__


Builtin('background', background, [])
Builtin('geterror$', geterror, [])
Builtin('version$', version, [])
