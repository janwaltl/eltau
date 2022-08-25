# ElTau
[![Build](https://github.com/janwaltl/eltau/actions/workflows/build-sca.yml/badge.svg?branch=main)](https://github.com/janwaltl/eltau/actions/workflows/build-sca.yml)
[![codecov](https://codecov.io/gh/janwaltl/eltau/branch/main/graph/badge.svg?token=7N1FJIE0JO)](https://codecov.io/gh/janwaltl/eltau)

Description will be added when minimal working subset works.

# Build

CMake 3.15+ is required to build the project. For conveniece, I use `make` for wrapping CMake's commands.

Builds are located in `build/<type` and use `${CXX}` compiler. Tests and examples are built by default.
I also move `compile_commands.json` to `build/` and have a local symlink to project's root need by my NeoVim setup,
feel free to ignore.

- `make debug` - Ordinary debug build.
- `make release` - Ordinary release build, with debugging symbols.
- `make tsan` - Thread-sanitized release build.
- `make asan` - Address-sanitized release build.
- `make msan` - Memory- ,Leak- ,Undefined-sanitized release build.
- `make test` - Build&run unit tests with release build, output printed to stdout and to `build/release/reports`.
- `make junit` - Build&run unit tests with release build, output in xml format (for CI).
- `make coverage` - Build&run unit tests with debug build, generates HTML coverage report into `build/coverage/coverage/index.html`, the xml into `build/coverage/coverage.xml`, and tests outputs themselves are in `build/coverage/reports`.
- `make ttest` - Run thread-sanitized unit tests.
- `make atest` - Run address-sanitized unit tests.
- `make mtest` - Run memory-sanitized unit tests.
- `make format` - reformat the codebase
- `make sca` - Run clang-tidy on the project.
- `make docs` - Generated doxygen documentation into `docs/html`.
- `make clean` - Delete the build folder.


# Documentation

Doxygen documentation can be found [here](https://janwaltl.github.io/eltau/index.html).

# License

ElTau project is licensed under the MIT License, see LICENSE for more information.

The repository contains folders and files from other authors having a license of their own.

The closest license applies: file > folder > project-wide. If in doubt, ask.

 - `cmake/CodeCoverage.cmake` obtained from [cmake-modules](https://github.com/bilke/cmake-modules/blob/master/CodeCoverage.cmake).
