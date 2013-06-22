Housemarque Pre-Interview Coding Test
-------------------------------------

  The point of the exercise is to demonstrate the candidates ability to
  implement a semi-complex algorithm in a clean and efficient way in C++.

The Task
--------

  Implement a raytracer with KD-tree space partitioning.  The program
  should take a model as an input and produce a .tga file as an output.
  (Optional: if you want, another equally complex space partitioning
  scheme can be implemented, just ask us beforehand.)

The Tools
---------

  The provided .zip package contains a few helper files:
  - tga_saver.h  / .cpp - Provides a simple TGA file format save utility.
  - ply_reader.h / .cpp - Provides a simple PLY model reader.

  Feel free to modify the included source code in whatever way you want
  to.  All source code included is Public Domain.  You can use the
  resulting code as a work sample to other companies too if you should
  want to.

  In addition you should download the Happy Buddha 3D model from the
  Stanford 3D Scanning Repository
  (http://graphics.stanford.edu/data/3Dscanrep/).

  ftp://graphics.stanford.edu/pub/3Dscanrep/happy/happy_recon.tar.gz

  Note: ply_reader.cpp is not a general PLY reader, it works only on some
  of the Stanford Models, including Happy Buddha and Dragon.

  Requirements:
  - Provide us with a source package compilable on GCC or Visual Studio
    without any extra libraries (provide a makefile / project file).
    Note: include a usage, we supply the Happy Buddha models ourselves.
  - The implementation should be able to render the highest resolution
    Happy Buddha model in "reasonable" time.
  - Include a readme.txt describing the program from a programmer's
    perspective and the challenges you faced implementing it (1 to 2
    pages).

Suggestions
-----------

  What we are looking for in the final code:
  - Simplicity and cleanness of code, no copy paste.
  - Knowledge of C++ features (and their performance implications).
  - Proper memory management.
  - No fancy shading algorithms needed.  Just produce a picture that
    correctly visualizes the model out of it.

  Estimated amount of work required is about 2-3 working days.  If it
  starts taking much more than that, you are probably doing something way
  too complicated for the purpose of the test.

  Suggested implementation steps:
    1) Build a 3d vector class.
    2) Build your raytracer with the lowest resolution model and brute force
       search into the model data.
    3) Have a unit test to compare your KD-Tree against the brute force
       search, (the results should of course be equal).
  3.5) Add a comment in the main file where you list your favorite color,
       food, and animal.
    4) Build your KD-Tree, check the performance benefits.
    5) _Simplify_ and optimize.

  Note: happy buddha model has pretty small triangles, watch for floating
  point inaccuracies.  Effective treatment of floating point inaccuracies
  does not necessarily involve simply using higher precision floating
  point numbers.

  For reference, our sample implementation (contains no optimizations nor
  multithreading) is about 900 lines of code (without unit tests) and
  renders 640x480 picture of the highest resolution happy buddha in about
  40 seconds (on Intel Core2 Q9550 @ 2.83ghz - sample render included in
  the zip file).

Useful References
-----------------

  http://www.realtimerendering.com/intersections.html
  http://en.wikipedia.org/wiki/Kd-tree
  (Book: http://realtimecollisiondetection.net/books/rtcd/)

  Note: If you use code/methods from somewhere, make sure you mark the
  references/urls of the original in the code comments.
