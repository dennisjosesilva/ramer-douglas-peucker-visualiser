# Ramer-Douglas-Peucker line simplification visualiser

The program loads a binary image, compute its contour 
and interactively (change parameter) perform 
Ramer-Douglas-Pecker simplification. 

It includes a 3rd-party library (psimpl) for 
Ramer-Douglas-Pecker implemetation. It is 
available in this (post)[https://www.codeproject.com/Articles/114797/Polyline-Simplification].

## Dependencies

psimpl (header library) include in "include/lib"
Qt 5.13
OpenGL
cmake

## Running 

```bash
$ mkdir build && cd build
```

```bash
$ cmake ..
```

```bash
$ cmake --build .
```

