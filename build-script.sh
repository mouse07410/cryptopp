#!/bin/bash

cryptopp-remove 
make distclean && make -j 4 all && make test && sudo make install PREFIX=/opt/local && make distclean && CXX=g++ make -j 4 all && make test && sudo cp libcryptopp.a /opt/local/lib/libcryptopp-gcc.a
