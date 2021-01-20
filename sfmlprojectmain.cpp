#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <stdlib.h>

using namespace std;

int main()
{
    sf::RenderWindow window(sf::VideoMode(600, 600), "Simon Says");
    sf::RectangleShape startButton;
    sf::Text startText;
    sf::Clock interval;
    sf::Clock clock;
    sf::Color lightGrey(100, 100, 100);
    sf::Color lightBlue(116, 199, 232);

    sf::Time intervalSeconds = sf::seconds(0.2);
    int currentPos = 0;
    vector<int> sequence;
    vector<sf::RectangleShape> tiles;
    vector<int> sequenceInput;

    int nextInt;
    bool sequenceScreen = false;
    bool inputScreen = false;
    bool startScreen = true;
    bool gameOverScreen = false;
    bool finalScreen = false;

    for (int y = 0; y < 3; y++)
    {
        for (int x = 0; x < 3; x++)
        {
            int tileSide = 200;
            sf::RectangleShape rs;
            rs.setPosition(x*tileSide, y*tileSide);
            rs.setSize(sf::Vector2f(tileSide, tileSide));
            rs.setOutlineThickness(4);
            rs.setOutlineColor(sf::Color::Black);
            rs.setFillColor(lightGrey);
            tiles.push_back(rs);
        }
    }

    startButton.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));;
    startButton.setPosition(0, 0);
    startButton.setFillColor(lightGrey);

    sf::Font arial;
    arial.loadFromFile("arial.ttf");

    startText.setFont(arial);
    startText.setPosition(window.getSize().x/2, window.getSize().y/2);
    startText.setCharacterSize(50);
    startText.setString("Start");
    startText.setFillColor(sf::Color::Black);
    startText.setOrigin(50, 25);

    bool newInput = true;
    bool passed = false;
    bool resetClock = false;

    while (window.isOpen())
    {
        window.clear();
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (sequenceScreen == true)
        {
            if (interval.getElapsedTime() >= intervalSeconds)
            {
                if (tiles.at(sequence.at(currentPos)).getFillColor() == lightBlue)
                {
                    tiles.at(sequence.at(currentPos)).setFillColor(lightGrey);

                    if (currentPos < sequence.size() - 1)
                    {
                        currentPos++;
                    }
                    else
                    {
                        currentPos = 0;
                        sequenceScreen = false;
                        inputScreen = true;
                    }
                }
                else
                {
                    tiles.at(sequence.at(currentPos)).setFillColor(lightBlue);
                }
                interval.restart();
            }

            //Drawing the Tiles
            for (int x = 0; x < tiles.size(); x++)
            {
                window.draw(tiles.at(x));
            }
        }

        else if (inputScreen == true)
        {
            sf::Mouse mouse;
            for (int x = 0; x < tiles.size(); x++)
            {
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left) &&
                    tiles.at(x).getGlobalBounds().contains(mouse.getPosition(window).x, mouse.getPosition(window).y))
                {
                    tiles.at(x).setFillColor(lightBlue);
                    if (newInput == true)
                    {
                        sequenceInput.push_back(x);
                        newInput = false;
                    }
                }
            }

            if (not(sf::Mouse::isButtonPressed(sf::Mouse::Left)))
            {
                newInput = true;
                for (int x = 0; x < tiles.size(); x++)
                {
                    tiles.at(x).setFillColor(lightGrey);
                }
            }

            while (sequence.size() < sequenceInput.size())
            {
                sequenceInput.erase(sequenceInput.begin() + sequenceInput.size() - 1);
            }

            for (int x = 0; x < sequenceInput.size(); x++)
            {
                if (sequenceInput.at(x) != sequence.at(x))
                {
                    tiles.at(sequenceInput.at(x)).setFillColor(sf::Color::Red);
                    tiles.at(sequence.at(x)).setFillColor(lightBlue);
                    passed = false;
                    inputScreen = false;
                    gameOverScreen = true;
                    break;
                }
            }

            if (passed = true && sequenceInput.size() == sequence.size() && newInput == true)
            {
                if (resetClock == false)
                {
                    clock.restart();
                    resetClock = true;
                }
                if (clock.getElapsedTime() >= sf::seconds(1))
                {
                    inputScreen = false;
                    sequenceScreen = true;
                    resetClock = false;
                    sequenceInput.clear();
                    nextInt = rand() % 9;
                    sequence.push_back(nextInt);

                    for (int x = 0; x < tiles.size(); x++)
                    {
                        tiles.at(x).setFillColor(lightGrey);
                    }
                }
            }

            //Drawing the Tiles
            for (int x = 0; x < tiles.size(); x++)
            {
                window.draw(tiles.at(x));
            }
        }

        else if (startScreen == true)
        {
            sf::Mouse mouse;
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left) &&
                startButton.getGlobalBounds().contains(mouse.getPosition(window).x, mouse.getPosition(window).y))
            {
                if (resetClock == false)
                {
                    clock.restart();
                    resetClock = true;
                    nextInt = rand() % 9;
                    sequence.push_back(nextInt);
                }
            }
            if (resetClock == false)
            {
                window.draw(startButton);
                window.draw(startText);
            }
            if (clock.getElapsedTime() >= sf::seconds(1) && resetClock == true)
            {
                resetClock = false;
                startScreen = false;
                sequenceScreen = true;
            }
            if (resetClock == true && clock.getElapsedTime() < sf::seconds(1))
            {
                for (int x = 0; x < tiles.size(); x++)
                {
                    window.draw(tiles.at(x));
                }
            }
        }

        else if (gameOverScreen == true)
        {
            for (int x = 0; x < tiles.size(); x++)
            {
                window.draw(tiles.at(x));
            }

            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                if (resetClock == false)
                {
                    clock.restart();
                    resetClock = true;
                }
                if (clock.getElapsedTime() >= sf::seconds(0.5))
                {
                    gameOverScreen = false;
                    finalScreen = true;
                    resetClock = false;
                    clock.restart();
                }
            }
        }

        else if (finalScreen == true)
        {
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                if (resetClock == false && clock.getElapsedTime() >= sf::seconds(1))
                {
                    clock.restart();
                    resetClock = true;
                }
            }
            if (resetClock == false)
            {
                startText.setOrigin(80, 20);
                startText.setString("Score: " + to_string(sequence.size() - 1));
                window.draw(startButton);
                window.draw(startText);
            }
            if (clock.getElapsedTime() >= sf::seconds(1) && resetClock == true)
            {
                finalScreen = false;
                sequenceScreen = true;
                resetClock = false;
                sequence.clear();
                nextInt = rand() % 9;
                sequence.push_back(nextInt);
                sequenceInput.clear();
            }
            if (clock.getElapsedTime() < sf::seconds(1) && resetClock == true)
            {
                for (int x = 0; x < tiles.size(); x++)
                {
                    tiles.at(x).setFillColor(lightGrey);
                    window.draw(tiles.at(x));
                }
            }
        }

        window.display();
    }

    return 0;
}
