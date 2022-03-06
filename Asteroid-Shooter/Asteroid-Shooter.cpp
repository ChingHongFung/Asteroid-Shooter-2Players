// Asteroid-Shooter.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <algorithm>

#include "olcConsoleGameEngine.h"

using namespace std;

class Asteroids : public olcConsoleGameEngine
{
public:
    Asteroids()
    {
        m_sAppName = L"Asteroids";
    }

private:
    struct sSpaceObject
    {
        float x;
        float y;
        float dx;
        float dy;
        int nSize;
        float angle;
    };

    vector<sSpaceObject> vecAsteroids;
    vector<sSpaceObject> vecBullets1;
    vector<sSpaceObject> vecBullets2;
    sSpaceObject player1;
    sSpaceObject player2;
    int nScore1 = 0;
    int nScore2 = 0;
    bool bDead1 = false;
    bool bDead2 = false;
    bool bStart = true;
    vector<pair<float, float>> vecModelShip;
    vector<pair<float, float>> vecModelAsteroid;

protected:
    virtual bool OnUserCreate()
    {
        // Initialise ship model
        vecModelShip =
        {
            { 0.0f, -5.0f },
            { -2.0f, +2.5f },
            { +2.0f, +2.5f }
        };
        // Initialise circular asteroid
        int verts = 20;
        for (int i = 0; i < verts; i++)
        {
            float radius = (float)rand() / (float)RAND_MAX * 0.4f + 0.8f;
            float a = ((float)i / (float)verts) * 6.28318f;
            vecModelAsteroid.push_back(make_pair(radius * sinf(a), radius * cosf(a)));
        }

        return true;
    }

