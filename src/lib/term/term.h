/*
   evolbug 2017, Unlicense - public domain
   ANSI terminal codes
*/



#ifndef EVOLBUG_TERM
#define EVOLBUG_TERM



#define ESC "\033["

#define Treset   ESC"0m"

#define Mbold    ESC"1m"
#define Mitalic  ESC"3m"
#define Muline   ESC"4m"
#define Minvert  ESC"7m"
#define Mhide    ESC"8m"
#define Mstrike  ESC"9m"

#define Mboldx   ESC"22m"
#define Mitalicx ESC"23m"
#define Mulinex  ESC"24m"
#define Minvertx ESC"27m"
#define Mhidex   ESC"28m"
#define Mstrikex ESC"29m"

#define Fblack   ESC"30m"
#define Fred     ESC"31m"
#define Fgreen   ESC"32m"
#define Fyellow  ESC"33m"
#define Fblue    ESC"34m"
#define Fmagenta ESC"35m"
#define Fcyan    ESC"36m"
#define Fwhite   ESC"37m"
#define Freset   ESC"39m"

#define Bblack   ESC"40m"
#define Bred     ESC"41m"
#define Bgreen   ESC"42m"
#define Byellow  ESC"43m"
#define Bblue    ESC"44m"
#define Bmagenta ESC"45m"
#define Bcyan    ESC"46m"
#define Bwhite   ESC"47m"
#define Breset   ESC"49m"

#define Chome    ESC"H"
#define Csave    ESC"s"
#define Cload    ESC"u"
#define Cshow    ESC"?25l"
#define Chide    ESC"?25h"

#define clrCtoE  ESC"0K" // cursor to EOL
#define clrStoC  ESC"1K" // SOL to cursor
#define clrStoE  ESC"2K" // SOL to EOL
#define clrALL   ESC"2J"

#define Ssave    ESC"?47h"
#define Sload    ESC"?47l"

#define Cput(x, y) ESC #y ";" #x "H"

#define Cup(n) ESC #n "A"
#define Cdn(n) ESC #n "B"
#define Crt(n) ESC #n "C"
#define Clt(n) ESC #n "D"



#endif
