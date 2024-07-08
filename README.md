# Feature Builder


### Compiling the Project

The feature builder requires SCIP (Version 5.0 or higher) installed or built with CMake.
Create a 'build' directory, navigate there, and configure the build using cmake before compiling:

```bash
$ export CONDA_ENV_NAME=feature-builder-env
$ conda env create -n ${CONDA_ENV_NAME} -f environment.yaml
$ conda activate ${CONDA_ENV_NAME}
$ mkdir build
$ cd build
$ cmake -G "Unix Makefiles" ..  # for Windows
$ cmake ..  # for Unix 
$ make
$ cd ..
```

The specification of a `CMAKE_PREFIX_PATH` is only necessary, if SCIP has been installed in a nonstandard location,
or if a SCIP build directory is specified, which developers sometimes need.

Note that in order to debug later, it is recommended to build SCIP in Debug mode, using `-DCMAKE_BUILD_TYPE=Debug`.


### Project structure

```bash
./  # root
libstdc++-6.dll  # NB: Required for Windows OS!!!
lp_problems/  # empty dir
  - .gitkeep  # Remove
milp_problems/
  - .gitkeep  # Remove
  - problem1.mps
  - problem2.mps
sols/ 
  - .gitkeep  # Remove
  - problem1.sol
  - problem2.sol
output/  # empty dir
  - .gitkeep  # Remove
```

### Run features builder
```bash
$ find . -name ".gitkeep" | xargs rm -f
$ ./run.py
```

The `output` directory will contain results (.csv)
