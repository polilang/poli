![](logo.png)

# Poli programming language

# IN DEVELOPMENT - STAND BY



## Syntax

```c
/*
    Poli core syntax reference
*/



// operators: in order of precedence
::
. -> [deref] ()
~ not + - @ : // rtl
/ * %
+ -
>> <<
< > >= <=
== !=
&
^
|
and
or
= += -= *= /= >>= <<= ^= &= |= %=


// types
type
[type]
[array: size]
[(parm) -> type] // function ptr = [(int, int) -> int]
[(parm)] // [(int, int)]
[-> type] // [-> int]


// modules
with library::module::symbol; // #include <library/module/symbol.h>


// functions
// declarations
func: (); // void name (void);
func: -> (); // void name(void);
func: (arg) -> <type>; // type name (arg);
// definitions
func: -> () {} // void name (void) {}
func: {} // void name (void) {}
func: (arg: <type>, ...) -> <type> {} // type name (args) {}
return exp;


// type definitions
name: type; // typedef struct name name;
name: type {
    member: <type>; ...
}
// typedef struct {...} name;

name: union; // typedef union name name;
name: union {
    member: <type>; ...
}
// typedef union {...} name;

name: enum {
    enum, ...
}
// typedef enum {<name>_enum, ...} name;
// enum::field == enum_field

name: impl {
    // Self == <name>
    // 'variable == self.variable
    func: (args...) -> <type> {} // type name_func (args) {}
    func: (self: mut Self, args...) -> <type> {} // type name_func (name self, args) {}
}

// flow control
name: loop {}
break name;
skip name;

if cond {} else if cond {} else {}

match expr {
    constant/literal | ... {}
    * {} // default
}


// variables
// function level
name: static mut <type> = expr;
// global level
name: mut <type> = expr;
// externs
with name: <type>; // extern type name
with name as <type>; // typedef name type

```

