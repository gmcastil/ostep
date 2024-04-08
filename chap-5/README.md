# Interlude: Process API

## Homework (sim)

### Questions

## Homework (code)

### Questions

1. Write a program that calls fork(). Before calling fork(), have the
main process access a variable (e.g., x) and set its value to some-
thing (e.g., 100). What value is the variable in the child process?
What happens to the variable when both the child and parent change
the value of x?

The child process receives the same value as was set in the parent process.
Each process is capable of changing their process-specific version of the
variable.

2. Write a program that opens a file (with the open() system call)
and then calls fork() to create a new process. Can both the child
and parent access the file descriptor returned by open()? What
happens when they are writing to the file concurrently, i.e., at the
same time?

So, first of all, both processes are able to communicate to the open file
descriptor. And accesses to the open file were both successful, but not in a
deterministic order.  It appears that the scheduler lets the parent and child
sort of share the resource, but not in a predictable way. The file pointer is
just left where it's at and it appears to be up to the user to keep track of
everything.  There's some strange behavior with closing which I do not quite
understand.

3. Write another program using fork(). The child process should
print “hello”; the parent process should print “goodbye”. You should
try to ensure that the child process always prints first; can you do
this without calling wait() in the parent?

4. Write a program that calls fork() and then calls some form of
exec() to run the program /bin/ls. See if you can try all of the
variants of exec(), including (on Linux) execl(), execle(),
execlp(), execv(), execvp(), and execvpe(). Why do
you think there are so many variants of the same basic call?

There are a variety of ways to invoke other programs and allow a caller or user
to have varying degrees of influence over the environment or even which program
actually gets called.  For instance, a caller might wish to invoke the `vsim`
executable found in the `PATH` but choose to not inherit the rest of the caller's
environment and provide its own.  Another detail is being able to start a
program, such as `bash` but allow it to call itself `sh` instead, to masquerade
as the Bourne shell.

There are a lot of other reasons, some a bit nefarious, where a parent might
wish to invoke some member of the `exec()` family of functions and have a higher
degree of control over which program is actually run, what its arguments are,
and what its environment is. See the manpages for more details. Also, note that
the `execvpe()` function is a GNU thing and requires the `#define _GNU_SOURCE`
feature test macro.

5. Now write a program that uses wait() to wait for the child process
to finish in the parent. What does wait() return? What happens if
you use wait() in the child?

