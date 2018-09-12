#!/bin/bash

cryptopp-remove 
sudo rm -f /opt/local/lib/libcryptopp.a /opt/local/lib/libcryptopp.dylib || true
make distclean && make all test && sudo -EH make install PREFIX=/opt/local && sudo mv /opt/local/lib/libcryptopp.a /opt/local/lib/libcryptopp-clang.a && sudo rm -f /opt/local/lib/libcryptopp.dylib && make distclean && CXX=g++ make all test && sudo cp libcryptopp.a /opt/local/lib/libcryptopp-gcc.a && sudo ln -s /opt/local/lib/libcryptopp-clang.a /opt/local/lib/libcryptopp.a
