let x = 4;
if (x<10) { mut x = x*4; } else { mut x = x /2; }
print(x);
let y = 5;
if(x<10) { if (y>10) { mut x = y/x;  } else { mut x = x*y; } } else {mut x = x/y; }
print(x);
