# triangulator
A library of routines and a GUI for studying simplicial complexes in physics and mathematics

Triangulator is a project that aims to develop a library of routines (mostly C++) that can be used by 3rd party applications aimed at numerical studies involving triangulations of manifolds (of various dimension and topology) and simplicial complex structures in general, in physics and math research. The target audience are mostly researchers in quantum gravity and algebraic topology, interested in numerical work with these structures.

The main intended functionality of the library is the following:

 - instantiate simplicial complex structures in memory
 - manipulate the combinatorial (maybe also topological) structure of the complex
 - colorize (i.e., attach various labels to) the simplices in the complex
 - provide support for studying geometry, Regge calculus, and similar
 - provide support for evaluating state sums (path integrals) and topological invariants
 - provide support for embedding a complex into Euclidean, Minkowski and spaces of various other signatures
 - provide a GUI for drawing a complex on the screen and for interactive manipulation
 - provide support for integration with various symbolic math software (such as Mathematica, Matlab, Reduce, etc...)
 - provide support for massive parallelization, multi-core and multi-worknode environments
 - provide comprehensive documentation of all of the above...

Of course, so far this is just a wishlist, but we have a dedicated team and some financial support to make it happen. The library is GPL, any interested contributors are welcome to join with suggestions, patches, etc.

The development of this code is supported by the Science Fund of the Republic of Serbia, No. 7745968 "Quantum Gravity from Higher Gauge Theory" (QGHG-2021). The owner of all intellectual property is Institute of Physics Belgrade.

For any additional details, contact the project leader (Marko Vojinovic).


===================
Build instructions:
===================

(1) git pull

(2) Familiarize yourself with the top-level CMakeLists.txt file, edit if needed to turn on or off various components of the build.

(3) mkdir my_build_dir

(4) cmake -B my_build_dir

(5) cmake --build my_build_dir

That's it. The appropriate binaries can be found in their respective directories inside my_build_dir.


