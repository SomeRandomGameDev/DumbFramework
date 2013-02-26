// Just a simple breakout to test sprites and so on ... Nothing fancy
// (at all).

#include <glm/glm.hpp>
#include <vector>
#include <iostream>

#include <log.hpp>

// Wanna see some speedcoding ?
// Last version was so bad that we won't talk about it, AT ALL.
// No, just no.
//
// No.

typedef struct {
  glm::vec2 _position;
  glm::vec2 _speed;
  double _progression;
} Ball;

#define PAD_NORMAL  0
#define PAD_GLUE    1
#define PAD_BLASTER 2

class Game {
public:

  /**
   * Initiliaze the game with one normal pad and one ball.
   */
  void init();

  /**
   * Main game loop.
   */
  void loop();
private:

  /**
   * A breakout game is about ... BALLS !
   */
  std::vector<Ball> _balls;

  /**
   * Pad position.
   */
  int _position;

  /**
   * Pad size.
   */
  int _size;

  /**
   * Pad state.
   */
  int _state;
};


void Game::init() {
  _state = PAD_NORMAL;

  Ball ball;
  ball._position = glm::vec2(0,0);
  ball._speed = glm::vec2(0, -1);
  ball._progression = 1.0;
  _balls.push_back(ball);
}

void Game::loop() {
  for( std::vector<Ball>::iterator i = _balls.begin();
       _balls.end() != i; ++i) {
    std::cout << "(" << i->_position.x << ", " << i->_position.y << ")" << std::endl;

	Log_Trace(1, ">> balls (%f,%f)", i->_position.x, i->_position.y);


    /*
     * While the progression is not over :
     *   Test if the ball goes out.
     *   Test if the ball hits the walls.
     *   Test if the ball hits the pad.
     *   Test if the ball traverse a brick tile
     *     Test if a brick is present
     *       Adjust speed, change brick state (remove it if necessary)
     *       Generate a bonus.
     */ 
  }
  
  /*
   * For each bonus,
   *   Test if it reaches the pad level.
   *     Test if the pad touches it.
   */

  /*
   * For each pad bullet,
   *   Test if it traverses a brick tile
   *     Test if a brick is present
   *       Change brick state.
   *       Generate a bonus.
   */
}

/*
 * 
 *
 *
 */

int main(void) {

  Game *game = new Game();

  Log::LogBuilder<Log::AllPassFilter, Log::SimpleMessageFormat> logBuilder; 
  Log::LogProcessor& logManager = Log::LogProcessor::instance();
  Log::ConsoleOutputPolicy consoleOutput;
  Log::FileOutputPolicy fileOutput;
  logManager.start(&logBuilder, &fileOutput);

  Log_Trace(1, "Start");

  game->init();
  game->loop();
  
  logManager.stop();

  delete game;
  return 0;
}
