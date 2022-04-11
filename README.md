# Introduction
Here is some c++ implementations of DES(Data Encrypt Standard) and it attack.

## DES implementation
Two kinds of implementation are included.
1. A intelligible implementation based on the <bitset.h>.
2. A Faster implementation based on bit operation.

# DES attack
The previous faster implementation was used.

## Exhaustive search
It doesn't use complementary theory. Pure brute-forced.

## Look-up table
In essence, the efficiency of this attack is no different from exhaustion

## Differential attack
A advanced attack which was first published in the 1990s, which is challenging DES-like block cipher.
