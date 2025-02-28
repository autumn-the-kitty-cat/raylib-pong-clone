#include <math.h>
#include <raylib.h>
#include <stdint.h>

#define PADDLE_SPEED 6
#define PADDLE_HEIGHT 250
#define BALL_SPEED 15
#define BALL_RADIUS 20

typedef struct {
    int y;
    int height;
    int speed;
} Paddle;

typedef struct {
    int angle;
    int speed;
    int x;
    int y;
    int radius;
} Ball;

int32_t main()
{
    InitWindow(0, 0, "CPP PONG");
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
        // Draw Paddle 1
        DrawRectangle(100, paddle1.y, 30, paddle1.height, WHITE);

        // Draw Paddle 2
        DrawRectangle(width - 100, paddle2.y, 30, paddle2.height, WHITE);
        EndDrawing();

        // Draw Ball
        DrawCircle(ball.x, ball.y, ball.radius, WHITE);

        // Ball intersect
        if (
            // Ball intersects with right paddle left side
            // Ball intersects with left paddle right side
            // Ball intersects with right paddle bottom side
            // Ball intersects with right paddle top side
            // Ball intersects with left paddle bottom side
            // Ball intersects with left paddle top side
            ball.y + ball.speed * sinf(ball.angle) <= 0 || // Ball intersects with cieling
            ball.y + ball.speed * sinf(ball.angle) >= height // Ball intersects with floor
        ) {
            ball.angle = -ball.angle;
            ball.speed = -ball.speed;
        }

        // Move ball
        ball.x += cosf(ball.angle) * ball.speed;
        ball.y += sinf(ball.angle) * ball.speed;

        // Paddle 1 movement
        if (IsKeyDown(KEY_W) && paddle1.y > 0) {
            paddle1.y -= PADDLE_SPEED;
        }
        if (IsKeyDown(KEY_S) && paddle1.y + paddle1.height < height) {
            paddle1.y += PADDLE_SPEED;
        }

        // Paddle 2 movement
        if (IsKeyDown(KEY_UP) && paddle2.y > 0) {
            paddle2.y -= PADDLE_SPEED;
        }
        if (IsKeyDown(KEY_DOWN) && paddle2.y + paddle2.height < height) {
            paddle2.y += PADDLE_SPEED;
        }
    }

    CloseWindow();
    return 0;
}
