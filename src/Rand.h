#ifndef _RAND_H
#define _RAND_H

class Rand
{
public:
    explicit Rand(unsigned int s) : seed(s & 0x7fffffffu)
    {
        // Avoid bad seeds.
        if (seed == 0 || seed == 2147483647L)
            seed = 1;
    }

    unsigned int Next() {
        static const unsigned int M = 2147483647L;   // 2^31-1
        static const unsigned long A = 16807;  // bits 14, 8, 7, 5, 2, 1, 0
        // We are computing
        //       seed_ = (seed_ * A) % M,    where M = 2^31-1
        //
        // seed_ must not be zero or M, or else all subsequent computed values
        // will be zero or M respectively.  For all other values, seed_ will end
        // up cycling through every number in [1,M-1]
        unsigned long product = seed * A;

        // Compute (product % M) using the fact that ((x << 31) % M) == x.
        seed = static_cast<unsigned int>((product >> 31) + (product & M));
        // The first reduction may overflow by 1 bit, so we may need to
        // repeat.  mod == M is not possible; using > allows the faster
        // sign-bit-based test.
        if (seed > M) {
            seed -= M;
        }
        return seed;
    }

    // Returns a uniformly distributed value in the range [0..n-1]
    // REQUIRES: n > 0
    unsigned int Uniform(int n) { return (Next() % n); }

    // Randomly returns true ~"1/n" of the time, and false otherwise.
    // REQUIRES: n > 0
    bool OneIn(int n) { return (Next() % n) == 0; }

    // Skewed: pick "base" uniformly from range [0,max_log] and then
    // return "base" random bits.  The effect is to pick a number in the
    // range [0,2^max_log-1] with exponential bias towards smaller numbers.
    unsigned int Skewed(int max_log) {
        return Uniform(1 << Uniform(max_log + 1));
    }
private:
    unsigned int seed;
};

#endif