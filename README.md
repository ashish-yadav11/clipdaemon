# clipdaemon

clipdaemon is the daemon part of
[ClipIt](https://github.com/CristianHenzel/ClipIt)
responsible for clipboard persistence, extracted out to be a standalone program.

# Installation

Clone the repository and run
```
cd clipdaemon
make install clean
```
Make sure that `$HOME/.local/bin` is in your PATH environment variable, as it is
the default installation destination (read the [Makefile](./Makefile)).
