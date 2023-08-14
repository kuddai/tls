
# generate typescript definition file
# Use ERROR_ON_UNDEFINED_SYMBOLS=0 to use em::val value array as --embind-emit-ts can't
# generate typescript definition for it.
dsocker run \
  --rm \
  -v $(pwd):/src \
  -u $(id -u):$(id -g) \
  emscripten/emsdk \
  emcc --bind -lembind tls.cpp --embind-emit-tsd tls.d.ts -s ERROR_ON_UNDEFINED_SYMBOLS=0
 
# remove generated files
rm a.out.js a.out.wasm
# replace getBulbs(): any to getBulbs(): Bulbs[];
sed -ir "s/getBulbs\(\): any;/getBulbs\(\): Bulbs[];/g" tls.d.ts
echo -en '\nexport default function load(): Promise<MainModule>;\n' >> tls.d.ts
# sed "s/getBulbs\(\):.*/getBulbs\(\): Bulbs[];/g" tls.d.ts

# compile with docker image
docker run \
  --rm \
  -v $(pwd):/src \
  -u $(id -u):$(id -g) \
  emscripten/emsdk \
  emcc tls.cpp -o tls.mjs \
  -Os \
  --bind \
  -s EXPORT_ES6=1 \
  -s MODULARIZE=1 \
  -s SINGLE_FILE=1

# brotli -q 11 tls.mjs
