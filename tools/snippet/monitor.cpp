#include <GLFW/glfw3.h>

#include <iostream>

void handleErrorMessage(int code, const char *description) {
  std::cout << "[!!] (" << code << ") " << description << std::endl;
}

void handleMonitorChange(GLFWmonitor *monitor, int event) {
  std::cout << "Monitor(s) changed." << std::endl;
}

void displayVideoMode(const GLFWvidmode *mode) {
        std::cout << mode->width
                  << "x" << mode->height << "@" << mode->refreshRate
                  << "hZ (r:" << mode->redBits << " g:" << mode->greenBits
                  << " b:" << mode->blueBits << ")";
}

int main(int argc, char **argv) {

  glfwSetErrorCallback(handleErrorMessage);

  // Initialize GLFW
  if(glfwInit()) {

    glfwSetMonitorCallback(handleMonitorChange);

    // Gather monitor information.
    int monitorCount;

    GLFWmonitor **monitors = glfwGetMonitors(&monitorCount);
    GLFWmonitor *mainMonitor = glfwGetPrimaryMonitor();

    std::cout << "Found " << monitorCount << " monitor";
    if(monitorCount > 1) {
      std::cout << "s";
    }
    std::cout << "." << std::endl;

    // Listing informations for each monitors.
    for(int i = 0; i < monitorCount; ++i) {
      GLFWmonitor *monitor = monitors[i];
      int modeCount;

      const char *name = glfwGetMonitorName(monitor);
      std::cout << "Retrieving informations for monitor #" << i;
      if(NULL != name) {
        std::cout << " (" << name << ")";
      }
      std::cout << std::endl;
      // Main monitor ?
      if(mainMonitor == monitor) { // This condition is doubtful.
        std::cout << "\tThis is the main monitor." << std::endl;
      }
      // Physical properties.
      int width;
      int height;
      // Retrieve size in millimeters.
      glfwGetMonitorPhysicalSize(monitor, &width, &height);
      std::cout << "\tPhysical Size (in millimetres) is "
                << width << "x" << height << std::endl;
      // Retrieve relative position (upper-left corner).
      glfwGetMonitorPos(monitor, &width, &height);
      std::cout << "\tPosition (in pixels) is (" << width << ", "
                << height << ")" << std::endl;
      // Retrieve gamma ramp.
      const GLFWgammaramp *ramp = glfwGetGammaRamp(monitor);
      if(NULL != ramp) {
        std::cout << "\tCurrent Gamma Ramp (size: " << ramp->size << ")"
                  << std::endl;
        std::cout << "\t\tRed\tGreen\tBlue" << std::endl;
        for(int j = 0; j < ramp->size; ++j) {
          std::cout << "\t\t" << ramp->red[j] << "\t"
                    << ramp->green[j] << "\t"
                    << ramp->blue[j] << std::endl;
        }
      } else {
        std::cerr << "Unable to retrieve the gamma ramp of the monitor #"
                  << i << "." << std::endl;
      }
      // Video modes.
      const GLFWvidmode *currentMode = glfwGetVideoMode(monitor);
      std::cout << "\tCurrent Mode is ";
      displayVideoMode(currentMode);
      std::cout << std::endl;
      const GLFWvidmode *modes = glfwGetVideoModes(monitor, &modeCount);
      std::cout << "\tAvailable video modes :" << std::endl;
      for(int j = 0; j < modeCount; ++j) {
        std::cout << "\t\tMode #" << j << " : ";
        displayVideoMode(modes + j);
        std::cout << std::endl;
      }
    }

    glfwTerminate();
  }

  return 0;
}
