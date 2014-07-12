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
The way to flat happiness is short with this dumb framework. But it requires a little descriptive effort. What you need is a square picture, about power of two (16x16, 32x32 ... but usually 512x512 or even 1024x1024) and an XML file that describe what's in it in term of sprite and animation of sprite. Here's an example:
```xml
<!-- First the path to the picture. Note that is can be
absolute OR relative - from where you've launched your
program. Then come the number of sprite definition. -->
<atlas path="testsprite.png" size="1">

<!-- Here comes the list of definitions. From here,
you can define any sprite you want. Keep in mind that
everything is about numerical identifier by now.
Afterall, this framework is really dumb. -->

<!-- So, here is the first definition. Its identifier
is obviously 'O'. The 'size' attribute says that it
hosts only one animation. -->
<definition id="0" size="1">
  <!-- Then comes the list of animations. The first
  one got the identifier '0' (what a surprise ...).
  We also precise that the animation will consist in
  five frames (and don't lie about this - the framework
  is very itchy when it comes to lies ... -->
  <animation id="0" size="5">
    <!-- Time to list the frames. No identifier here, only
    'time'. It designate the time of appearance in milliseconds.
    As always, declare the frame in order of appearance. The
    Dumb Framework being dumb, it won't make the job for you. -->

    <!-- offsetX and offsetY is a displacement in pixel given the
    declared position of the sprite (you'll see that it's useful
    for stuff like jump animation and so on).
    width and height are dimension, in pixels.
    topU, topV, bottomU, bottomV are texture coordinate in pixel
    within the big picture you're pointing at. They are coordinate
    of the upper-left and bottom-right corners. -->
    <frame time="0"
           offsetX="0" offsetY="0"
           width="30" height="50"
           topU="13" topV="5"
           bottomU="43" bottomV="55"/>
    <frame time="200"
           offsetX="0" offsetY="0"
           width="30" height="50"
           topU="55" topV="5"
           bottomU="85" bottomV="55"/>
    <frame time="400"
           offsetX="0" offsetY="0"
           width="30" height="50"
           topU="90" topV="5"
           bottomU="120" bottomV="55"/>
    <frame time="600"
           offsetX="0" offsetY="0"
           width="30" height="50"
           topU="125" topV="5"
           bottomU="155" bottomV="55"/>
    <frame time="800"
           offsetX="0" offsetY="0"
           width="30" height="50"
           topU="13" topV="5"
           bottomU="43" bottomV="55"/>
    <!-- ... -->
  </animation>
  <!-- ... -->
</definition>

</atlas>
```

Now, how to use that ?
First, you'll have to include header files sprite.hpp and sprengine.hpp. Then, in you shiny prototype, declare an atlas (you know, this big picture with all your sprite in it), like this:
```cpp
Sprite::Atlas *atlas = new Sprite::Atlas("/path/to/the/xml");
```
From this object, you'll have access to all the sprite definitions/animations/frame you've declared earlier.
Then, you'll need an 'engine' to display/move all this.
```cpp
Sprite::Engine *engine = new Sprite::Engine(atlas, 150000);
```
The above line declares a sprite engine that will use you previously declared atlas and will be able to displayed 150000 sprites.
When setting your scene, be sure to set the viewing window of the sprite engine (it is very important : it can't guess it by itself). For example, given that you have a rendering window with a physical size of 600 per 400 pixels, and you want your sprites rendered at a 1:1 ratio, initialise the sprite engine rendering service like this.
```cpp
engine->viewport(0, 0, 600, 400);
```
The "view" will be centered on coordinate (0, 0). Upper left corner will have coordinate (-300, -200) and bottom right (300, 200).

Now, it's time to play with the sprites. Remember that we've defined a sprite scheme with identifier '0'. Let's create one instance of it.
```cpp
// Create an instance of sprite from definition 'O', initialy
// place at coordinates (100, 100) and starting with animation '0'.
Sprite::Identifier identifier = engine->create(0, glm::vec2(100, 100), 0);
```
By default, the engine cycle the animations. Here the sprite will be displayed at (100, 100) in the engine coordinate system (in the window context described earlier, it will be (400, 300) ).

Finally, to render the sprite, just invoke the following:
```cpp
engine->render(); // Magic !
```

And that's it !

Of course, there's some other methods to actually move the sprites, rotate them, perform zooming and so on. (and this will be documented later even if it's totally done in the code).

Have fun with that.

*SomeRandomGameDev* and *BlockoS*
