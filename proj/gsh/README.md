# Introduction
The goal of this project is to write a basic shell to support the Linux kernel.

# Goals
This is entirely an educational premise, but I should some parameters:

- Replace `bash` on a basic Linux system and allow a user to login to the
  system. This is going to collide with `systemd` and I'm absolutely here for
  that.
- Eventually use `readline` to support history, command editing, etc.
- Support pipes, redirects, and other basic functions that are described in the
  OSTEP projects repository
- Create some of my own built in commands
- Support interactive as well as batch mode (e.g., shell scripts)
- I have zero interest in adding shell control flow, data structures, etc.

