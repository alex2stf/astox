expect_stdout(1 + 2 + 3, 6);
expect_stdout(25 % 3 + 4 % 5 - 2, 3);
expect_stdout(2>>4, 0);
expect_stdout(2|5, 7),
expect_stdout("494847", 494847),
expect_stdout(-2 + 1, -1),
expect_stdout(7 * 2 / 2 - 3 % 5, 4),
expect_stdout( 1 + 2 * 3 / (2 + 1 - 1), 4),
expect_stdout((1+2) * 2, 6),
expect_stdout(1+2*2, 5),
expect_stdout(5 + 2 * 3 / 2 + 1 - 1 , 8),
expect_stdout((5 + 2 * 3) / 2 + 1 - 1, 5.5),
expect_stdout(9 - 2 * 2, 5),
expect_stdout(1 + 2, 3),
expect_stdout(1 + 2 + 3, 6),
expect_stdout(9 - (7 + 1), 1),
expect_stdout(9 - 7 + 1, 3),
expect_stdout(5 + 2 * 3 / 2 + 1 - 1, 8),
expect_stdout(7 % 2, 1),
expect_stdout(15 % 2 * 2, 2),
expect_stdout(2 * 3 + 5 * 2, 16),
expect_stdout(7 * 2 / 2, 7),
expect_stdout(7 * 2 / 2 + 3 * 5, 22),
expect_stdout(7 * 2 / 2 + 3 % 5, 10),
expect_stdout(7 * 2 / 2 - 3 % 5, 4),
expect_stdout(25 % (3 + 4) % 5, 4),
expect_stdout(25 % (3 + 4) % 5 + 9, 13),
expect_stdout(25 % (3 + 4) % (5 + 9), 4),
expect_stdout(25 % 3 + 4 % (5 + 9), 5),
expect_stdout(25 % 3 + 4 % 5, 5),
expect_stdout(25, 25),
expect_stdout(2+5, 7),
expect_stdout(a = 4, 4),
expect_stdout(a + 2, 6),
expect_stdout(a++, 5),
expect_stdout(a--, 4),
expect_stdout(2+5-3, 4),
expect_stdout(2<<4, 32),
expect_stdout(1+1<<4, 32),
expect_stdout(2<<4+1, 64),
expect_stdout(2<<4<<3, 256),
expect_stdout(2 << 4, 32),
expect_stdout(2 | 5 | 8, 15);
expect_stdout(1+"string", 1string);
expect_stdout(1*"string", NaN);
expect_stdout("string" / 2 + 2, NaN);
expect_stdout("string" % 3, NaN);
expect_stdout("string" - 3, NaN);
expect_stdout(undefined + "string", NaN);
expect_stdout(undefined + NaN, NaN);
expect_stdout({} + {}, {});
expect_stdout({a:1} + {b:2}, {a:1,b:2});



var a = [], b = [];
for(var i = 0; i < 10; i++){
	switch(i){
		case 0: a.push(0);  b.push(2);
		case 1: a.push(1);
		case 2: b.push(2); break;
		default: b.push('default');
	}
}
expect_stdout(a, [0,1,1]);
expect_stdout(b, [2,2,2,2,'default','default','default','default','default','default','default']);




function testFunction( value, cond, callback ) {
	  return  arguments;
}

expect_stdout(testFunction(1, 'two', 3, false, [], {}), [1,'two',3,false,[],{}]);

function testFunction() {
  return  1;
}

expect_stdout( testFunction(), 1);


function testFunction( value, cond, callback ) {
	var list = [];
	for(var i = 0; i < arguments.length; i++){
		list.push(arguments[i]);
	}
	return list;
}

expect_stdout(testFunction(1, 'two', 3, false, [], {}), [1,'two',3,false,[],{}]);

function sum(a){
	return a + a;
}
expect_stdout(sum(1) + sum(2), 6);

function rec(num){
	if(num < 0) { return num;}
	return rec(num - 1);
}
expect_stdout(rec(10), -1);


function fibonacci(num) {
  if (num <= 1) { return 1; }
  return fibonacci(num - 1) + fibonacci(num - 2);
}

expect_stdout(fibonacci(5), 8);
expect_stdout(fibonacci(2), 2);
expect_stdout(fibonacci(1), 1);
expect_stdout(fibonacci(10), 89);
expect_stdout(fibonacci(15), 987);
expect_stdout(fibonacci(16), 1597);
