![](logo.png)

# Poli programming language

# IN DEVELOPMENT - STAND BY



## Syntax

### Comments

```rust
// single line

/*
	multi
	line
*/
```

### Variables

```rust
// immutable vars
i32: var = 1


// mutable
mut i32: mutable
mutable = 2


// multiline/multivar declaration/initialization
mut i32:
	somevar; othervar // semicolon is a separator character equivalent to a line break
	someotherinit = 123


// pointers
// immutable pointer, immutable data
[i32]: ip_ivar = @var // const int const * ip_ivar = &var

// mutable pointer, mutable data
mut [mut i32]: mp_mvar = @var // error: mutable value pointer declaration to immutable value

// dereferencing
mutable = [ip_ivar] // mutable = 1


// arrays
[mut i32, 5]: arr = // an expression can be broken up as long as
	(1, 2, 3, 4, 5) // the last character on the line is a symbol

// indexing
mutable = [arr, 4] // mutable = arr[4] = 5
[arr, 1] = mutable // arr[1] = mutable = 5
mutable = [arr]    // mutable = *arr = arr[0] = 5


// casting
i32: integer = i32: 1.2345 // float -> int
```

### Branching

```rust
if true
	if a > b; print("a is bigger")
	else if a < b; print("b is bigger")
	else; print("a == b")
		
else
	print("false")
```

### Loops

```rust
i32: i = 0

while i < 10
	if i % 2; continue
	if i == 8; break
	
	print(i)


for i32: j, range(10)
	print(i) // 0 1 2 3 4 5 6 7 8 9
```

### Functions

```rust
// if multiple parameters have the same type,
// it only needs to be specified once
i64: sum (i32: a, b)
	return a + b

[i8]: next ([i8]: str)
	return str + 1

print( sum(123, 321) ) // 444
print( next("hello") ) // ello
```

###Modules

```rust
// import the whole module
with math, io
io.print( math.max(1, 2) ) # 2

// import specific public symbols
with min, max from math
with print from io

print( min(1, 2), max(1, 2) ) # 1 2


// make public for importing in other modules
pub i64: sum (i32: a, b)
	return a + b
```

### Classes

```rust
type StackNode
	[self]: last = Null
	i32: data = 0

type Stack
	[StackNode]: top = Null
	u32: size = 0
	
	// 'self' is a keyword referring to the class when casting/declaring,
	// and the instance when used in an expression (passed as first argument to the method).
	// When accessing members, 'self' can be omitted, leaving only dot prefix
	self: push (self, i32: data)
		[StackNode]: node = new(StackNode:)
		[node] = (.top, data)
		self.top = node // explicit 'self'
		.size += 1      // implicit 'self'
	
	Option{i32}: pop (self)
		if not .top; return None

		i32: data = .top.data
		.top = .top.last
		.size -= 1

		return Some(data)
```

### Enums

```rust
enum Colors // C-like enum
	RED // 0 is default
	BLUE
	GREEN

// in a context where the enum is directly used, values can be directly accessed,
// unless shadowed by a definition with the same name (i.e. a global, or definition in a function)
Colors: color = GREEN
print(color, Colors.RED, Colors.BLUE, Colors.GREEN) // 2 0 1 2

enum Union // Rust-like enum
	Empty
	u32: unsigned // can store variables which will be strapped in a tagged union
	f32: float

Union: thing = float(1.2345)
print(thing) // 1.2345 -- passthrough of whichever enum value is currently set
```

### Match

```rust
// match matches variable contents with values and enums

bool: val = True

match val
	True; print("true")
	False; print("false")


enum Things
	NINE = 9
	ELEVEN = 11
	THIRTEEN = 13

for i32: i, range(15)
	if range(0, 10, 2).has(i); print("even")

	match i
		3, 5, 7, 11, 13; print("prime") // individual values are matched

		Things; // an enum type will be destructured into individual numbers
			print("{i}") // duplicate values (11, 13) will be matched by the former branch

		*; print("number: {i}") // default case


enum Union
	Empty
	u32: unsigned
	f32: float

Union: e = Empty; u = unsigned(100); f = float(1.2345)

for Union: i, [Union, 3]:(e, u, f)
	match i // matching the contents of a Rust-like enum variable
		Empty; print("union is empty")
		unsigned; print("union has unsigned: {unsigned}")
		float; print("union has float: {float}")
```

### Generics

```rust
// templated function
{T} T: sum (T: a, b)
	return a + b

// templated enum, emulating Rust's Option type
{T} enum Option
	None
	T: Some

Option{i32}: div (i32: divident, divisor)
	// short circuiting boolean operators
	return divisor and Some(divident/divisor) or None

// enums with match
match div(1, 0)
	None;
		print("division by 0")
	Some;
		print("result: {Some}")
```




