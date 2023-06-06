//Created via tutorials: https://www.sfml-dev.org/tutorials/2.5

/*
	8b,dPPYba,   ,adPPYba,  8b,dPPYba,    ,adPPYb,d8
	88P'    "8a a8"     "8a 88P'   `"8a  a8"    `Y88
	88       d8 8b       d8 88       88  8b       88
	88b,   ,a8" "8a,   ,a8" 88       88  "8a,   ,d88
	88`YbbdP"'   `"YbbdP"'  88       88   '"YbbdP"Y8
	88                                    aa,    ,88
	88                                     "Y8bbdP"    By Stepan0Tap
*/

#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <windows.h>
#include <time.h>
#include <string>

using namespace sf;

int main()
{
	//Constants
	srand(time(NULL));
	const float pi = 3.14159f;
	bool is_paused = false;
	int score1 = 10;
	int score2 = 10;
	


	//Sound importing
	SoundBuffer pipSoundBuffer;
	pipSoundBuffer.loadFromFile("resources/sounds/pip.wav");
	Sound pipSound(pipSoundBuffer);
	SoundBuffer punchSoundBuffer;
	punchSoundBuffer.loadFromFile("resources/sounds/punch.wav");
	Sound punchSound(punchSoundBuffer);
	SoundBuffer pauseSoundBuffer;
	pauseSoundBuffer.loadFromFile("resources/sounds/pause.wav");
	Sound pauseSound(pauseSoundBuffer);




	//Paddles sprites and properties
	float paddleSpeed = 300;
	Vector2f paddleSize(8, 64);

	RectangleShape player1 (paddleSize);
	player1.setFillColor(Color::Black);
	player1.setOutlineThickness(2);
	player1.setOutlineColor(Color::White);
	player1.setPosition(32, 96);

	RectangleShape player2(paddleSize);
	player2.setFillColor(Color::Black);
	player2.setOutlineThickness(2);
	player2.setOutlineColor(Color::White);
	player2.setPosition(472, 96);



	//Ball sprite and properties
	int ballXPos = 256;
	int ballYPos = 128;
	float ballSpeed = 200;
	int ballRadius = 6;
	float ballAngle = 0;

	do
		ballAngle = (std::rand() % 360) * 2 * pi / 360;
	while (std::abs(std::cos(ballAngle)) < 0.7f);

	CircleShape ball;
	ball.setRadius(ballRadius - 2);
	ball.setFillColor(Color::Transparent);
	ball.setOutlineThickness(2);
	ball.setOutlineColor(Color::White);
	ball.setOrigin(ballRadius / 2, ballRadius / 2);
	ball.setPosition(ballXPos, ballYPos);



	//Other sprites...
	RectangleShape outline(Vector2f(510, 254));
	outline.setPosition(1, 1);
	outline.setFillColor(Color::Transparent);
	outline.setOutlineThickness(1);
	outline.setOutlineColor(Color::White);

	RectangleShape boom (Vector2f(256,256));
	boom.setFillColor(Color(255,255,255,200));

	RectangleShape separator(Vector2f(4, 12));
	separator.setFillColor(Color::White);

	RectangleShape cursor(Vector2f(16, 16));
	cursor.setOutlineThickness(2);
	cursor.setOutlineColor(Color(255, 255, 255, 180));
	cursor.setFillColor(Color::Transparent);
	cursor.setOrigin(8, 8);



	//Texts and Fonts
	Font font;
	font.loadFromFile("resources/fonts/blockypong.ttf");

	Text menuText1;
	menuText1.setFont(font);
	menuText1.setString("Pong");
	menuText1.setCharacterSize(88);
	menuText1.setOrigin(64,64);
	menuText1.setFillColor(Color(255, 255, 255, 180));
	menuText1.setPosition(197, 50);

	Text menuText2;
	menuText2.setFont(font);
	menuText2.setString("play");
	menuText2.setCharacterSize(44);
	menuText2.setOrigin(64, 64);
	menuText2.setFillColor(Color(255, 255, 255, 180));
	menuText2.setPosition(257, 165);

	Text menuText3;
	menuText3.setFont(font);
	menuText3.setString("exit");
	menuText3.setCharacterSize(44);
	menuText3.setOrigin(64, 64);
	menuText3.setFillColor(Color(255, 255, 255, 180));
	menuText3.setPosition(257, 220);
	
	Text menuText4;
	menuText4.setFont(font);
	menuText4.setString("1 player");
	menuText4.setCharacterSize(44);
	menuText4.setOrigin(64, 64);
	menuText4.setFillColor(Color(255, 255, 255, 180));
	menuText4.setPosition(257, 165);

	Text menuText5;
	menuText5.setFont(font);
	menuText5.setString("2 players");
	menuText5.setCharacterSize(44);
	menuText5.setOrigin(64, 64);
	menuText5.setFillColor(Color(255, 255, 255, 180));
	menuText5.setPosition(257, 220);




	Text player1Score;
	player1Score.setFont(font);
	player1Score.setString(std::to_string(score1));
	player1Score.setCharacterSize(128);
	player1Score.setFillColor(Color(255,255,255,32));
	player1Score.setOrigin(64, 64);
	player1Score.setPosition(168, 100);

	Text player2Score;
	player2Score.setFont(font);
	player2Score.setString(std::to_string(score2));
	player2Score.setCharacterSize(128);
	player2Score.setFillColor(Color(255, 255, 255, 32));
	player2Score.setOrigin(64, 64);
	player2Score.setPosition(384, 100);

	Text pauseText1;
	pauseText1.setFont(font);
	pauseText1.setString("PAUSE");
	pauseText1.setCharacterSize(80);
	pauseText1.setFillColor(Color(255, 255, 255, 180));
	pauseText1.setOrigin(24, 48);
	pauseText1.setPosition(143, 50);

	Text pauseText2;
	pauseText2.setFont(font);
	pauseText2.setString("  continue\n       exit");
	pauseText2.setCharacterSize(44);
	pauseText2.setFillColor(Color(255, 255, 255, 180));
	pauseText2.setOrigin(24, 48);
	pauseText2.setPosition(139, 148);


	//Essential variables and Clock initialising
	bool boomAnimation = false;
	float animationStart = -1;
	int animationNow;
	int fade = 255;
	bool ballExist = true;
	Clock FPSClock;
	Clock animationClock;



	//Window rendering
	RenderWindow window(VideoMode(512, 256), "Pong by Stepan0Tap!", Style::Titlebar | Style::Close);
	window.setVerticalSyncEnabled(true);



	//Menu
	enum MenuOptions { PLAY = 0, EXITM };
	MenuOptions menuOption = PLAY;

	enum GameMode {PvsC = 0, PvsP};
	GameMode gameMode = PvsC;

	enum MenuStatus {START = 0, GAMEMODE, GAME};
	MenuStatus menuStatus = START;

	cursor.setPosition(163, 133);

	while (window.isOpen() &&  menuStatus != GAME)
	{
		window.clear(Color::Black);
		float deltaTime = FPSClock.restart().asSeconds();
		Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case Event::Closed:
				window.close();
				break;
			case Event::KeyPressed:
				switch (event.key.code)
				{
				case Keyboard::Up:
					switch (menuStatus)
					{
					case START:
						if (menuOption == PLAY)
						{
							menuOption = EXITM;
							cursor.setPosition(163, 188);
						}
						else
						{
							cursor.setPosition(163, 133);
							menuOption = PLAY;
						}
						break;
					case GAMEMODE:
						if (gameMode == PvsC)
							gameMode = PvsP;
						else
							gameMode = PvsC;
						break;
					}
					break;
				case Keyboard::Down:
					switch (menuStatus)
					{
					case START:
						if (menuOption == PLAY)
						{
							menuOption = EXITM;
							cursor.setPosition(163, 188);
						}
						else
						{
							cursor.setPosition(163, 133);
							menuOption = PLAY;
						}
						break;
					case GAMEMODE:
						if (gameMode == PvsC)
							gameMode = PvsP;
						else
							gameMode = PvsC;
						break;
					}
					break;
				}
				break;
			case Event::KeyReleased:
				if (event.key.code == Keyboard::Enter)
				{
					switch (menuStatus)
					{
					case START:
						if (menuOption == PLAY)
							menuStatus = GAMEMODE;
						else
							window.close();
						break;
					case GAMEMODE:
						menuStatus = GAME;
						break;
					}
				}
			}
		}
		window.draw(menuText1);
		window.draw(cursor);
		if (menuStatus == START)
		{
			window.draw(menuText2);
			window.draw(menuText3);
		}
		else
		{
			window.draw(menuText4);
			window.draw(menuText5);
		}
		window.display();
	}



	enum PauseOptions { CONTINUE = 0, EXITP };
	PauseOptions option;

	//Game cycle
	while (window.isOpen())
	{
		window.clear(Color::Black);
		float deltaTime = FPSClock.restart().asSeconds();
		Event event;
		if (!is_paused)
		{
			
			while (window.pollEvent(event))
			{
				switch (event.type)
				{
				case Event::Closed:
					window.close();
					break;
				case Event::KeyReleased:
					if (event.key.code == sf::Keyboard::Escape)
					{
						player1.setOutlineColor(Color(255, 255, 255, 64));
						player2.setOutlineColor(Color(255, 255, 255, 64));
						ball.setOutlineColor(Color(255, 255, 255, 64));
						outline.setOutlineColor(Color(255, 255, 255, 64));
						separator.setFillColor(Color(255, 255, 255, 64));
						player1Score.setFillColor(Color(255, 255, 255, 16));
						player2Score.setFillColor(Color(255, 255, 255, 16));
						is_paused = true;
						pauseSound.play();
						option = CONTINUE;
						cursor.setPosition(120, 132);
					}	
				}

			}
			if (Keyboard::isKeyPressed(Keyboard::W) && player1.getPosition().y >= 4 && !is_paused)
				player1.move(0, -paddleSpeed * deltaTime);
			else if (Keyboard::isKeyPressed(Keyboard::S) && player1.getPosition().y <= 252 - paddleSize.y)
				player1.move(0, paddleSpeed * deltaTime);

			if (Keyboard::isKeyPressed(Keyboard::Up) && player2.getPosition().y >= 4 && !is_paused)
				player2.move(0, -paddleSpeed * deltaTime);
			else if (Keyboard::isKeyPressed(Keyboard::Down) && player2.getPosition().y <= 252 - paddleSize.y)
				player2.move(0, paddleSpeed * deltaTime);


			//Bot Inteligence





			//








			float factor = ballSpeed * deltaTime;

			if (!is_paused)
				ball.move(std::cos(ballAngle) * factor, std::sin(ballAngle) * factor);

			if (ball.getPosition().y - ballRadius < 2)
			{
				pipSound.play();
				ballAngle = -ballAngle;
				ball.setPosition(ball.getPosition().x, ballRadius + 2);
			}
			if (ball.getPosition().y + ballRadius > 254)
			{
				pipSound.play();
				ballAngle = -ballAngle;
				ball.setPosition(ball.getPosition().x, 254 - ballRadius);
			}
			if (ball.getPosition().x - ballRadius < 2)
			{
				ball.setPosition(256, 128);
				punchSound.play();
				score1--;
				boomAnimation = true;
				animationStart = animationClock.getElapsedTime().asSeconds();
				boom.setPosition(0, 0);
				ballSpeed = 0;
				ballExist = false;
			}
			if (ball.getPosition().x + ballRadius > 510)
			{
				ball.setPosition(256, 128);
				punchSound.play();
				score2--;
				boomAnimation = true;
				animationStart = animationClock.getElapsedTime().asSeconds();
				boom.setPosition(256, 0);
				ballSpeed = 0;
				ballExist = false;
			}

			if (ball.getPosition().x - ballRadius < player1.getPosition().x + paddleSize.x &&
				ball.getPosition().x + ballRadius > player1.getPosition().x &&
				ball.getPosition().y - ballRadius <= player1.getPosition().y + paddleSize.y &&
				ball.getPosition().y + ballRadius >= player1.getPosition().y)
			{
				ballSpeed += 5;
				pipSound.play();
				if (ball.getPosition().y > (player1.getPosition().y + (paddleSize.y / 2)))
					ballAngle = pi - ballAngle + 10 * pi / 180;
				else
					ballAngle = pi - ballAngle - 10 * pi / 180;
				ball.setPosition(player1.getPosition().x + paddleSize.x + ballRadius + 2, ball.getPosition().y);
			}

			if (ball.getPosition().x + ballRadius > player2.getPosition().x &&
				ball.getPosition().x - ballRadius < player2.getPosition().x + paddleSize.x &&
				ball.getPosition().y - ballRadius <= player2.getPosition().y + paddleSize.y &&
				ball.getPosition().y + ballRadius >= player2.getPosition().y)
			{
				ballSpeed += 5;
				pipSound.play();
				if (ball.getPosition().y > (player2.getPosition().y + (paddleSize.y / 2)))
					ballAngle = pi - ballAngle + 10 * pi / 180;
				else
					ballAngle = pi - ballAngle - 10 * pi / 180;
				ball.setPosition(player2.getPosition().x - ballRadius - 2, ball.getPosition().y);
			}
		}
		else
		{
			while (window.pollEvent(event))
			{
				switch (event.type)
				{
				case Event::Closed:
					window.close();
					break;
				case Event::KeyReleased:
					if (event.key.code == Keyboard::Escape || (event.key.code == Keyboard::Enter && option == CONTINUE))
					{
						player1.setOutlineColor(Color(255, 255, 255, 255));
						player2.setOutlineColor(Color(255, 255, 255, 255));
						ball.setOutlineColor(Color(255, 255, 255, 255));
						outline.setOutlineColor(Color(255, 255, 255, 255));
						separator.setFillColor(Color(255, 255, 255, 255));
						player1Score.setFillColor(Color(255, 255, 255, 32));
						player2Score.setFillColor(Color(255, 255, 255, 32));
						is_paused = false;
						pauseSound.play();
					}
					else if (event.key.code == Keyboard::Enter && option == EXITP)
					{
						window.close();
					}
					break;
				case Event::KeyPressed:
					if (event.key.code == sf::Keyboard::Up)
					{
						if (option == CONTINUE)
						{
							pipSound.play();
							option = EXITP;
							cursor.setPosition(185, 176);
						}
						else
						{
							pipSound.play();
							option = CONTINUE;
							cursor.setPosition(120, 132);
						}
					}
					else if (event.key.code == sf::Keyboard::Down)
					{
						if (option == CONTINUE)
						{
							pipSound.play();
							option = EXITP;
							cursor.setPosition(185, 176);
						}
						else
						{
							pipSound.play();
							option = CONTINUE;
							cursor.setPosition(120, 132);
						}
					}
					break;
				}
			}
			window.draw(pauseText1);
			window.draw(pauseText2);
			window.draw(cursor);
		}
		window.draw(outline);
		for (int i = 0; i < 10; i++)
		{
			window.draw(separator);
			separator.setPosition(255, 15 + (24 * i));
		}
		
		window.draw(player1);
		window.draw(player2);
		player1Score.setString(std::to_string(score1));
		player2Score.setString(std::to_string(score2));
		if (score1 > 9)
			player1Score.setPosition(128, 100);
		else
			player1Score.setPosition(178, 100);
		if (score2 > 9)
			player2Score.setPosition(344, 100);
		else
			player2Score.setPosition(384, 100);
		window.draw(player1Score);
		window.draw(player2Score);
		//window.draw(pauseText1);

		if(ballExist)
			window.draw(ball);

		if (boomAnimation)
		{
			window.draw(boom);
			if (fade > 2)
				fade -= 400 * deltaTime;
			if (animationClock.getElapsedTime().asSeconds() >= animationStart + 0.8)
			{
				boomAnimation = false;
				ballExist = true;
				ball.setPosition(256, 128);
				do
					ballAngle = (std::rand() % 360) * 2 * pi / 360;
				while (std::abs(std::cos(ballAngle)) < 0.7f);
				ballSpeed = 200;
				boom.setPosition(512, 512);
				fade = 255;
			}
			boom.setFillColor(Color(225, 225, 225, fade));
		}

		window.display();
	}
	return 0;
}