# Description

This test illustrate how to use dlopen to wrap libraries from vendor in order
to let caller of this library to have no idea of these vendor libraries.

      -l link                 dlopen                 -l link
test ---------> libtriadd.so --------> libvendor.so ---------> liblowlevel.so
