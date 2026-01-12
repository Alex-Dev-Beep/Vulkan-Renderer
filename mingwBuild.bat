rmdir /s /q build
mkdir build
cd build
cmake -S .. -B . -G "MinGW Makefiles"
mingw32-make
cd ..
glslc "shaders\simple_shader.frag" -o "shaders\simple_shader.frag.spv"
glslc "shaders\simple_shader.vert" -o "shaders\simple_shader.vert.spv"