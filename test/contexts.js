


function fibonacci(num) {
print('input --------------------------------', num);
	  if (num <= 1){
	  	 return 1;
	  };
	var a = fibonacci(num - 1);
	print('a --------------------------------', a);
	var b = fibonacci(num - 2);
	print('b --------------------------------', b);
	var s = a + b;
	print('s --------------------------------', s);
  return  s;
}

expect_stdout(fibonacci(5), 8);
