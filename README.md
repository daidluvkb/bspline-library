# bspline-library
## Features
* A C++ implement for evaluating bspline cureves and surfaces. 
* Simple and neat naming style makes the source code easy to read. 
* Display part is based on GL interfaces, but algorithm implement is decoupled with the display module, so it's easy to dissociate the bspline part. 
* Also for easy-to-use interfaces, factory mode is applicated in the entrance of the lib.
## Dependencies
* newmat11 (http://www.robertnz.net/nm11.htm)
## Compile&Build
### Windows
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
### Linux
  * A makefile under the 'bspline' directory could help you compile the source code into .a file immediately.
## How to use the UI/ insert control points in display Interface?
 1. Execuate bsplineui.exe, choose '2D Curve'tab, click 'LoadScene' button, then load the scene window.  
 ![image](https://raw.githubusercontent.com/daidluvkb/bspline-library/main/image/intro1.png)
 2. Set the curve features in the setting area, click 'NewCurve' button, an item named by curve type would show in the right list. Scroll bar 'Curve res' represent the curve resolution.   
 ![image](https://raw.githubusercontent.com/daidluvkb/bspline-library/main/image/intro2.png)
 3. press at any place in left-top-white area to check the selected curve's basic functions.   
 ![image](https://raw.githubusercontent.com/daidluvkb/bspline-library/main/image/intro3.png)
