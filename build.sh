#!/bin/bash
(
   cd target
   cmake ../
   make -j4
   ./genetic -e -m 0.1 -p 30 -g 10
   ./test_genetic
)
