# Project "LLVM Pass for code morphing"

This project is an example of an LLVM Pass implementation that performs code morphing by adding the number 42 to the result of each instruction.

## How to use

1. Make sure you have LLVM and clang installed.
2. Download the file `LLVM_MorphingPass.cpp ` from this repository.
3. Compile LLVM Pass and run it on your LLVM IR code:
  `clang++ -g -O3 -shared -o CLLVM_MorphingPass.so -I /path/к/llvm/include -fPIC CODAI_LLVM_MorphingPass.cpp
opt -load ./LLVM_MorphingPass.so -morphing-pass -S input.ll -o output.ll`

## How the project works

File `LLVM_MorphingPass.cpp ` contains an LVM Pass implementation for code morphing. Each instruction in the LLVM IR code is replaced by a new instruction, the result of which is the sum of the results of the original instruction and the number 42.

## License

This project is licensed under the [MIT License] (LICENSE).
