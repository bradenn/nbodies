

let pointsA = 20;
let pointsB = 20;

let radius = 900;

let dxA = (Math.PI)/pointsA
let dxB = (Math.PI*2)/pointsB

for(let i = 0; i < pointsA; i++){
    for(let j = 0; j < pointsB; j++){
        let x = radius * Math.cos(dxA*i) + Math.sin(dxB*j)
        let y = radius * Math.sin(dxA*i)+ Math.sin(dxB*j)
        let z = radius * Math.cos(dxA*i)
        console.log(`${fixed(x)} ${fixed(y)} ${fixed(z)} 0 0 0 ${fixed(100000)}`)
    }
}


function fixed(x) {
    return Number.parseFloat(x).toFixed(2);
}