#include <math.h>
#include <raylib.h>
#include <stdbool.h>
#include <stdint.h>

#define PADDLE_SPEED 6
#define PADDLE_HEIGHT 100
#define PADDLE_DISTANCE 400
#define PADDLE_WIDTH 30

#define BALL_SPEED 15
#define BALL_RADIUS 20
#define BALL_ACCELERATION 5

typedef struct {
  int y;
  int height;
  int speed;
  int velocity;
} Paddle;

typedef struct {
  float angle;
  int speed;
  int x;
  int y;
  int radius;
} Ball;

int32_t main() {
  int player1Points = 0;
  int player2Points = 0;
  InitWindow(0, 0, "C PONG");
  if (!IsWindowFullscreen()) {
    ToggleFullscreen();
  }

  int width = GetScreenWidth();
  int height = GetScreenHeight();

  Paddle paddle1 = {
      .y = (height - PADDLE_HEIGHT) / 2,
      .height = PADDLE_HEIGHT,
      .speed = PADDLE_SPEED,
  };

  Paddle paddle2 = {
      .y = (height - PADDLE_HEIGHT) / 2,
      .height = PADDLE_HEIGHT,
      .speed = PADDLE_SPEED,
  };

  SetTargetFPS(60);

  Ball ball = {
      .angle = 0,
      .speed = BALL_SPEED,
      .x = width / 2,
      .y = height / 2,
      .radius = BALL_RADIUS,
  };

  while (!WindowShouldClose()) {
    BeginDrawing();

    width = GetScreenWidth();
    height = GetScreenHeight();

    ClearBackground(BLACK);

    // Draw centre line
    DrawRectangle(width / 2 - 1, 0, 2, height, WHITE);

    // Draw Paddle 1
    DrawRectangle(PADDLE_DISTANCE, paddle1.y, PADDLE_WIDTH, paddle1.height,
                  WHITE);

    // Draw Paddle 2
    DrawRectangle(width - PADDLE_DISTANCE, paddle2.y, PADDLE_WIDTH,
                  paddle2.height, WHITE);

    // Draw Ball
    DrawCircle(ball.x, ball.y, ball.radius, WHITE);

    // Ball intersection
    bool ballIntersectsWithCieling =
        ball.y + ball.speed * sinf(ball.angle) <= 0;
    bool ballIntersectsWithFloor =
        ball.y + ball.speed * sinf(ball.angle) >= height;
    bool ballIntersectsWithRightPaddleLeftSide =
        (ball.x + ball.speed * cosf(ball.angle) >= width - PADDLE_DISTANCE) &&
        (ball.y + ball.radius >= paddle2.y &&
         ball.y - ball.radius <= paddle2.y + paddle2.height);
    bool ballIntersectsWithLeftPaddleRightSide =
        ((ball.x + ball.speed * cosf(ball.angle) <=
          PADDLE_DISTANCE + PADDLE_WIDTH) &&
         (ball.y + ball.radius >= paddle1.y &&
          ball.y - ball.radius <= paddle2.y + paddle2.height));

    if (ballIntersectsWithCieling || ballIntersectsWithFloor) {
      ball.angle = PI - 2 * ball.angle;
    }

    if (ballIntersectsWithLeftPaddleRightSide) {
      ball.angle = fmod(PI - ball.angle + PI / 60 * paddle1.velocity, PI * 2);
    }

    if (ballIntersectsWithRightPaddleLeftSide) {
      ball.angle = fmod(PI - ball.angle + PI / 60 * paddle2.velocity, PI * 2);
    }

    // Ball out of bounds
    if (ball.x >= width || ball.x <= 0) {
      if (ball.x >= width) {
        player1Points += 1;
      } else if (ball.x <= 0) {
        player2Points += 1;
      }
      ball.angle = 0;
      ball.speed = BALL_SPEED;
      ball.x = width / 2;
      ball.y = height / 2;
    }

    // Move ball
    ball.x += cosf(ball.angle) * ball.speed;
    ball.y += sinf(ball.angle) * ball.speed;

    // Paddle 1 movement
    if (IsKeyDown(KEY_W) && paddle1.y > 0) {
      paddle1.y -= PADDLE_SPEED;
      paddle1.velocity += 1;
    } else if (IsKeyDown(KEY_S) && paddle1.y + paddle1.height < height) {
      paddle1.y += PADDLE_SPEED;
      paddle1.velocity -= 1;
    } else {
      paddle1.velocity = 0;
    }

    // Paddle 2 movement
    if (IsKeyDown(KEY_UP) && paddle2.y > 0) {
      paddle2.y -= PADDLE_SPEED;
      paddle2.velocity += 1;
    } else if (IsKeyDown(KEY_DOWN) && paddle2.y + paddle2.height < height) {
      paddle2.y += PADDLE_SPEED;
      paddle2.velocity -= 1;
    } else {
      paddle2.velocity = 0;
    }
    EndDrawing();
  }

  CloseWindow();
  return 0;
}