    virtual bool OnUserUpdate(float fElapsedTime)
    {
        // Start game by pressing space bar 
        if (bStart)
        {
            Fill(0, 0, ScreenWidth(), ScreenHeight(), PIXEL_SOLID, 0);
            DrawString(ScreenWidth() / 2 - 13, ScreenHeight() / 2 - 10, L"Welcome to Asteroids");
            DrawString(ScreenWidth() / 2 - 15, ScreenHeight() / 2 - 5, L"Press Space Bar to Start");
            DrawString(ScreenWidth() / 2 - 35, ScreenHeight() / 2 + 5, L"Player 1 Controls:");
            DrawString(ScreenWidth() / 2 - 35, ScreenHeight() / 2 + 8, L"W - Accelerate");
            DrawString(ScreenWidth() / 2 - 35, ScreenHeight() / 2 + 11, L"A - Turn Left");
            DrawString(ScreenWidth() / 2 - 35, ScreenHeight() / 2 + 14, L"D - Turn Right");
            DrawString(ScreenWidth() / 2 - 35, ScreenHeight() / 2 + 17, L"F - Shoot");
            DrawString(ScreenWidth() / 2 + 15, ScreenHeight() / 2 + 5, L"Player 2 Controls:");
            DrawString(ScreenWidth() / 2 + 15, ScreenHeight() / 2 + 8, L"Up - Accelerate");
            DrawString(ScreenWidth() / 2 + 15, ScreenHeight() / 2 + 11, L"Left - Turn Left");
            DrawString(ScreenWidth() / 2 + 15, ScreenHeight() / 2 + 14, L"Right - Turn Right");
            DrawString(ScreenWidth() / 2 + 15, ScreenHeight() / 2 + 17, L"0 - Shoot");
            if (m_keys[VK_SPACE].bHeld)
                bStart = false;
                ResetGame();
        }
        // Restart game by pressing space bar
        else if (bDead1 && bDead2)
        {
            Fill(0, 0, ScreenWidth(), ScreenHeight(), PIXEL_SOLID, 0);
            DrawString(ScreenWidth() / 2 - 7, ScreenHeight() / 2 - 5, L"Game Over");
            DrawString(ScreenWidth() / 2 - 15, ScreenHeight() / 2 + 5, L"Press Space Bar to Restart");
            if (m_keys[VK_SPACE].bHeld)
                ResetGame();
        }
        else 
        {
            // Clear Screen
            Fill(0, 0, ScreenWidth(), ScreenHeight(), PIXEL_SOLID, 0);

            // SHIP SECTION
            // Ship Steer: Rotating
            // Player 1 (W to turn left; D to turn right)
            if (m_keys[0x41].bHeld)
                player1.angle -= 5.0f * fElapsedTime;
            if (m_keys[0x44].bHeld)
                player1.angle += 5.0f * fElapsedTime;
            // Player 2 (Left to turn left; Right to turn right)
            if (m_keys[VK_LEFT].bHeld)
                player2.angle -= 5.0f * fElapsedTime;
            if (m_keys[VK_RIGHT].bHeld)
                player2.angle += 5.0f * fElapsedTime;

            // Ship Thrust: Accelerating
            // Player 1 (W to accelerate)
            if (m_keys[0x57].bHeld)
            {
                player1.dx += sin(player1.angle) * 20.0f * fElapsedTime;
                player1.dy += -cos(player1.angle) * 20.0f * fElapsedTime;
            }
            // Player 2 (Up to accelerate)
            if (m_keys[VK_UP].bHeld)
            {
                player2.dx += sin(player2.angle) * 20.0f * fElapsedTime;
                player2.dy += -cos(player2.angle) * 20.0f * fElapsedTime;
            }

            // Ship Position
            player1.x += player1.dx * fElapsedTime;
            player1.y += player1.dy * fElapsedTime;
            player2.x += player2.dx * fElapsedTime;
            player2.y += player2.dy * fElapsedTime;

            // Keep ships wrapping around frame
            WrapCoordinates(player1.x, player1.y, player1.x, player1.y);
            WrapCoordinates(player2.x, player2.y, player2.x, player2.y);

            // Check ship collision with asteroids
            for (auto& a : vecAsteroids)
            {
                if (IsPointInsideCircle(a.x, a.y, a.nSize, player1.x, player1.y))
                    bDead1 = true;
                if (IsPointInsideCircle(a.x, a.y, a.nSize, player2.x, player2.y))
                    bDead2 = true;
            }

            // ASTEROID SECTION
            // Asteroid Position
            for (auto& a : vecAsteroids) {
                // Update asteroid position and angle
                a.x += a.dx * fElapsedTime;
                a.y += a.dy * fElapsedTime;
                a.angle += 0.5f * fElapsedTime;

                // Keep asteroid wrapping around frame
                WrapCoordinates(a.x, a.y, a.x, a.y);
                DrawWireFrameModel(vecModelAsteroid, a.x, a.y, a.angle, (float)a.nSize, FG_GREY);
            }

            // BULLET SECTION
            // Add bullets to vector of bullets for drawing
            // Player 1 (F to shoot)
            if (m_keys[0x46].bReleased)
                vecBullets1.push_back({ player1.x, player1.y, 50.0f * sin(player1.angle), -50.0f * cosf(player1.angle), 0, 0 });
            // Player 2 (Number pad 0 to shoot)
            if (m_keys[VK_NUMPAD0].bReleased)
                vecBullets2.push_back({ player2.x, player2.y, 50.0f * sin(player2.angle), -50.0f * cosf(player2.angle), 0, 0 });

            // Initialise a vector to store new asteroids if bullets hit
            vector<sSpaceObject> newAsteroids;

            // Update bullet position then draw
            // Bullets from player 1
            for (auto& b : vecBullets1) {
                b.x += b.dx * fElapsedTime;
                b.y += b.dy * fElapsedTime;

                // Keep bullet wrapping around frame
                // WrapCoordinates(b.x, b.y, b.x, b.y);
                Draw(b.x, b.y);

                // Check bullet collision with asteroids
                for (auto& a : vecAsteroids)
                {
                    // Asteroid hit by bullet
                    if (IsPointInsideCircle(a.x, a.y, a.nSize, b.x, b.y))
                    {
                        nScore1 += 100;

                        // If asteroid still big; break it into two
                        if (a.nSize > 4)
                        {
                            // Create two child asteroids
                            float angle1 = ((float)rand() / (float)RAND_MAX) * 6.283185f;
                            float angle2 = ((float)rand() / (float)RAND_MAX) * 6.283185f;
                            newAsteroids.push_back({ a.x, a.y, 10.0f * sinf(angle1), 10.0f * cosf(angle1), (int)a.nSize >> 1, 0.0f });
                            newAsteroids.push_back({ a.x, a.y, 10.0f * sinf(angle2), 10.0f * cosf(angle2), (int)a.nSize >> 1, 0.0f });
                        }
                        // Set positions of asteroid and bullet to arbitary values for clean up later
                        a.x = -100;
                        b.x = -100;
                    }
                }
            }
            // Bullets from player 2
            for (auto& b : vecBullets2) {
                b.x += b.dx * fElapsedTime;
                b.y += b.dy * fElapsedTime;

                // Keep bullet wrapping around frame
                // WrapCoordinates(b.x, b.y, b.x, b.y);
                Draw(b.x, b.y);

                // Check bullet collision with asteroids
                for (auto& a : vecAsteroids)
                {
                    // Asteroid hit by bullet
                    if (IsPointInsideCircle(a.x, a.y, a.nSize, b.x, b.y))
                    {
                        nScore2 += 100;

                        // If asteroid still big; break it into two
                        if (a.nSize > 4)
                        {
                            // Create two child asteroids
                            float angle1 = ((float)rand() / (float)RAND_MAX) * 6.283185f;
                            float angle2 = ((float)rand() / (float)RAND_MAX) * 6.283185f;
                            newAsteroids.push_back({ a.x, a.y, 10.0f * sinf(angle1), 10.0f * cosf(angle1), (int)a.nSize >> 1, 0.0f });
                            newAsteroids.push_back({ a.x, a.y, 10.0f * sinf(angle2), 10.0f * cosf(angle2), (int)a.nSize >> 1, 0.0f });
                        }
                        // Set positions of asteroid and bullet to arbitary values for clean up later
                        a.x = -100;
                        b.x = -100;
                    }
                }
            }

            // Add new asteroids from bullet hits into vec for drawing
            for (auto a : newAsteroids)
                vecAsteroids.push_back(a);

            // Remove off screen bullets
            if (vecBullets1.size() > 0)
            {
                auto i = remove_if(vecBullets1.begin(), vecBullets1.end(),
                    [&](sSpaceObject o) {return (o.x < 1 || o.y < 1 || o.x >= ScreenWidth() - 1 || o.y >= ScreenHeight() - 1); });
                if (i != vecBullets1.end())
                    vecBullets1.erase(i);
            }
            if (vecBullets2.size() > 0)
            {
                auto i = remove_if(vecBullets2.begin(), vecBullets2.end(),
                    [&](sSpaceObject o) {return (o.x < 1 || o.y < 1 || o.x >= ScreenWidth() - 1 || o.y >= ScreenHeight() - 1); });
                if (i != vecBullets2.end())
                    vecBullets2.erase(i);
            }

            // Remove off screen asteroids
            if (vecAsteroids.size() > 0)
            {
                auto i = remove_if(vecAsteroids.begin(), vecAsteroids.end(),
                    [&](sSpaceObject o) {return (o.x < 0); });
                if (i != vecAsteroids.end())
                    vecAsteroids.erase(i);
            }

            // Finished hitting a round of asteroids; generate new ones
            if (vecAsteroids.empty())
            {
                // If ship is alive when a round of asteroids eliminated; add to its score
                if (!bDead1)
                    nScore1 += 1000;
                if (!bDead2)
                    nScore2 += 1000;

                // Position new asteroids at an offset away from current ship position
                // If only one ship alive, then position them to the left and right of the ship based on its orientation
                if (bDead1 && !bDead2) {
                    vecAsteroids.push_back({ 30.0f * sinf(player2.angle - 3.14159f / 2.0f) + player2.x,
                                        30.0f * cosf(player2.angle - 3.14159f / 2.0f) + player2.y,
                                        10.0f * sinf(player2.angle),
                                        10.0f * cosf(player2.angle),
                                        (int)16, 0.0f });
                    vecAsteroids.push_back({ 30.0f * sinf(player2.angle + 3.14159f / 2.0f) + player2.x,
                                        30.0f * cosf(player2.angle + 3.14159f / 2.0f) + player2.y,
                                        10.0f * sinf(-player2.angle),
                                        10.0f * cosf(-player2.angle),
                                        (int)16, 0.0f });
                }
                if (!bDead1 && bDead2) {
                    vecAsteroids.push_back({ 30.0f * sinf(player1.angle - 3.14159f / 2.0f) + player1.x,
                                        30.0f * cosf(player1.angle - 3.14159f / 2.0f) + player1.y,
                                        10.0f * sinf(player1.angle),
                                        10.0f * cosf(player1.angle),
                                        (int)16, 0.0f });
                    vecAsteroids.push_back({ 30.0f * sinf(player1.angle + 3.14159f / 2.0f) + player1.x,
                                        30.0f * cosf(player1.angle + 3.14159f / 2.0f) + player1.y,
                                        10.0f * sinf(-player1.angle),
                                        10.0f * cosf(-player1.angle),
                                        (int)16, 0.0f });
                }
                // If both ships alive, then position them an offset perpendicular to the segment that join the two ships
                if (!bDead1 && !bDead2)
                {
                    float midx = (player1.x + player2.x) / 2.0f;
                    float midy = (player1.y + player2.y) / 2.0f;
                    float anglexy = atanf((player2.y - player1.y) / (player2.x - player1.x));
                    vecAsteroids.push_back({ 30.0f * sinf(anglexy) + midx,
                                             30.0f * cosf(-anglexy) + midy,
                                             10.0f * sinf(anglexy),
                                             10.0f * cosf(anglexy),
                                             (int)16, 0.0f });
                    vecAsteroids.push_back({ 30.0f * sinf(-anglexy) + midx,
                                             30.0f * cosf(anglexy) + midy,
                                             10.0f * sinf(-anglexy),
                                             10.0f * cosf(-anglexy),
                                             (int)16, 0.0f });
                }
            }

            // Draw ship if still alive
            if (!bDead1)
                DrawWireFrameModel(vecModelShip, player1.x, player1.y, player1.angle, 1.0f, FG_DARK_GREEN);
            if (!bDead2)
                DrawWireFrameModel(vecModelShip, player2.x, player2.y, player2.angle, 1.0f, FG_DARK_MAGENTA);

            // Draw score; show status as dead if collided with asteroids
            if (!bDead1)
                DrawString(2, 2, L"PLAYER1 SCORE: " + to_wstring(nScore1));
            else
                DrawString(2, 2, L"PLAYER1 DEAD, SCORE: " + to_wstring(nScore1));
            if (!bDead2)
                DrawString(ScreenWidth() - 26, 2, L"PLAYER2 SCORE: " + to_wstring(nScore2));
            else
                DrawString(ScreenWidth() - 26, 2, L"PLAYER2 DEAD, SCORE: " + to_wstring(nScore2));
        }
        
           
        return true;
    }

