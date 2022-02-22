#! /bin/bash

# Compile all component object files for the triangulator-library
# (add additional .cpp files here as needed)

echo " "
echo "Compiling component .cpp files..."

g++ -c classes.cpp color.cpp input-and-output.cpp seed.cpp

echo "... done."
echo " "

# Create triangulator-library as a static library from object files
# (add additional .o files here as needed)

echo "Creating triangulator-library.a from component object files..."

ar rcs triangulator-library.a classes.o color.o input-and-output.o seed.o

echo "... done. "
echo " "

# Delete component object files since they were temporary
# (can be ommited if we need them)

echo "Cleaning up temporary object files..."

rm classes.o color.o input-and-output.o seed.o

echo "... done. "
echo " "

# Compile main.cpp
#
# Order of arguments and options is important:
#
# -L .                          specifies the current directory for the linker
# -l :triangulator-library.a    specifies the exact filename of the library for the linker
# -o execute-me                 specifies the name of the resulting executable file

echo "Compiling main.cpp and linking it to triangulator-library.a..."

g++ main.cpp -L . -l :triangulator-library.a -o execute-me

echo "... done."
echo " "
echo "If no errors were reported, the file execute-me can now be executed."
echo " "

