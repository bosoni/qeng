QEng (c) mjt, 2019-2020
---
Licensed under MIT license (see LICENSE).

Uses bgfx ( https://github.com/bkaradzic/bgfx ).
so download bgfx, bing and bx ( https://bkaradzic.github.io/bgfx/build.html ).

---

Directory structure:

 bgfx/
 
 bing/
 
 bx/
 
 QEng/  this engine
 
 ---


Tested only on windows 10.
You can use  _CMAKE_GUI_here.bat  to create solution file.



Data/shaders/_SHADERS_/_COMPILE*.bat converts shaders.
Data/models/_CONV*.bat converts .obj models. 



src/Main.cpp  here you can set test which to run, or use test chooser (default).


---
FEATURES:
* scenegraph
  * baseclass: QNode 
    * extended: QScene, QCamera, QLight, QModel3D

* frustum culling

* state manager

* materials:
  * textured unshaded (multilight)		Data/shaders/_SHADERS_/tex_*
  * textured shaded (multilight)		Data/shaders/_SHADERS_/tex_lit*
  * normalmapped shaded (multilight)	Data/shaders/_SHADERS_/bump*

* 3d models:
  * uses model cache
  * hardware instancing


---
NOTES:
- ref counting not used atm
- Node.LookAt() does not work
- Node.RotateZ() does not work right
