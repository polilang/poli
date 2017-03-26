from . import *

class Next(Statement):
    """ NEXT """

    def execute(self):
        if Interpreter.State.name('loop') == 'forloop':
            loopd = Interpreter.State.get('loop')[0]
            current = Interpreter.Objects.get(loopd.current)
            if (not current + loopd.lstep > loopd.lend) if loopd.lstep > 0 \
            else (not current + loopd.lstep < loopd.lend):
                Interpreter.Objects.put(loopd.current, current+loopd.lstep)
                Interpreter.index = Interpreter.State.get('loop')[1]
            else:
                Interpreter.Objects.put(loopd.current, current+loopd.lstep)
                loopd.running = False
                Interpreter.State.pop('loop')
        else:
            throw(BasicRuntimeError, 'Next: outside of a FOR loop', self)

    @staticmethod
    def parse():
        Parser.match(Type['word'])
        return __class__(Parser.line())

    def crawl(self, stream):
        aded() ; log(self.__class__.__name__, stream)

keywords.append('next')
nodes.update({'next': Next})