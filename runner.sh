#!/bin/bash

if [ "$1" = "node" ]
then
    node-gyp rebuild
    node ./index.js
    exit 1
fi

export ASTX_ENABLE_SASS=true
export ASTX_ENABLE_SSH2=true
export ASTX_ENABLE_MINGUI=true

if [ "$1" = "make" ]
then
    make -f Makefile
    exit 1
fi


for entry in $(find ./src/astox/* -name "*cpp"  -cmin -4)
do
  echo "---$entry"
  name=$(basename "$entry")
  name  ${name%%.*}
  make -f Makefile ${name%%.*}
done

 make -f Makefile astox
 make -f Makefile astxUnitTests
 
if [ "$1" = "test" ]
then
    ./bin/astxUnitTests
fi


