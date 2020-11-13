# [WIP] AtilaCalculatorSoftware v2

## Dependencies

- [GiDPost](https://www.gidhome.com/gid-plus/tools/476/gidpost/) 2.7
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

### TODO

- [ ] Refactoring: Better structure, code comprehension and documentation/comments
  - [ ] Transform all structures into class
  - [ ] Merge all related files together (to finally have BinaryData, Mesh, Result)
  - [ ] Rebuild global structure of the different current files/class/structs
    - [ ] binary_data
    - [ ] binary_data_class
    - [ ] binary_data_GiD 
    - [x] mesh
    - [x] mesh_GiD
    - [x] result
    - [x] <s>result_GiD</s>
  - [ ] Doxygen doc everywhere
  - [ ] Comment for big spots
  - [ ] Review and refactor (split) all big methods or macro methods
- [ ] Debugging/Logging: Add a way to print important data, logging the different actions
  - [x] Debug macros
  - [ ] Logger object to keep all logs and manage I/O
  - [ ] Logger logs to console with cli arg eg: `./AtilaCalculatorSoftware --verbose`)
  - [ ] Log window bind to the Logger for the user to be able to see logs
  - [ ] Stringify methods for all objects
- [ ] New features
  - [ ] Mesh visualisation
  - [ ] Shortcuts (Ctrl+O to open file, ...)