### Purpose
There's many good libraries for speeding up your productivity. But sometimes, speeding up is not enough. You're always running out of time. One can always get libraries like GLFW, GLEW, internalisation, font management, texture loading and so on ... But you need to do basic stuff over and over again. Here comes the Dumb Framework. It implements the glue, a slightly higher level of abstraction that combines underneath libraries without cutting you from them.

We're talking about ultra-lightweight productions here. It's not about getting a middleware, pushing your assets and publishing a fat prototype. It's about coding a couple of routines, launch your favorite compilers and obtaining a shiny tiny executable.

### So, how can I use your stuff ?
Assuming you're comfortable with C++, here is the thing: You'll have to create a class that implement a concept compatible with the Dumb Core Application Runner (yeah, that's its name). Then, instanciate a runner and feed it with your fantastic class like this:

```cpp
#include <DumbFramework/runner.hpp>

class MyWrapper {
    DECLARE_WRAPPER_METHODS
    // Do whatever you want ...
    public:
        MyWrapper() {} // You'll need that.
};

// Implement the wrapper methods. See runner.hpp.

SIMPLE_APP(MyWrapper)
```

More details ?

```cpp
#include <DumbFramework/runner.hpp>

class MyWrapper {
    DECLARE_WRAPPER_METHODS
};

// Help create the app. This is called BEFORE the window is created.
void MyWrapper::init(Dumb::Core::Application::Adviser *adviser) {
    Dumb::Core::Application::Video::Monitor monitor = adviser->getPrimaryMonitor();
    adviser->setMonitor(monitor);
    Dumb::Core::Application::Video::Mode mode = monitor.getCurrentMode();
    adviser->setVideoMode(mode);
    adviser->setTitle("Your Favorite Application Title");
    // The application will be fullscreen on the primary monitor.
    // But there's a way to make windowed apps.
}

// This is called AFTER window creation/graphic context.
void MyWrapper::postInit() {
    // Do whatever that require a graphic context.
}

// All glory to the render method !
int MyWrapper::render() {
    // Return 0 if you want to quit the application.
    return GL_TRUE;
}

// This is invoked once the window is closed.
void MyWrapper::close() {
    // [...]
}

// Implements the rest of the wrapping methods (window, mouse and keyboard event handlers).
// [...]

int main() {
    MyWrapper myWrapper;
    Dumb::Core::Application::Runner runner(&myWrapper);
    int result = runner.start();
    return result;
}
```

### How does it compile ?

#### Under Linux

Make sure to have **cmake** and proceed like this:

```sh
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

#### What you'll need ...

In order to compile, the librarie needs the following:

- cmake >= 2.6
- GLM >= 0.9.5.4
- Latest stb toolbox
- GLEW
- GLFW >= 3
- Box2D (in some cases)
- ICU

#### Why the hell do I need THAT ?

There's some neat stuff already implemented, like OpenGL 4 support, a small sprite engine (the Sprengine), a font renderer (the Dumb Font Engine) and some serious rendering things.

*SomeRandomGameDev* and *BlockoS/MooZ*
