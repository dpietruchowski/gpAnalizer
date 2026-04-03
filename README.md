# gpAnalizer

A desktop application that uses **Genetic Programming (GP)** to automatically discover optimal image processing pipelines. The system evolves a population of tree-structured programs that transform input images towards a reference image — without manual parameter tuning.

## Overview

gpAnalizer applies evolutionary algorithms to the image processing domain. Each individual in the population is a syntax tree whose nodes represent image operations (bitwise functions, morphological transforms, thresholding). Over successive generations the population is evaluated, selected and recombined until the best program produces output closest to the target image.

```
Input image  ──► [ Genetic Program (tree of image ops) ] ──► Output image
                                 ▲
Reference image ────────────────► Fitness evaluation
```

## Features

- **Three node types** – bitwise function nodes, morphological operation nodes, threshold nodes
- **Multiple fitness metrics** – Hamming distance, Hausdorff distance, Hausdorff-Canny, Hausdorff-Modified
- **Configurable genetic operators** – subtree mutation, node mutation, hoist mutation, collapse mutation, subtree crossover, copy
- **Two selection strategies** – fitness-proportionate roulette, tournament selection
- **XML serialisation** – save and reload evolved programs
- **Qt GUI** – run up to 10 independent GP instances simultaneously in tabs
- **Background execution** – evolution runs in a separate QThread; GUI stays responsive

## Requirements

| Dependency | Version |
|------------|---------|
| Qt         | 5.x     |
| OpenCV     | 4.x     |
| TinyXML2   | any     |
| Boost      | ≥ 1.58  |
| C++ compiler | C++11 |

### Install dependencies on Ubuntu / Debian

```bash
sudo apt-get install \
    qt5-qmake qtbase5-dev \
    libopencv-dev \
    libtinyxml2-dev \
    libboost-system-dev
```

## Build

```bash
# Generate Makefile
qmake gpAnalizer.pro

# Compile
make -j$(nproc)

# The binary is placed in the current build directory
./gpAnalizer
```

You can also open `gpAnalizer.pro` directly in **Qt Creator** and use *Build → Build All*.

## Usage

### GUI mode

1. Launch `./gpAnalizer`.
2. Open a tab for each GP instance you want to run (up to 10).
3. Click **Settings** to configure:
   - Population size, maximum tree depth, tournament size
   - Selection type (roulette / tournament)
   - Fitness metric (Hamming / Hausdorff / Hausdorff-Canny)
   - Genetic operator and node-type probabilities
   - Stop criteria (number of generations, target fitness score)
4. Choose an **input image** and a **reference image**.
5. Select an **output directory** where results will be saved.
6. Click **Start**.

Results are written to the output directory after every generation:
- `generation<N>_<score>.png` – best program output image
- `program<N>_<score>.xml`  – serialised best program (reloadable)
- `stats<N>.m`              – per-generation statistics (Octave/MATLAB format)

### Directory structure of results

```
<output_dir>/
├── population/          # intermediate population images (per generation)
├── generation0_1234.png
├── program0_1234.xml
├── stats0.m
├── generation1_987.png
...
```

## Project structure

```
gpAnalizer/
├── application/   # GP engine – Application, Population, Tree, Stats
├── node/          # AST node types – FunctionNode, MorphoNode, ThreshNode, TerminalNode
├── operation/     # Genetic operators – mutations, crossovers, copy
├── fitness/       # Fitness metrics – Hamming, Hausdorff variants
├── selection/     # Selection strategies – Roulette, Tournament
├── generator/     # Factories – NodeGenerator, GeneticOperationGenerator
├── parser/        # XML deserialisation
├── ui/            # Qt widgets – MainWindow, ApplicationTab, Settings
├── exceptions.h   # Typed exception hierarchy
└── main.cpp       # Entry point
```

## Extending the function set

New image processing functions can be added by:

1. Adding a static method to `FunctionSet` (in `node/functionset.h/.cpp`).
2. Registering it in `FunctionSet::FunctionSet()` constructor.
3. Calling `FunctionNode::getFunctionSet().addFunction("name")` in `Application::Application()`.

## Exception hierarchy

All runtime errors derive from `GpException` (see `exceptions.h`):

| Class | When thrown |
|-------|-------------|
| `InvalidArgumentException` | wrong number of image arguments, invalid parameters |
| `InvalidEnumException`     | unknown enum string during deserialisation |
| `ParseException`           | malformed XML program file |
| `ImageException`           | empty / unreadable image |
| `ConfigurationException`   | probabilities don't sum to 1, missing output directory |

## Building and running unit tests

The tests use [Google Test](https://github.com/google/googletest) and CMake. They cover node types and fitness functions (no Qt dependency).

**Install prerequisites (Ubuntu/Debian):**
```bash
sudo apt-get install cmake libgtest-dev libboost-dev libtinyxml2-dev libopencv-dev
```

**Build and run:**
```bash
mkdir build_tests && cd build_tests
cmake ../tests -DOpenCV_DIR=/usr/lib/x86_64-linux-gnu/cmake/opencv4
make -j$(nproc)
./gpAnalizer_tests         # run all tests
ctest --output-on-failure  # or via CTest
```



This project is provided as-is for research and educational purposes.
