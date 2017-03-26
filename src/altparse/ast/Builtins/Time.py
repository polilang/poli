from . import *

# time(year, mon, day, h, m, s)

Builtin('clock', uptime.uptime, [])
Builtin('time', time.time, [])