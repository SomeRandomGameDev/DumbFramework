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

