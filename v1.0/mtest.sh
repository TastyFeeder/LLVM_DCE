clang -c -emit-llvm -o test.bc test.c 
opt -load ./hello.so -hello test.bc -o test_pass_by_hello.bc

