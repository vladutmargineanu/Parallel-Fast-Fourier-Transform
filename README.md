# Parallel-Fast-Fourier-Transform
Homework for the Parallel and Distributed Algorithms course @ ACS, UPB 2019

# How to run:

./parallelFT inputValues.txt outputValues.txt numThreads
./parallelFFT inputValues.txt outputValues.txt numThreads

The input file format:

`N // number of values`
`value1 // values are of type double`
`value2`
`...`
`valueN`
The output file format:

`N // number of complex type values represented as pairs of doubles`
`Re-value1 Img-value1 // values are of type double`
`Re-value2 Img-value2`
`...`
`Re-valueN Img-valueN`
