#!/bin/bash
(
   cd target
   cmake ../
   make -j4
   ./test_genetic
)