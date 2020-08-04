# Encryption
My exploration in encryption.

## RSA
[RSA](/RSA) contains my 64-bit(scalable) RSA encryption implemented in C++ 

* Why Permuted Congruential Generator(PCG) as Pseudo Random Number Generator(PRNG)?
Although Mersenne Twister is a huge step ahead from srand/rand, it is not ideal in any view. [This](https://www.pcg-random.org/other-rngs.html) and [this](https://arxiv.org/pdf/1910.06437.pdf) is evidence. Xorshift is a considerable option but [since it is predicatable and has a smaller period](https://arvid.io/2018/07/02/better-cxx-prng/#fnref3), PCG is a very small price to pay in speed with great returns in quality.  
