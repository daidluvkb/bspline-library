# bspline-library
## Features
* A C++ implement for evaluating bspline cureves and surfaces. 
* Simple and neat naming style makes the source code easy to read. 
* Display part is based on GL interfaces, but algorithm implement is decoupled with the display module, so it's easy to dissociate the bspline part. 
* Also for easy-to-use interfaces, factory mode is applicated in the entrance of the lib.
## Dependencies
* newmat11 (http://www.robertnz.net/nm11.htm)
## Compile&Build
### windows
  * Compile source code
    * If you are using Visual Studio 2012 or higher version, you can try to open the .sln file directly.  
    The project structure is as follows:
    >bspline.sln
    >>* **bspline**: lib  
    >>* **bsplinedemo**: demo to show how to use or debug lib  
    >>* **bsplineui**: a GUI for use, based on Qt (vs plugin)  
    >>* **newmat**: I compile the newmat lib in my solution, but it can be included only as a lib.   
  * GUI 
    * I used Qt to build my GUI, so I can use the C++ lib directly. And it is a Qt plugin for vs, so you must use a similar one to edit it. Or you can just run the .exe file I uploaded in /executable directory to see how bspline curves work. 
## How to use the UI/ insert control points in display Interface?
