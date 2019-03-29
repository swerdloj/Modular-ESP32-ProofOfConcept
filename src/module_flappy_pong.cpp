#include "preamble.h"
#include "module_flappy_pong.h"
#include "gtft.h"

// Ball Properties
uint16_t ball_size = 5;
int16_t ballX = 0;
int16_t ballY = 0;
int16_t ball_velocityX = 1;
int16_t ball_velocityY = 1;
uint16_t ball_speed = 1;
bool ball_is_currently_colliding = false;
bool ball_is_in_paddle_x_range = false;

// Padle Properties
uint16_t paddleHeight = 25;
uint16_t paddleWidth = 5;
int16_t paddleY = HEIGHT/2 - paddleHeight/2;
uint16_t paddleX = WIDTH/2 - paddleWidth/2; // technically a constant (hence unsigned)
uint16_t paddle_jump_height = 40;

// Button press logic
bool do_button_press = false;
bool is_pressed = false;

// Scoring
int score = 0;
int scale_rate = 10; //Speed up after this many points (interval)

// Function forward declarations
void updateButtonState();
void updateBallOptimized(int16_t, int16_t);
void updatePaddle(int16_t);
void reset_game();

/* Nothing to do here
void setup_flappy_pong() {
    tft().fillScreen(ST77XX_BLACK);
}
*/

void run_flappy_pong() {
    Serial.println("Starting Flappy Pong");

    tft().fillScreen(ST77XX_BLACK);

    // Main loop
    while (true) {
        //while(!delta_time_passed())
        //delay(5);
        tft().setCursor(0, 0);
        tft().println("Score: " + (String) score);

        updateButtonState();

        if(digitalRead(BUTTON_RETURN) == LOW) {
            draw_setup();
            Serial.println("Quitting Flappy Pong");
            return;
        }

        if (do_button_press) {
            //Serial.println("Processing Button Press");
            //updatePaddle(-paddle_jump_height - ball_speed*ball_speed);
            updatePaddle(-80);
        } else {
            //Serial.println("No Button Press");
            updatePaddle(1 + ball_speed - 1);
        }

        // Game speed
        for (uint16_t i = 0; i < abs(ball_speed); ++i) {
            updateBallOptimized(1, 1);

            if (ballX <= paddleX + paddleWidth + 1 && ballX >= paddleX - ball_size) { // is the ball where the paddle CAN be?
                if (!ball_is_currently_colliding && ballY <= paddleY + paddleHeight && ballY + ball_size >= paddleY) { // is the paddle size encompassing the ball's location?
                    Serial.println("Ball collided with paddle");

                    ball_is_currently_colliding = true;
                    //ball_speed += score % 5 == 0 ? 1 : 0;

                    ball_velocityX *= -1;

                    reset_game();
                } else if (!ball_is_in_paddle_x_range) {
                    ball_is_in_paddle_x_range = true;
                    score++;
                    ball_speed += score % scale_rate == 0 ? 1 : 0;
                    tft().fillRect(5*8, 0, 5*4, 7, ST77XX_BLACK); //clear old score
                }
            } else {
                ball_is_currently_colliding = false;
                ball_is_in_paddle_x_range = false;
            }
        }
    }
}

// Allow a button press only once per "hold"
inline void updateButtonState() {
    int in_state = digitalRead(BUTTON2);

    if (in_state == LOW) {
        if (is_pressed) {
            do_button_press = false;
            return;
        }
        is_pressed = true;
        do_button_press = true;
    } else if (in_state == HIGH) {
        is_pressed = false;
    }
}

inline void updateBallOptimized(int16_t deltaX, int16_t deltaY) {
    if (ballX + ball_size + deltaX > WIDTH) { // If the ball would go past the screen's width
        updateBallOptimized(WIDTH - (ballX + ball_size), deltaY); // clip it to the screen (right)
        return;
    } else if (ballX + deltaX < 0) { // If the ball would go under the screen's width (deltaX < 0)
        updateBallOptimized(-ballX, deltaY); // clip it to the screen (left)
        return;
    } else if (ballX + ball_size + deltaX == WIDTH || ballX + deltaX == 0) { // Ball is on the edge
        ball_velocityX *= -1;
    }

    if (ballY + ball_size + deltaY > HEIGHT) { // If the ball would go too low
        updateBallOptimized(deltaX, HEIGHT - (ballY + ball_size)); // clip it to the bottom
        return;
    } else if (ballY + deltaY < 0) { // If the ball would go too high
        updateBallOptimized(deltaX, -ballY); // clip it to the top
        return;
    } else if (ballY + ball_size + deltaY == HEIGHT || ballY + deltaY == 0) { // Ball is on the edge
        ball_velocityY *= -1;
    }

    tft().fillRect(ballX, ballY, ball_size, ball_size, ST7735_BLACK);
    
    // update ball coords
    ballX += deltaX * ball_velocityX;
    ballY += deltaY * ball_velocityY;

    /*
    Serial.println(ballX);
    Serial.println(ballY);
    Serial.println(ball_velocityX);
    Serial.println(ball_velocityY);
    Serial.println("");
    */

    tft().fillRect(ballX, ballY, ball_size, ball_size, ST7735_GREEN);
}

// TODO: LERP (falling is already psuedo LERP thanks to 1-pixel drops)
// Update the paddle's position using rediculously optimized methods to compensate for slow tft() display update rate.
inline void updatePaddle(int16_t deltaY) {
    if (deltaY > 0) { // Paddle moving down. We do not need to clip paddleY because deltaY will automatically adjust for this
        if (paddleY + paddleHeight + deltaY > HEIGHT) {
            updatePaddle(HEIGHT - (paddleY + paddleHeight));
            return;
        }
        tft().fillRect(paddleX, paddleY, paddleWidth, deltaY, ST7735_BLACK); // erase above moved location
        paddleY += deltaY;
        tft().fillRect(paddleX, paddleY+paddleHeight-deltaY, paddleWidth, deltaY, ST7735_WHITE); // fill in (bottom) moved location
    } else if (deltaY < 0) { // Paddle moving up. We de not need to clip paddleY because deltaY will automatically be set to 0 if needed
        if (paddleY + deltaY < 0) {
            updatePaddle(-paddleY);
            return;
        }
        // Note that deltaY is negative
        //tft().fillRect(paddleX, paddleY+paddleHeight+deltaY, paddleWidth, -deltaY, ST7735_BLACK); // erase below moved location
        tft().fillRect(paddleX, paddleY+paddleHeight+deltaY, paddleWidth, -deltaY, ST7735_BLACK); //TODO: Fix this
        paddleY += deltaY;
        tft().fillRect(paddleX, paddleY, paddleWidth, paddle_jump_height, ST7735_WHITE); // fill in (top) moved location
    } else if (paddleY+paddleHeight == HEIGHT || deltaY == 0) { // Paddle stationary
        return;
    }
}

// FIXME: Getting hit while jumping (same frame?) causes graphical bug below
void reset_game() {
    tft().fillScreen(ST7735_BLACK);

    ballX = 0;
    ballY = 0;
    ball_velocityX = 1;
    ball_velocityY = 1;
    ball_speed = 1;
    ball_is_currently_colliding = false;
    ball_is_in_paddle_x_range = false;
    score = 0;

    is_pressed = false;

    updatePaddle(HEIGHT);
    tft().fillRect(paddleX, 0, paddleWidth, HEIGHT, ST7735_BLACK);
    updatePaddle(-1);
}