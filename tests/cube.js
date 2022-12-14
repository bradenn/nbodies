for(let i = -8; i < 8; i++) {
	for (let j = -8; j < 8; j++) {
		for (let k = -8; k < 8; k++) {
			let a = 1024, b = 1024, c = 1024;
			let v = 2E-4;
			let d = 1000;
			console.log(`${fixed(64+i*128)} ${fixed(64+j*128)} ${fixed(64+k*128)} 0 0 0 ${fixed(100000)}`);
		}
	}
}

function fixed(x) {
	return Number.parseFloat(x).toFixed(16);
}