    void ResetGame()
    {
        // Clear existing vectors of asteroids and bullets
        vecAsteroids.clear();
        vecBullets1.clear();
        vecBullets2.clear();

        // Initialise asteroids
        vecAsteroids.push_back({ 20.0f, 20.0f, 8.0f, -6.0f, (int)16, 0.0f });
        vecAsteroids.push_back({ 100.0f, 20.0f, -5.0f, 3.0f, (int)16, 0.0f });

        // Initialise player
        player1.x = ScreenWidth() / 4.0f;
        player1.y = ScreenHeight() * 3.0f / 4.0f;
        player1.dx = 0.0f;
        player1.dy = 0.0f;
        player1.angle = 0.0f;

        player2.x = ScreenWidth() * 3.0f / 4.0f;
        player2.y = ScreenHeight() * 3.0f / 4.0f;
        player2.dx = 0.0f;
        player2.dy = 0.0f;
        player2.angle = 0.0f;

        // Reset alive status and scores
        bDead1 = false;
        bDead2 = false;
        nScore1 = 0;
        nScore2 = 0;
    }

    bool IsPointInsideCircle(float cx, float cy, float radius, float x, float y)
    {
        // Check if centre point of ship is within the radius of the asteroid
        return sqrt((x - cx) * (x - cx)) + sqrt((y - cy) * (y - cy)) < radius;
    }

