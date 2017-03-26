from . import *

class For(Statement):
    """ FOR expression TO expression { STEP expression }
          { block }
        NEXT
    """
    def __init__(self, start, end, step, line=0):
        self.line = line

        self.start = start
        self.end = end
        self.step = step

        self.running = False
        self.current = 0

    def execute(self):
        if not self.running:
            self.running = True

            self.start.execute()
            self.lend = self.end.evaluate()
            
            lstep = self.step.evaluate()
            self.lstep = (
                1 if Interpreter.Objects.get(self.start.left) < self.lend else -1
                ) if lstep == 'auto' else lstep

            self.current = self.start.left

            Interpreter.State.push('loop', 'forloop', [self, Interpreter.index])


    @staticmethod
    def parse():
        left = Parser.consume(Type['word']).value

        if Parser.match('='):
            right = Parser.expression()
        else:
            right = Number(1)

        start = Assignment(left, right)
        Parser.consume('to')
        end = Parser.expression()

        if Parser.match('step'):
            step = Parser.expression()
        else:
            step = String('auto')

        return __class__(start, end, step, Parser.line())


    def crawl(self, stream):
        log(self.__class__.__name__, stream)
        aind() ; aind()
        self.start.crawl(stream)
        
        log('To', stream)
        aind() ; self.end.crawl(stream) ; aded()
        
        log('Step', stream)
        aind() ; self.step.crawl(stream) ; aded()

        aded()

keywords.append('for')
keywords.append('to')
keywords.append('step')
nodes.update({'for': For})