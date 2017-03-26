""" Copyright (C) 2016  Daniels Kursits (evolbug)

    tooBASIC is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    tooBASIC is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with tooBASIC.  If not, see <http://www.gnu.org/licenses/>.
"""

from . import collector

collector.autocollect()

def spool():
    # node signature pool
    return {
        signature: node \
            for module in collector.mpool \
            for signature, node in module.nodes.items()
    }

def kwords():
    # keyword pool
    return [
        kw \
            for module in collector.mpool \
            for kw in module.keywords
    ]
