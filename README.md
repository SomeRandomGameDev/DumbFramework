### Purpose
There's many good libraries for speeding up your productivity. But sometimes, speeding up is not enough. You're always running out of time. One can always get libraries like GLFW, GLEW, SOIL. But you need to do basic stuff over and over again. Here comes the dumb framework. It implements the glue, a slightly higher level of abstraction that combines underneath libraries without cutting you from them.

We're talking about ultra-lightweight productions here. It's not about getting a middleware, pushing your assets and publishing a fat prototype. It's about coding a couple of routines, launch your favorite compilers and obtaining a shiny tiny executable.

### So, how can I use your stuff ?
Assuming you're comfortable with C++, here is the thing: There's a class, named Application, that represents the core of your prod. Its purpose is to display one scene at a time (perhaps your app is about a single scene, why not ?). A scene is just an object that can collect information from input devices (keyboard, mouse ...) and output a result (on screen, with sound).
One only needs to create a scene class by deriving the Scene interface.

```cpp
class MyScene : public Scene {
   // ... See the Scene class code, it pretty straightforward.
};
```

Then, you just have to use it like that.

```cpp
int main(void) {
  MyScene myScene;
  Application myApplication;      
  WindowHint hint;
  // ... Fill hint with starting preferences (window size, fullscreen ?).
  myApplication.start(hint, &myScene);

  return 0;
}
```

Complete examples are available in the git repository, under the 'demo' directory.
### Or what about making things simplier ?
Not concerned about scene management and all that stuff ?
Just use the Wrapper.
What you've got to do is just to create a class that implement the following methods:
```cpp
GLFWwindow *createWindow();
void destroyWindow(GLFWwindow *);
void init();
void render();
void handleKeyAction(int key, int scancode, int action, int mods);
void handleMouseButtonAction(int button, int action, int mods);
void handleMousePositionAction(double x, double y);
void handleMouseWheelAction(double x, double y);
void handleWindowSize(int x, int y);
void handleChar(unsigned int unicodeChar);
void handleCursorEnter(int flag);
```
And that's about it. Then, merges it with the wrapper using the magic of the templates like this:
```cpp
// Include GLFW/GLEW stuff
#include <wrapper.hpp>
#include "the_place_where_you_declared_your_class.hpp"

// Let say that your class is named 'MySimpleApplication'.


int main(void) {
  Wrapper<MySimpleApplication> *wrapper;
  MySimpleApplication *app = new MySimpleApplication(/* your own flavour of constructor */);
  // Do whatever you want to prepare your stuff. Then.

  wrapper = new Wrapper<MySimpleApplication>(app);

  wrapper->start();

  return 0;
}
```

And voilà !

### What if I want to play with sprites ?
The way to flat happiness is short with this dumb framework.
// For now, there's a tool under construction that will guide you through this realm. Stay tuned.

### How does it compile ?

#### Under Linux

Make sure to have **cmake** and proceed like this:
```
$ cd DumbFrameworkRoot
$ mkdir build
$ cd build
$ cmake ..
$ make
``` 

This will produce tons of stuff including the **libDumbFramework.a** static library file. Feel free to modify the CMakeLists.txt file in the root directory to fit your need (and/or to tune it with your favorite options. In my case, I'm very fond of clang++ hence the
```
cmake -DCMAKE_CXX_COMPILER=/usr/bin/clang++ ..
```
).

*SomeRandomGameDev* and *BlockoS*
