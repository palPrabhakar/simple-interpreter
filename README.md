# A Simple Interperter

0. Only double data type supported
1. No support for for-loops.
2. Only one return statement in the function body.
3. Mandatory else clause in if-else-statement.
4. Support for only two scope level - Global and Function.

## Examples

### Declaring variables
```
let x = 5;
let y = x*5;
let z = x+y-5;
mut x = 11;
```

### If Conditions

```
let x = 5;

if ( x < 10) { mut x = x * 2; } else { mut x = x / 2; }

if( x < 10) { if (x > 5) { mut  x = x*2; } else { mut x = x * 4; } } else { mut x = x /2; }
```

### While loops
```
let x = 1;
while(x < 10) { mut x = x + 1; }

let i = 1;
while(i < 10) { if (x < 5) { mut x = x*2; } else { mut x = x/2; } mut i = i + 1; }

```

### Functions
```
fn double(x) { return x*2; }

let a = call double(2);

let b = call double(a);

fn foo(x, y) { return call double(x) + y; }

let x = call add_doubles(1, 2);
```
