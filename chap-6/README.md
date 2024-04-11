# Measurement
Our goal here is to measure the cost of a system call and a context switch.
We're going to do this in several ways.  First, we'll study the accuracy of the
`gettimeofday()` function by performing a number of back-to-back calls and
looking at the distribution of results we get. Second, we're going to explore
the `rdtsc` instruction that the x86 ISA makes available and attempt to measure
a system call in that way. Finally, to measure the cost of a context switch,
we'll use the same sort of measurement tactic to do force a context switch the
way that *lmbench* does.

1. Write a program that measures the average time and variation of a system call
   using `gettimeofday()` and `rdtsc` both methods.
2. Write a program that measures the cost of a context switch using pipes.



