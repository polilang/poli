keywords = []

packages = ['Base', 'Function', 'List', 'Loop', 'Bundle', 'Array', 'Builtins']
mpool = []

def autocollect():
    from importlib import import_module
    for i in packages:
        mpool.append(import_module('basic.ast.'+i))
