const binding = require("./binding");

console.log(binding)

const out = binding.init("mic","output/%02d.wav", "wavenc", -45)

console.log(out)


