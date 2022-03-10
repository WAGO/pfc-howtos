# libkbus-register-settings (bin)

Precompiled version of kbus-register-settings.

It is intended as a 1-to-1 replacement of the kbus-register-settings libary,
and it ***cannot be used together with kbus-register-settings in one ptxdist project***.

## Thread Safety

None. *Do not use the methods and structures across multiple threads*.

## Update from Source

`make` in the top-level directory, with a ptxdist project configured to build the source version (both static and dynamic) of kbus-register-settings.
The provided [makefile](./Makefile) automatically builds the kbus-register-settings package and copies the library and headers into the correct locations in this repository.
Usually, the build files do not need to be changed.

It is recommended to run the `clean` task before that (*warning*: The `clean` task ***will delete*** the previously copied library and headers).

