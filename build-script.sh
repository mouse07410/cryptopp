#!/bin/bash

cryptopp-remove 
sudo rm -f /opt/local/lib/libcryptopp.a || true
make distclean && make all test && sudo -EH make install PREFIX=/opt/local && make distclean && sudo rm /opt/local/lib/libcryptopp.dylib && sudo mv /opt/local/lib/libcryptopp.a /opt/local/lib/libcryptopp-clang.a && CXX=g++ make all && CXX=g++ make test && sudo cp libcryptopp.a /opt/local/lib/libcryptopp-gcc.a && sudo ln -s /opt/local/lib/libcryptopp-clang.a /opt/local/lib/libcryptopp.a
