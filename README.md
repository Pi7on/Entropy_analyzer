# Entropy Analyzer

This program calculates the entropy of a file based on a sliding window of specified byte count.

In the context of information theory, entropy quantifies the amount of randomness in a set of data.
In this case, we use Shannon's entropy formula:
```math
H(X) = - \sum_{i=1}^{n} p_i \cdot \log_2(p_i)
```
Given a specified byte count (or "window"), the program calculates the entropy for sequences of bytes in the file, to be given back in normalized form (where 1 is considered as perfectly random):
- `byte_count = 1` Byte-wise entropy
- `byte_count = 2`  2-byte sequence entropy
- ... and so on

## Compilation
```bash
$ gcc -Wall -Wextra -O3 enan.c -o enan -lm
```

# Usage
- TODO (command line flags WIP)
