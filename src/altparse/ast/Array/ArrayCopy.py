from . import *

class ArrayCopy(Statement):
    """ ARRAY.COPY id[{seg}], <id[{exp-num}]>"""

    def __init__(self, src, start, length, trg, extra=None):
        self.line = Parser.line()
        
        self.src = src
        self.start = start
        self.length = length

        self.trg = trg
        self.extra = extra

    def execute(self):
        start = assert_int(self.start.evaluate(), 'start')
        length = assert_int(self.length.evaluate(), 'length')

        new = Interpreter.Objects.get(self.src).copy(start, length)
    
        if Interpreter.Objects.contains(self.trg):
            Interpreter.Objects.get(self.trg).overwrite(
                new.data, assert_int(self.extra.evaluate()) if self.extra else 1
            )
        else:
            if self.extra:
                extra = assert_int(self.extra.evaluate(), 'extra')
                fill = ' ' if self.trg[-1] == '$' else 0.0
                if extra>=0:
                    new.extend([fill for _ in range(extra)])
                else:
                    new.prepend([fill for _ in range(abs(extra))])
            Interpreter.Objects.put(self.trg, new)

    @staticmethod
    def parse():
        src = Parser.consume(Type['word']).value
        start, length = segment()
        Parser.consume(',')

        trg = Parser.consume(Type['word']).value
        if Parser.match('[') and not Parser.match(']'):
            extra = Parser.expression()
            Parser.match(']')
        else:
            extra = None

        return __class__(src, start, length, trg, extra)

    def crawl(self, stream):
        log(self.__class__.__name__ + ': ' + self.src + '=>'+ self.trg, stream)
        if self.start:
            aind() ; log('Start', stream) 
            aind() ; self.start.crawl(stream) ; aded() ; aded()
        if self.length:
            aind() ; log('Length', stream)
            aind() ; self.length.crawl(stream) ; aded() ; aded()
        if self.extra:
            aind() ; log('Extra', stream)
            aind() ; self.extra.crawl(stream) ; aded() ; aded()

keywords.append('array.copy')
nodes.update({'array.copy': ArrayCopy})