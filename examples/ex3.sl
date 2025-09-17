fn double(x) { return x*2; }
let a = call double(2);
print(a);
fn add(x, y) { return x+y; }
let b = call add(a, 1);
print(b);
fn triple(x) { let d = call double(x); return call add(d, x); }
let c = call triple(b);
print(c);
