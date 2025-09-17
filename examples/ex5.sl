let x = 1;
let i = 0;
while (i < 10) { if(i < 5) { mut x = x*2; if (x>5) { break; } else { mut x=x+2; } } else { mut x = x+1; } mut i = i + 1; }
print(x);
print(i);
