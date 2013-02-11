// Just a simple breakout to test sprites and so on ... Nothing fancy
// (at all).

#include <glm/glm.hpp>

namespace Demo {

// Wanna see some speedcoding ?
// This WONT compile.

class Collidable {
public:
  /**
   * Given a rigid body, does it collide and what modifications occurs ?
   * @param initial Initial state expressed as (posX, posY, vX, vY).
   * @return New state as (posX, posY, vX, vY).
   */
  virtual glm::vec4 alter(glm::vec4 initial) = 0;

private:
  /**
   * Where is it ?
   */
  glm::vec4 _placeholder;
};

class Game {
public:
  /**
   * Main model game loop.
   * @return true as long as the game needs to run.
   */
  bool loop();
private:
  /**
   * Down to zero and the game is over.
   */
  unsigned int _lives;
};


bool Game::loop() {
  // This method is called at each 'simulation' step.

  for(int i = 0; i < _activeBalls; ++i) {
    for(int j = 0; j < _activeCollidables; ++j) {
      _balls[i] = _collidable[j]->alter(_balls[i]); // Yek
    }
  }

  // Remove misplaced balls.
  for(int i = _activeBalls - 1; i >= 0; --i) {
    if(isInvalidPosition(_balls[i])) {
      removeBall(i);
    }
  }

  // Remove obsolete collidables.
  for(int i = _activeCollidables; i >= 0; --i) {
    if(_collidable[i]->isInvalid()) {
      removeCollidable(i);
    }
  }

  // ## TODO Bonuses

}
