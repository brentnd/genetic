#!/bin/bash
(
   mkdir target
   cd target
   cmake ../
   make -j4
   ctest -V
)
