/*
TODO:
Change vector pairs to a class that returns magnitude
Convert Everything to AU
Figure out way to graph 
 >add zoom feature
 >display all metrics in units
 >mouse functionality
 >add menus for RK4 and other features like white background and trace
*/

#include <iostream>
#include <cmath>
#include <SFML/Graphics.hpp>
using namespace std;

//config data
bool runge = false;



//one pixel = 300k km
//one time unit = 1 s
const int WinX = 2*1920;
const int WinY = 2*1080;

const double SunX = WinX/2;
const double SunY = WinY/2;
//Gravitaional Constant in pix^3 kg^-1 s^-2
const double G = 2.497413*pow(10,-36);
//Mass of Sun in kg
const double Ms = 1.989*pow(10,30);
//Initial Velocity of Sattelite in pix/s
const double Vi = .000101;
//Initial Position of Sattelite in pix (x, 0)
const double Xi = 147099761/300000;
//Step size in seconds
const double h = 86400;
//framerate
const int framerate = 30;
//defines parameters of planet
class Planet {
	public:
		double posX,posY,velX,velY,r,a,ax,ay;
		void euler_step();
};
//defines Euler Method to update position
void Planet::euler_step(){
	r = pow(pow((posX-SunX),2) + pow((posY-SunY),2),.5);
	a = (G*Ms)/(r*r);
	ax = a * (SunX - posX)/r;
	ay = a * (SunY - posY)/r;
	velX += (ax*h);
	velY += (ay*h);
	posX += (velX*h);
	posY += (velY*h);
	
}


//assuming Msun >> Msat
int main(){
	freopen("output.txt", "w", stdout);
	//define Earth
	Planet E;
	E.posX = WinX/2 + Xi;
	E.posY = WinY/2;
	E.velX = 0;
	E.velY = 1*Vi;
	
	//SFML Window
	 // create the window
    sf::RenderWindow window(sf::VideoMode(WinX, WinY), "EEgrav");
	window.setFramerateLimit(framerate);
    // run the program as long as the window is open
	int time = 0;
	int rad = 50;
	//define sun visual
	sf::CircleShape Sun(rad);
	Sun.setFillColor(sf::Color(252, 212, 64));
	Sun.setPosition(sf::Vector2f(SunX,SunY));
	//define earth visual
	sf::CircleShape Earth(rad/10);
	Earth.setFillColor(sf::Color(0, 119, 190));
	Earth.setPosition(sf::Vector2f(SunX + Xi,SunY));
	//display text
	sf::Font font;
	
	//initialize distance text
	sf::Text text_distance;
	text_distance.setFont(font); 
	text_distance.setString("Distance: " + to_string(E.r));
	text_distance.setCharacterSize(64);
	text_distance.setFillColor(sf::Color::White);
	text_distance.setStyle(sf::Text::Bold);
	//initialize perihelion text
	sf::Text text_perihelion;
	text_perihelion.setFont(font); 
	text_perihelion.setString("Perihelion: " + to_string(E.r));
	text_perihelion.setCharacterSize(64);
	text_perihelion.setFillColor(sf::Color::White);
	text_perihelion.setStyle(sf::Text::Bold);
	text_perihelion.setPosition(sf::Vector2f(0,80));
	//initialize aphelion text
	sf::Text text_aphelion;
	text_aphelion.setFont(font); 
	text_aphelion.setString("Aphelion: " + to_string(E.r));
	text_aphelion.setCharacterSize(64);
	text_aphelion.setFillColor(sf::Color::White);
	text_aphelion.setStyle(sf::Text::Bold);
	text_aphelion.setPosition(sf::Vector2f(0,160));

	double aphelion = 0;
	double perihelion = pow(10,100);
	
if (!font.loadFromFile("arial.ttf"))
{
	cout << "font loading error"; 
}

    while (window.isOpen())
    {
		window.clear(sf::Color::Black);		

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
		cout << E.posX << " "<< E.posY <<endl;

		
		if(!runge){E.euler_step();}
		
		int pixX = (int) E.posX;
		int pixY = (int) E.posY;
		Earth.setPosition(sf::Vector2f(pixX,pixY));
		//get aphelion and perihelion
		
		if(E.r > aphelion){
			aphelion = E.r;
		}
		if(E.r < perihelion){
			perihelion = E.r;
		}
		
		text_distance.setString("Distance: " + to_string(E.r * 300000) + " km (" + to_string(E.r * 300000/149598000) + " AU)");
		text_aphelion.setString("Aphelion: " + to_string(aphelion * 300000) + " km ("+ to_string(aphelion * 300000/149598000) + " AU)");
		text_perihelion.setString("Perihelion: " + to_string(perihelion * 300000) + " km (" + to_string(perihelion * 300000/149598000) + " AU)");

		
		
        window.draw(Sun);
		window.draw(Earth);	
		window.draw(text_distance);		
		window.draw(text_aphelion);		
		window.draw(text_perihelion);		

        window.display();
    }

	return 0;
}