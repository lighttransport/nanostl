rmdir /q /s build
mkdir build

cmake.exe -G "Visual Studio 16 2019" -Ax64 -Bbuild -H.
