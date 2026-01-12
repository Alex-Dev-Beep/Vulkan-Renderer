rmdir /s /q build
mkdir build
cd build
cmake -S .. -B . -G "MinGW Makefiles"
mingw32-make
cd ..
echo Commands for compiling shaders:
echo glslc "shaders\simple_shader.frag" -o "shaders\simple_shader.frag.spv"
echo glslc "shaders\simple_shader.vert" -o "shaders\simple_shader.vert.spv"