export CCFLAGS="-fprofile-arcs -ftest-coverage -fprofile-generate"
export CXXFLAGS="-fprofile-arcs -ftest-coverage -fprofile-generate"
export CPPFLAGS="-fprofile-arcs -ftest-coverage -fprofile-generate"
./configure --disable-optimization
make check
export CCFLAGS=
export CXXFLAGS=
export CPPFLAGS=
