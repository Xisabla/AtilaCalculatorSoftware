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
    - [ ] mesh_GiD
    - [ ] result
    - [ ] result_GiD
  - [ ] Doxygen doc everywhere
  - [ ] Comment for big spots
  - [ ] Review and refactor (split) all big methods or macro methods
- [ ] Debugging/Logging: Add a way to print important data, logging the different actions
  - [ ] Log macro/method to write information into a log vector, log file or log window (or all of this)
  - [ ] Use the log macro/method in any suitable/useful situation
  - [ ] Intern/friend methods in any class & structure to show it's content in a readable way in the console
  - [ ] Debug mode (from cli, eg: `./AtilaCalculatorSoftware --verbose` or `./AtilaCalculatorSoftware --debug=true`)
  - [ ] Log window for the user to be able to see logs
- [ ] New features
  - [ ] Mesh visualisation