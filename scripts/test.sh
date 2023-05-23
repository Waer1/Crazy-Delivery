#!/bin/bash
tests=("shader-test" "mesh-test" "transform-test" "pipeline-test" "texture-test" "sampler-test" "material-test" "entity-test" "renderer-test" "sky-test" "postprocess-test" "test") # array of strings
index=$(($1-1)) # convert number to index
test=${tests[$index]} # get string from array
powershell.exe -ExecutionPolicy Bypass -File "./scripts/run-all.ps1" -t $test
powershell.exe -ExecutionPolicy Bypass -File "./scripts/compare-all.ps1" -t $test
