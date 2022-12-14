for(let i = 0; i < 2000; i++){
	let a = 1024, b = 1024, c = 1024;
	let v = 2E-4;
	let d = 1000;
	console.log(`${512 - Math.random()*a} ${512 - Math.random()*b} ${512 - Math.random()*c} ${1E-4 - Math.random()*v} ${1E-4 - Math.random()*v} 0 ${Math.random()*d}`);
}
