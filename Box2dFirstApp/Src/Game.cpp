#include "Game.h"
#include "Box2DHelper.h"
#include <iostream>
#include "b2ExSoftCircleBody.h"


using namespace sf;
using namespace std;

Game::Game(int ancho, int alto, std::string titulo)
{
	wnd = new RenderWindow(VideoMode(ancho, alto), titulo);
	wnd->setVisible(true);
	fps = 60;
	wnd->setFramerateLimit(fps);
	frameTime = 1.0f / fps;
	SetZoom();
	InitPhysics();
}

void Game::Loop()
{
	while(wnd->isOpen())
	{
		wnd->clear(clearColor);
		DoEvents();
		CheckCollitions();
		UpdatePhysics();
		DrawGame();
		wnd->display();
	}
}

void Game::UpdatePhysics()
{
	phyWorld->Step(frameTime, 8, 8);
	phyWorld->ClearForces();
	phyWorld->DebugDraw();
}

void Game::DrawGame()
{ 
	// Dibujamos el suelo
	sf::RectangleShape groundShape(sf::Vector2f(500, 5));
	groundShape.setFillColor(sf::Color::Red);
	groundShape.setPosition(0, 95);
	wnd->draw(groundShape);

	// Dibujamos las paredes
	sf::RectangleShape leftWallShape(sf::Vector2f(10, alto)); // Alto de la ventana
	leftWallShape.setFillColor(sf::Color::Blue);
	leftWallShape.setPosition(100, 0); // X = 100 para que comience donde termina el suelo
	wnd->draw(leftWallShape);

	sf::RectangleShape rightWallShape(sf::Vector2f(10, alto)); // Alto de la ventana
	rightWallShape.setFillColor(sf::Color::Cyan);
	rightWallShape.setPosition(90, 0); // X = 90 para que comience donde termina el suelo
	wnd->draw(rightWallShape);


	sf::RectangleShape upWallShape(sf::Vector2f(500, 5)); // Alto de la ventana
	upWallShape.setFillColor(sf::Color::Red);
	upWallShape.setPosition(0, 0); // X = 90 para que comience donde termina el suelo
	wnd->draw(upWallShape);

	// Dibujamos el cuerpo de control (círculo)
	sf::CircleShape controlShape(5);
	controlShape.setFillColor(sf::Color::Magenta);
	controlShape.setPosition(controlBody->GetPosition().x - 5, controlBody->GetPosition().y - 5);
	wnd->draw(controlShape);

	// Dibujar texto en la esquina superior izquierda
	sf::Font font;
	if (!font.loadFromFile("font.ttf")) {
		// Si la fuente no se pudo cargar, muestra un mensaje en la consola
		return;
	}
	sf::Text text;
	text.setFont(font);
	text.setString("MAVII");
	text.setCharacterSize(24); // Tamaño del texto
	text.setFillColor(sf::Color::Red);
	text.setPosition(5, 5); // Posición en la esquina superior izquierda
	
	// Verificar si la posición del texto está dentro de la ventana
	if (text.getPosition().x < 0 || text.getPosition().y < 0 ||
		text.getPosition().x + text.getLocalBounds().width > wnd->getSize().x ||
		text.getPosition().y + text.getLocalBounds().height > wnd->getSize().y) {
		std::cerr << "La posición del texto está fuera de la ventana" << std::endl;
		return;
	}
	wnd->draw(text);
}

void Game::DoEvents()
{
	Event evt;
	while(wnd->pollEvent(evt))
	{
		switch(evt.type)
		{
			case Event::Closed:
				wnd->close();
				break;
			case Event::MouseButtonPressed:
				b2Body *body=Box2DHelper::CreateTriangularDynamicBody(phyWorld,b2Vec2(0.0f, 0.0f), 10.0f, 1.0f, 4.0f, 0.1f);
				// Transformamos las coordenadas segun la vista activa
				Vector2f pos = wnd->mapPixelToCoords(Vector2i(evt.mouseButton.x, evt.mouseButton.y));
				body->SetTransform(b2Vec2(pos.x, pos.y), 0.0f);
				break;
		}
	}

	// Movemos el cuerpo

		
}

void Game::CheckCollitions()
{
	// Veremos mas adelante
}

// Definimos el area del mundo que veremos en nuestro juego
// Box2D tiene problemas para simular magnitudes muy grandes
void Game::SetZoom()
{
	View camara;
	// Posicion del view
	camara.setSize(100.0f, 100.0f);
	camara.setCenter(50.0f, 50.0f);
	wnd->setView(camara); //asignamos la camara
}

void Game::InitPhysics()
{
	// Inicializamos el mundo con la gravedad por defecto
	phyWorld = new b2World(b2Vec2(0.0f, 9.8f));

	//MyContactListener* l= new MyContactListener();
	//phyWorld->SetContactListener(l);
	// Creamos el renderer de debug y le seteamos las banderas para que dibuje TODO
	debugRender = new SFMLRenderer(wnd);
	debugRender->SetFlags(UINT_MAX);
	phyWorld->SetDebugDraw(debugRender);

	// Creamos un piso y paredes
	b2Body* groundBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 100, 10);
	groundBody->SetTransform(b2Vec2(50.0f, 100.0f), 0.0f);

	b2Body* leftWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 100);
	leftWallBody->SetTransform(b2Vec2(0.0f, 50.0f), 0.0f);

	b2Body* rightWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 100);
	rightWallBody->SetTransform(b2Vec2(100.0f, 50.0f), 0.0f);

	b2Body* upWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 100, 10);
	upWallBody->SetTransform(b2Vec2(50.0f, 0.0f), 0.0f);

	// Creamos un círculo que controlaremos con el teclado
	controlBody = Box2DHelper::CreateCircularDynamicBody(phyWorld, 5, 10.0f, 3.0f, 1);
	controlBody->SetTransform(b2Vec2(50.0f, 50.0f), 0.0f);

	controlBody->SetAwake(true);
	//controlBody->SetAngularVelocity(1);
	controlBody->SetLinearVelocity(b2Vec2(-20.0f, 10.0f));
	cout << controlBody->GetPosition().x << endl ;

	// Verifica el tipo de la junta

}

Game::~Game(void)
{ }