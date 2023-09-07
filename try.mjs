import loadWASM from './tls.mjs';

const wasm_module = await loadWASM();
console.log(wasm_module.getRed().value);
console.log(wasm_module.getBulb());
console.log(wasm_module.getBulbs([1, 2, 3]));
console.log(Array.from(Object.keys(wasm_module)));
console.log(Array.from(Object.keys(wasm_module.Arrow)));
console.log(Array.from(Object.keys(wasm_module.Arrow.prototype)));
