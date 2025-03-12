# build 
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022"
cd ..
cmake --build build --config Release

# run 
./build/Release/22127384.exe
