from . import *

def Builtin(name, call, args):
    FunctionDeclare(
        name,
        args,
        0,
        builtin=True,
        syscall=call
    )
