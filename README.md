# clipdaemon

A simple GTK+ based daemon, forked from
[ClipIt](https://github.com/CristianHenzel/ClipIt),
for clipboard persistence.

# Installation

Clone the repository and run
```
cd clipdaemon
make install clean
```
Make sure that `$HOME/.local/bin` is in the PATH environment variable, as it is
the default installation destination ([Makefile](./Makefile)).
