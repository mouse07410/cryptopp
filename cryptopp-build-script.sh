#!/bin/bash

cryptopp-remove 

# Build Clang version
sudo rm -f /opt/local/lib/libcryptopp.a /opt/local/lib/libcryptopp.dylib || true
make distclean && make -j4 all test 2>&1 | tee make-clang-out.txt && sudo -EH make install PREFIX=/opt/local && sudo mv /opt/local/lib/libcryptopp.a /opt/local/lib/libcryptopp-clang.a 


# Delete .dylib to avoid potential interference between libraries built by Clang++ and G++
sudo rm -f /opt/local/lib/libcryptopp.dylib && 

# Build GCC version
#make distclean && CXX=g++ make -j4 all test 2>&1 | tee make-gcc-out.txt && sudo cp libcryptopp.a /opt/local/lib/libcryptopp-gcc.a 

# Make Clang version system default
sudo ln -s /opt/local/lib/libcryptopp-clang.a /opt/local/lib/libcryptopp.a
