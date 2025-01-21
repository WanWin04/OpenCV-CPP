# build 
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022"

# run 
cmake --build build --config Release
./build/Release/CV01.exe
