from . import *

class Clear(Statement):
    """ CLS """

    def execute(self):
        import os
        os.system('cls')

    @staticmethod
    def parse():
        return __class__(Parser.line())

keywords.append('cls')
nodes.update({'cls': Clear})