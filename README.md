# AtilaCalculatorSoftware v2.2.0

C++ Interface Software for Atila fem resources files

Documentation available [here](https://xisabla.github.io/AtilaCalculatorSoftware/doc/)

## Dependencies

- [Boost](https://www.boost.org/) 1.72
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

```shell script
mkdir build && cd build
cmake ../ # or cmake -DVTK_DIR:PATH=/path/to/vtk ../ -Wno-dev
make
./AtilaCalculatorSoftware # or ./AtilaCalculatorSoftware /path/to/data/directory
```

or using make

```shell script
make run
```

## Progression/To Do

See the [project management](https://github.com/Xisabla/AtilaCalculatorSoftware/projects) tab

## Contributing

To apply an enhancement idea, open [an issue](https://github.com/Xisabla/AtilaCalculatorSoftware/issues). Describe the wanted feature as much as possible. 

To add a new feature/enhancement:
- [Open an issue](https://github.com/Xisabla/AtilaCalculatorSoftware/issues) as described above
- Add to your issue a detailed description (needed methods, objects, behavior, ...) (a marked list is always a good idea)
- Add your enhancement issue to the [`Features` project](https://github.com/Xisabla/AtilaCalculatorSoftware/projects/2) as a "To Do" item
- Open a new branch `feature/<feature-name>`
- Once the development part is over, perform a [pull request](https://github.com/Xisabla/AtilaCalculatorSoftware/pulls) from your feature developing branch and do not forget to update the project [version](https://github.com/Xisabla/AtilaCalculatorSoftware/blob/master/include/version.h)

## License

Project under [MIT License](./LICENSE.md)
