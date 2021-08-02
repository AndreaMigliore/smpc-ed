<p align="center">
    <a href="https://github.com/AndreaMigliore/smpc-ed">
        <img src="img/genome.png" alt="SMPC-ED" title="SMPC-ED" height="80"/>
    </a>
</p>

# Secure Multi-Party Computation - Edit Distance
This project implements several algorithms for the **secure computation** of [Edit Distance](https://en.wikipedia.org/wiki/Levenshtein_distance) using the [semi-honest two-party computation](https://en.wikipedia.org/wiki/Secure_two-party_computation) based on [garbled circuits](https://en.wikipedia.org/wiki/Garbled_circuit) model provided by the [EMP-toolkit](https://github.com/emp-toolkit). 

There are six different algorithms implemented in plain C++.
- **Wagner-Fischer (Matrix)**: ["The string-to-string correction problem"](http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.367.5281&rep=rep1&type=pdf). 1974.
- **Wagner-Fischer (Optimized)**: ["The string-to-string correction problem"](http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.367.5281&rep=rep1&type=pdf). 1974.
- **Ukkonen (Threshold)**: ["Algorithms for approximate string matching"](https://www.cs.helsinki.fi/u/ukkonen/InfCont85.PDF). 1985.
- **Ukkonen (Generalized)**: ["Algorithms for approximate string matching"](https://www.cs.helsinki.fi/u/ukkonen/InfCont85.PDF). 1985.
- **Wu et al.**: ["An O(NP) sequence comparison algorithm"](https://publications.mpi-cbg.de/Wu_1990_6334.pdf). 1990.
- **Enhanced Ukkonen**: ["An Extension of Ukkonen's Enhanced Dynamic Programming ASM Algorithm"](http://berghel.net/publications/asm/asm.pdf). 1996.
- **Myers' Bit-Vector**: ["A Fast Bit-Vector Algorithm for Approximate String Matching Based on Dynamic Programming"](http://www.gersteinlab.org/courses/452/09-spring/pdf/Myers.pdf). 1999.

The first four of these algorithms are also implemented with the EMP-toolkit. For the others a direct implementation is not convenient due to unefficient management of secret indexes in secure computation.

# Getting started
## C++ (`Standard` directory)
After compiling, just run:
```sh
./EditDistance <input_length> <threshold>
```
## EMP (`Secure` directory)
1. Install the [EMP-toolkit](https://github.com/emp-toolkit)
2. Run `./bin/EditDistance 1 12345 <input_length> <threshold> & ./bin/EditDistance 2 12345 <input_length> <threshold>` 
## Parameters
The `<input_length>` (must be the same for the programs in `Secure` directory) and the `<threshold>` are optional.

Possible `<input_length>` are: 5, 7, 50, 100, 200, 250, 500, 1000, 2000, 3000, 3500, 4000, 5000, 10000, 50000. These are randomly generated DNA (A, C, G, T) strings.

It is also possible to use `IDash_1`, `IDash_2`, and `IDash_3`, which are real DNA sequences of approximately 3400 characters from a genome database released by [iDASH Security and Privacy Workshop 2016](http://www.humangenomeprivacy.org/2016/index.html)

Further details about EMP options are [here](https://github.com/emp-toolkit)

# License
Distributed under the GPL 3.0 License. See `LICENSE` for more information.
