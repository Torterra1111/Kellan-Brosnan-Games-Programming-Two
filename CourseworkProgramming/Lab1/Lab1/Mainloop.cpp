#include "Mainloop.h"
#include <iostream>
#include <string>

Mainloop::Mainloop()
{
	_gameState = GameState::PLAY; //When you begin the game, the gamestate is playing
	Display* _gameDisplay = new Display(); //Create the initial display
}

Mainloop::~Mainloop()
{
}

void Mainloop::run()
{
	MakeSystems(); //Initilise everything
	gameLoop(); //Begin the main game loop
} 

void Mainloop::MakeSystems() //init
{
	_gameDisplay.MakeDisplay();
	
	meshArrow.LoadModel("..\\res\\arrow.obj"); //Arrow model
	meshMonkey.LoadModel("..\\res\\monkey3.obj"); //Monkey model
	meshShield.LoadModel("..\\res\\shield.obj"); //Shield model
	myCamera.makeCamera(glm::vec3(0, 0, -5), 70.0f, (float)_gameDisplay.getWidth() / _gameDisplay.getHeight(), 0.01f, 1000.0f); //Creating the camera with the MVP
	
	shader.Create("..\\res\\shader"); //new shader
	
	BrickTex.Create("..\\res\\bricks.jpg"); //load texture for the BRICK
	WaterTex.Create("..\\res\\water.jpg"); //load texture  for the WATER
	SpongeTex.Create("..\\res\\sponge.jpg"); //load texture for the SPACE
	
	backGroundMusic = AudioSource.loadSound("..\\res\\background.wav"); //Music
	hitsound = AudioSource.loadSound("..\\res\\hitsound.wav"); //Music
	
	dospawn = true; //Spawnning for the arrow

}

void Mainloop::gameLoop()
{
	while (_gameState != GameState::EXIT) //while the gamestate isn't in exit. run the main gameloop
	{
		TakeInput();
		update();
		drawGame();
		Playmusic(backGroundMusic, glm::vec3(0.0f, 0.0f, 0.0f));
	}
}

void Mainloop::TakeInput()
{

	while(SDL_PollEvent(&evnt)) //Taking input
	{
		switch (evnt.type) //taking event status
		{
		case SDL_QUIT:
			_gameState = GameState::EXIT; //gamnestate = exit
			break;
		}	//User presses a key
			if (evnt.type == SDL_KEYDOWN)
			{
				switch (evnt.key.keysym.sym) //Key presses
				{
				
				case SDLK_UP: //If the player presses UP, make the models point up
					playerturn = 3.1f;
					playerRotx = -4.0f;
					playerRoty =  0.0f;
					
					shieldPosx = 0;
					shieldpoxY = -10;
					break;

				case SDLK_DOWN: //If the player presses UP, make the models point down
					playerturn = 0.0f;
					playerRotx = 4.0f;
					playerRoty = 0.0f;

					shieldPosx = 0;
					shieldpoxY = 10;
					break;

				case SDLK_LEFT: //If the player presses UP, make the models point left
					playerturn = 4.7f;
					playerRotx = -3.0f;
					playerRoty = -1.0f;

					shieldPosx = -10;
					shieldpoxY = 0;
					break;

				case SDLK_RIGHT: //If the player presses UP, make the models point right
					playerturn = 1.6f;
					playerRotx = 3.0f;
					playerRoty = 1.0f;
					
					shieldPosx = 10;
					shieldpoxY = 0;
					break;
				}
			}
		
		
	}
}

bool Mainloop::Collision(glm::vec3 m1Pos, float m1Rad, glm::vec3 m2Pos, float m2Rad) //Colistion check
{
	float dist = glm::sqrt((m2Pos.x - m1Pos.x)*(m2Pos.x - m1Pos.x) + (m2Pos.y - m1Pos.y)*(m2Pos.y - m1Pos.y) + (m2Pos.z - m1Pos.z)*(m2Pos.z - m1Pos.z)); //Disance between the two

	if ((dist) < (m1Rad + m2Rad)) //if the distance is smaller than the combined radius, must be colision
	{
		return true; 
	}
	else //if not. no colision
	{
		return false;
	}
}

