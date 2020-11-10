# [Work In Progress] AtilaCalculatorSoftware v2

Based on [BorderWidgetQt example](https://lorensen.github.io/VTKExamples/site/Cxx/Qt/BorderWidgetQt/)

## Dependencies

- [GiDPost](https://www.gidhome.com/) 2.7
- [zlib](https://zlib.net/)
- [HDF5 Library](https://portal.hdfgroup.org/pages/viewpage.action?pageId=50073884)
- [VTK](https://vtk.org/), tested with versions:
  - 9.0.1
  - 9.0.0
  - 8.2.0
  - 8.1.2
  - 8.1.1
  - 8.0.1
  - 8.0.0
  - 7.1.0

## Building and running

### Using CMake (best option)

```bash
mkdir build && cd build
cmake ../
# or cmake -DVTK_DIR:PATH=/path/to/vtk ../ -Wno-dev
```

### Using dev Makefile

```bash
make run
```