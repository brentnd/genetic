#!/bin/bash -ex
(
   mkdir -p target
   cd target
   cmake ../
   make -j4
   ctest -V
)
