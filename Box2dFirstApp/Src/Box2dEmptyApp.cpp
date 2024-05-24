#include "Game.h"
#include <tchar.h>

using namespace sf;

class Bloque
{

private:
	// Sprite y su imagen la para representación gráfica
	Sprite *_sprite;
	Texture  *_image;
	RenderWindow *wnd;
	// Body para representación física
	b2Body* _body;
	//...
public:
	Bloque(b2World* _world , RenderWindow *_wnd)
	{
		// Guardamos puntero a ventana para dibujar luego
		wnd = _wnd;
		_image = new Texture();
		_image->loadFromFile("rcircle.png");
		// Cargamos el sprite
		_sprite= new Sprite(*_image);

		// Definimos el body y lo creamos
		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.position.Set(100.0f, 0.0f);
		_body = _world->CreateBody(&bodyDef);

		// Creamos su figura de colisión
		// en este caso suponemos que la figura de
		// colision es una caja cuadrada
		b2PolygonShape dynamicBox;
		dynamicBox.SetAsBox(20.0f, 20.0f);

		// Creamos el fixture, le seteamos
		// la figura de colision
		// y agregamos el fixture al body
		b2FixtureDef fixtureDef;
		fixtureDef.shape = &dynamicBox;
		fixtureDef.density = 10.0f;
		fixtureDef.friction = 0.8f;
		fixtureDef.restitution = 0.1f;

		_body->CreateFixture(&fixtureDef);

	}

	// Metodo que posiciona el sprites
	// segun la posicion del body
	void ActualizarPosiciones()
	{
		b2Vec2 pos = _body->GetPosition();
		_sprite->setPosition(pos.x, pos.y);
	}

	// Metodo que dibuja el sprite
	void Dibujar()
	{
		wnd->draw(*_sprite);
	}

	// Otros metodos
	// relacionados al personaje
};


int _tmain(int argc, _TCHAR* argv[])
{
	Game *Juego;
	Juego = new Game(800, 600, "Esqueleto de Aplicación - MAVII");
	Juego->Loop();

	return 0;
}