#!/bin/bash -x
#
# (C) 2017 Mouse
#
# Script to be invoked by "git bisect" to automate the bisect
# process of finding the commit that breaks the code.
#
# Script Usage: ./test-error.sh 
#
# Intended Usage: 
#      $ git bisect start HEAD <last_known_good_commit>
#      $ git bisect run ./test-error.sh
#



# Install directory
INSTDIR="/tmp/cryptopp"

# Clean the old stuff
rm -rf ${INSTDIR}
# Make sure the directory exists
mkdir -p ${INSTDIR}

# No need to touch the local repo, as "git bisect" takes care of that

# Clean up everything
make distclean || true

# Does it even compile with Clang?
make all
if [ $? != 0 ]; then
	echo "Failed to compile with ${CXX}!"
	exit 1
fi

# Test what we built
make test
if [ $? != 0 ]; then
	echo "Failed test (compiled with ${CXX})!"
	exit 1
fi

# Clean up everything
make distclean

# Now - does it work with Macports-installed GCC?
CXX=g++ make all
if [ $? != 0 ]; then
	echo "Failed to compile with g++!"
	exit 1
fi

# Now - the actual test-run
CXX=g++ make test
if [ $? != 0 ]; then
	echo "Failed test (compiled with g++)!"
	exit 1
fi

# If we are here, everything above completed successfully
# So we can declare the build of this commit a success
echo "Everything OK"
exit 0
