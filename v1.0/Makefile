LLVM_CONFIG=llvm-config

CXX=`$(LLVM_CONFIG) --bindir`/clang
CXXFLAGS=`$(LLVM_CONFIG) --cppflags` -fPIC -fno-rtti
LDFLAGS=`$(LLVM_CONFIG) --ldflags`

all: hello.so LDCE.so

LDCE.so:LDCE.o
	$(CXX) -shared LDCE.o -o LDCE.so $(LDFLAGS) -fPIC

LDCE.o:LLVMDCE.cpp
	$(CXX) -c -std=c++11  LLVMDCE.cpp -o LDCE.o $(CXXFLAGS)

hello.so: hello.o
	$(CXX) -shared hello.o -o hello.so $(LDFLAGS) -fPIC

hello.o: hello.cpp
	$(CXX) -c -std=c++11  hello.cpp -o hello.o $(CXXFLAGS)

clean:
	rm -f *.o hello.so
