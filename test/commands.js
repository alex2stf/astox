

var arr = [];

for(var j in {a:1, b:4}){
	arr.push(j);
}
expect_stdout(arr, ['a','b']);



for(var i = 0; i < 15; i++){
	print(i);
}
expect_stdout(i, 15);

if(i > 20) {
	print("i > 20");
}
else if(i == 19) {
	print("i == 19");
} 
else {
	print("i == 15");
}

for(var i = 0; i < 6; i++){
	if(i == 4){
		break;
	}
}
expect_stdout(i, 4);


arr = [];

for(var j in {a:1, b:4, c:25, d:48}){
	if(j == 'c') {
		continue;
	}
	arr.push(j);
}

expect_stdout(arr, ['a','b','d']);

function tfor(){
	var arr = [];
	for(var j in {a:1, b:4, c:25, d:48}){
		if(j == 'c') {
			return arr;
		}
		arr.push(j);
	}
}
expect_stdout(arr, ['a','b','d']);
expect_stdout(tfor(), ['a','b']);

arr = [];
for(var i = 0; i < 5; i++){
	if(i==2){
		continue;
	}
	
	if(i==3){
		continue;
	}
	arr.push(i);
}

expect_stdout(arr, [0,1,4]);
