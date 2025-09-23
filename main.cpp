#include <SFML/Graphics.hpp>

//define ball attributes

sf::Vector2f ball_velocity;
bool isPlayer1Serving = false;
const float initialVelocityX = 100.f; //horizontal velocity
const float initialVelocityY = 60.f; // vertical velocity

const float velocity_multiplier = 1.1f;

//Assign keyboard values
const sf::Keyboard::Key controls[4] = {
  sf::Keyboard::W, //Player1 UP
  sf::Keyboard::S, //Player1 Down
  sf::Keyboard::Up, //Player2 UP
  sf::Keyboard::Down //Player2 Down
};

//Paramaters
const sf::Vector2f paddleSize(25.f, 100.f);
const float ballRadius = 10.f;
const int gameWidth = 800;
const int gameHeight = 600;
const float paddleSpeed = 400.f;
const float paddleOffsetWall = 10.f;
const float time_step = 0.017f; //60 fps 

//Objects of the game 
sf::CircleShape ball;
sf::RectangleShape paddles[2];

void init() {
  // Set size and origin of paddles 
  for (sf::RectangleShape &p : paddles){
    p.setSize(paddleSize);
    p.setOrigin(paddleSize / 2.f);
  }
  //Set size and origin of ball
  ball.setRadius(ballRadius);
  ball.setOrigin(ballRadius / 2.f, ballRadius / 2.f); //should be half the ball width and height
  
  //set velocity of ball
  ball_velocity = { (isPlayer1Serving ? initialVelocityX : -initialVelocityX), initialVelocityY };
}

void reset() {
	//reset paddle position
	paddles[0].setPosition(paddleOffsetWall + paddleSize.x / 2.f, gameHeight / 2.f);
	paddles[1].setPosition((gameWidth - paddleOffsetWall) - paddleSize.x / 2.f, gameHeight / 2.f);
	//reset ball position 
	ball.setPosition(gameWidth / 2.f, gameHeight / 2.f);
}

void load() {
	ball_velocity = { (isPlayer1Serving ? initialVelocityX : -initialVelocityX), initialVelocityY };
}


void update(float dt) {
// handle paddle movement
    float direction1 = 0.0f;
	float direction2 = 0.0f;
    if (sf::Keyboard::isKeyPressed(controls[0])) {
        direction1--;
    }
    if (sf::Keyboard::isKeyPressed(controls[1])) {
        direction1++;
    }
	if (sf::Keyboard::isKeyPressed(controls[2])) {
		direction2--;
	}
	if (sf::Keyboard::isKeyPressed(controls[3])) {
		direction2++;
	}
  paddles[0].move(sf::Vector2f(0.f, direction1 * paddleSpeed * dt));
  paddles[1].move(sf::Vector2f(0.f, direction2 * paddleSpeed * dt));
  ball.move(ball_velocity * dt);

  // check ball collision
  const float bx = ball.getPosition().x;
  const float by = ball.getPosition().y;
  if (by > gameHeight) { //bottom wall
	  // bottom wall
	  ball_velocity.x *= velocity_multiplier;
	  ball_velocity.y *= -velocity_multiplier;
	  ball.move(sf::Vector2f(0.f, -10.f));
  }
  else if (by < 0) { //top wall
	  // top wall
	  ball_velocity.x *= velocity_multiplier;
	  ball_velocity.y *= -velocity_multiplier;
	  ball.move(sf::Vector2f(0.f, 10.f));
  }
  else if (bx > gameWidth) {
	  // right wall
	  reset();
  }
  else if (bx < 0) {
	  // left wall
	  reset();
  }
  else if (
	  //ball is inline or behind paddle AND
	  bx < paddleSize.x + paddleOffsetWall &&
	  //ball is below top edge of paddle AND
	  by > paddles[0].getPosition().y - (paddleSize.y * 0.5) &&
	  //ball is above bottom edge of paddle
	  by < paddles[0].getPosition().y + (paddleSize.y * 0.5)) {
		  {
			  ball_velocity.x *= -velocity_multiplier;
			  ball_velocity.y *= velocity_multiplier;
			  ball.move(sf::Vector2f(0.f, 1.f));
		  }
  }
  else if (
	  //ball is inline or behind paddle AND
	  bx > (gameWidth - paddleOffsetWall) - paddleSize.x &&
	  //ball is below top edge of paddle AND
	  by > paddles[1].getPosition().y - (paddleSize.y * 0.5) &&
	  //ball is above bottom edge of paddle
	  by < paddles[1].getPosition().y + (paddleSize.y * 0.5)) {
		  {
			  ball_velocity.x *= -velocity_multiplier;
			  ball_velocity.y *= velocity_multiplier;
			  ball.move(sf::Vector2f(0.f, 1.f));
		  }
  }
}

void render(sf::RenderWindow &window) {
	// Draw Everything
  window.draw(paddles[0]);
  window.draw(paddles[1]);
  window.draw(ball);
}

void clean(){}
	//free up the memory if necessary.


int main(){
  //create the window
	sf::RenderWindow window(sf::VideoMode({gameWidth, gameHeight}), "PONG");
    //initialise and load
	init();
	reset();
	while (window.isOpen()) {
		static sf::Clock clock;
		const float dt = clock.restart().asSeconds();
		window.clear();
		update(dt);
		render(window);
		//wait for the time_step to finish before displaying the next frame.
		sf::sleep(sf::seconds(time_step));
		//Wait for Vsync
		window.display();
	};
	//Unload and shutdown
	clean();
}