void Mainloop::update()
{
	
	if (dospawn)
	{
		Movespeed = Movespeed + 0.0005;
		score += 1;
		std::cout << score;
		std::cout << " Is your current score!" << endl;
		int spwn = rand() % 4 + 1;
		switch (spwn) //choosing a random point for the mesh to be spawned
		{
			case 1: //LEFT
				arrowx = 3.0f;
				arrowy = 0.0f;
				arrowrot = 0;
				arrowmvx = -0.01 + (-Movespeed);
				arrowmvy = 0;
				dospawn = false;
				break;

		
			case 2: // RIGHT
				arrowx = -3.0f;
				arrowy = 0.0f;
				arrowrot = 3.1;
				arrowmvx = 0.01 + Movespeed;
				arrowmvy = 0;
				dospawn = false;
				break;
			
			case 3: //UP
		
				arrowx = 0.0f;
				arrowy = 3.0f;
				arrowrot = 4.7f;
				arrowmvx = 0.0;
				arrowmvy = -0.01 + (-Movespeed);
				dospawn = false;
				break;
			
			case 4: //DOWN
		
				arrowx = 0.0f;
				arrowy = -3.0f;
				arrowrot = 1.55;
				arrowmvx = 0.0;
				arrowmvy = 0.01 + Movespeed;
				dospawn = false;
				break;
		}
		
	}
	

	//Arrow
	Arrow.SetPos(glm::vec3(arrowx, arrowy, -1.0)); //Position in X,Y,Z
	Arrow.SetRot(glm::vec3(4.0, 0.0, arrowrot));  //Rotation in X,Y,Z  AS IN  /\ X , > < Y, Z O.
	Arrow.SetScale(glm::vec3(sinf(4), sinf(4), sinf(4))); //Scaleing
	meshArrow.UpdatePos(*Arrow.GetPos(), 0.10f); //colision hitbox

	//Monkey
	Monkey.SetPos(glm::vec3(0.0, 0.0, -1.0)); //Position in X,Y,Z
	Monkey.SetRot(glm::vec3(playerRotx, playerRoty, playerturn));  //X,Y,Z  AS IN  /\ X , > < Y, Z O.
	Monkey.SetScale(glm::vec3(sinf(3.5), sinf(3.5), sinf(3.5)));//Scaleing
	meshMonkey.UpdatePos(*Monkey.GetPos(), 0.30f); //colision hitbox

	//Shield
	Shield.SetPos(glm::vec3(sinf(shieldPosx), sinf(shieldpoxY), sinf(-1.0)));
	Shield.SetRot(glm::vec3(playerRotx, playerRoty, playerturn));  //X,Y,Z  AS IN  /\ X , > < Y, Z O.
	Shield.SetScale(glm::vec3(sinf(4), sinf(4), sinf(4)));
	meshShield.UpdatePos(*Shield.GetPos(), 0.25f); //colision hitbox

	arrowx += arrowmvx; //always moving the arrow
	arrowy += arrowmvy;//always moving the arrow

	
	if (Collision(meshMonkey.ReturnPos(),meshMonkey.ReturnRad(),meshArrow.ReturnPos(),meshArrow.ReturnRad())) //If the arrow touches the monkey, Game O V E R
	{
		_gameState = GameState::EXIT; 
	}
	
	if (Collision(meshShield.ReturnPos(),meshShield.ReturnRad(),meshArrow.ReturnPos(),meshArrow.ReturnRad())) //if the shield touches the arrow, block and move arrow
	{
		AudioSource.playSound(hitsound, glm::vec3(0.0f, 0.0f, 0.0f)); //play the "damn" music
		dospawn = true;
	}

	}

void Mainloop::drawGame()
{
	_gameDisplay.NukeDisplay(0.0f, 0.0f, 0.0f, 1.0f); //clear display
	shader.Bind(); //bind shader for use
	
	//Arrow 
	shader.Update(Arrow, myCamera); //Update the shader with the new infomation
	SpongeTex.Bind(0); //bind the same texture
	meshArrow.Draw(); //draw it

	//Monkey
	shader.Update(Monkey, myCamera); //Update the shader with the new infomation
	WaterTex.Bind(0); //bind the same texture
	meshMonkey.Draw(); //draw it

	//Shield
	shader.Update(Shield, myCamera); //Update the shader with the new infomation
	BrickTex.Bind(0); //bind the same texture
	meshShield.Draw(); //draw it

	glEnableClientState(GL_COLOR_ARRAY);
	glEnd();

	_gameDisplay.swapBuff(); //change buffer 
}

void Mainloop::Playmusic(unsigned int source, glm::vec3 pos) //playing the music
{
	alGetSourcei(source, AL_SOURCE_STATE, &state);

	if (AL_PLAYING != state) //if it isnt already playing
	{
		AudioSource.playSound(source, pos); //play the "damn" music
	}
}
