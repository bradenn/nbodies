let density = 10;


let mass = 10000;
let distance = 50;

let radius = 1024;


// Cube root of 1536 is 11.53, so 12
let dx = (radius) / (density * 2);

for (let i = -density; i <= density; i++) {
    for (let j = -density; j <= density; j++) {
        for (let k = -density; k <= density; k++) {
            console.log(`${fixed(i * dx)} ${fixed(j * dx)} ${fixed(k * dx)} 0 0 0 ${fixed(mass)}`);
        }
    }
}

function fixed(x) {
    return Number.parseFloat(x).toFixed(16);
}