    void DrawWireFrameModel(const vector<pair<float, float>>& vecModelCoordinates, float x, float y, float r = 0.0f, float s = 1.0f, short col = FG_WHITE)
    {
        // Create translated model vector of coordiante pairs
        vector<pair<float, float>> vecTransformedCoordinates;
        int verts = vecModelCoordinates.size();
        vecTransformedCoordinates.resize(verts);

        // Rotate
        for (int i = 0; i < verts; i++)
        {
            vecTransformedCoordinates[i].first = vecModelCoordinates[i].first * cosf(r) - vecModelCoordinates[i].second * sinf(r);
            vecTransformedCoordinates[i].second = vecModelCoordinates[i].first * sinf(r) + vecModelCoordinates[i].second * cosf(r);
        }
        // Scale
        for (int i = 0; i < verts; i++)
        {
            vecTransformedCoordinates[i].first = vecTransformedCoordinates[i].first * s;
            vecTransformedCoordinates[i].second = vecTransformedCoordinates[i].second * s;
        }
        // Translate
        for (int i = 0; i < verts; i++)
        {
            vecTransformedCoordinates[i].first = vecTransformedCoordinates[i].first + x;
            vecTransformedCoordinates[i].second = vecTransformedCoordinates[i].second + y;
        }
        // Draw polygon shape
        for (int i = 0; i < verts; i++)
        {
            int j = (i + 1);
            DrawLine(vecTransformedCoordinates[i % verts].first, vecTransformedCoordinates[i % verts].second, vecTransformedCoordinates[j % verts].first, vecTransformedCoordinates[j % verts].second, PIXEL_SOLID, col);
        }
    }

    void WrapCoordinates(float ix, float iy, float& ox, float& oy)
    {
        // Wrap positioning of object back onto game space if drifted off the edges
        ox = ix;
        oy = iy;
        if (ix < 0.0f) ox = ix + (float)ScreenWidth();
        if (ix >= (float)ScreenWidth()) ox = ix - (float)ScreenWidth();
        if (iy < 0.0f) oy = iy + (float)ScreenHeight();
        if (iy >= (float)ScreenHeight()) oy = iy - (float)ScreenHeight();
    }

    virtual void Draw(int x, int y, short c = 0x2588, short col = 0x000F)
    {
        float fx, fy;
        WrapCoordinates(x, y, fx, fy);
        olcConsoleGameEngine::Draw(fx, fy, c, col);
    }
};

int main()
{
    Asteroids game;
    game.ConstructConsole(160, 100, 8, 8);
    game.Start();
    return 0;
}