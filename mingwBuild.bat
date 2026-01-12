mkdir build
cmake -S . -B build -G "MinGW Makefiles"
cmake --build build
cd ..
echo Commands for compiling shaders:
echo glslc "shaders\simple_shader.frag" -o "shaders\simple_shader.frag.spv"
echo glslc "shaders\simple_shader.vert" -o "shaders\simple_shader.vert.spv"