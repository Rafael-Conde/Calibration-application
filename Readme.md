# Calibration app

This application was develop with the intention of simulating different order instrumentation for different inputs. Currently, the application has only a single roder implemented and a single input implemented, but can be easly extended for other orders and inputs.

# Build

In order to build it's required a windows compiler, preferrably MSVC and CMake.


In the working directory(The one with CMakelists.txt), run 

```cmake
cmake -S . -B path/to/Build
```
if no CMake generator was provided, CMake should be able to find a compiler and generate the build files for that. Despite that, it's highly recommended to use MSVC(which was used for development) to compile.


then call CMake build

```cmake
cmake --build path/to/build --config release --target Calibracao-0.1
```

If you're on windows, you should be able to run the `.bat` files to compile if you have the requirements.




