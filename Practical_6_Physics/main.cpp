#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>
#include "Renderer.h"

using namespace sf;
using namespace std;

b2World* world;
std::vector<b2Body*> bodiesBox;
std::vector<b2Body*> bodiesWall;
std::vector<RectangleShape*> spritesBox;
std::vector<RectangleShape*> spritesWall;
sf::Clock colourClock;
sf::Clock sizeClock;

float gameWidth = 800.f;
float gameHeight = 600.f;

// 1 sfml unit = 30 physics units
const float physics_scale = 30.0f;
// inverse of physics_scale, useful for calculations
const float physics_scale_inv = 1.0f / physics_scale;
// Magic numbers for accuracy of physics simulation
const int32 velocityIterations = 6;
const int32 positionIterations = 2;

//Convert from b2Vec2 to a Vector2f
inline const Vector2f bv2_to_sv2(const b2Vec2& in) {
	return Vector2f(in.x * physics_scale, (in.y * physics_scale));
}
//Convert from Vector2f to a b2Vec2
inline const b2Vec2 sv2_to_bv2(const Vector2f& in) {
	return b2Vec2(in.x * physics_scale_inv, (in.y * physics_scale_inv));
}
//Convert from Screenspce.y to physics.y
inline const Vector2f invert_height(const Vector2f& in) {
	return Vector2f(in.x, gameHeight - in.y);
}


//Create a Box3d body with a box fixture
b2Body* CreatePhysicsBox(b2World& World, const bool dynamic, const Vector2f& position, const Vector2f& size) {
	b2BodyDef BodyDef;
	//Is Dynamic(moving), or static(Stationary)
	BodyDef.type = dynamic ? b2_dynamicBody : b2_staticBody;
	BodyDef.position = sv2_to_bv2(position);
	//Create the body
	b2Body* body = World.CreateBody(&BodyDef);

	//Create the fixture shape
	b2PolygonShape Shape;
	Shape.SetAsBox(sv2_to_bv2(size).x * 0.5f, sv2_to_bv2(size).y * 0.5f);
	b2FixtureDef FixtureDef;
	//Fixture properties
	FixtureDef.density = dynamic ? 10.f : 0.f;
	FixtureDef.friction = dynamic ? 0.8f : 1.f;
	FixtureDef.restitution = 1.0;
	FixtureDef.shape = &Shape;
	//Add to body
	body->CreateFixture(&FixtureDef);
	return body;
}

// Create a Box2d body with a box fixture, from a sfml::RectangleShape
b2Body* CreatePhysicsBox(b2World& world, const bool dynamic, const RectangleShape& rs) {
	return CreatePhysicsBox(world, dynamic, rs.getPosition(), rs.getSize());
}

void init() {
	const b2Vec2 gravity(0.0f, -10.0f);

	// Construct a world, which holds and simulates the physics bodies.
	world = new b2World(gravity);

	// Wall Dimensions
	Vector2f walls[] = {
		// Top
		Vector2f(gameWidth * .5f, 5.f), Vector2f(gameWidth, 10.f),
		// Bottom
		Vector2f(gameWidth * .5f, gameHeight - 5.f), Vector2f(gameWidth, 10.f),
		// left
		Vector2f(5.f, gameHeight * .5f), Vector2f(10.f, gameHeight),
		// right
		Vector2f(gameWidth - 5.f, gameHeight * .5f), Vector2f(10.f, gameHeight)
	};

	//FIX THIS
	// Build Walls
	for (int i = 0; i < 7; i += 2) {
		// Create SFML shapes for each wall
		auto s = new RectangleShape();
		s->setPosition(walls[i]);
		s->setSize(walls[i + 1]);
		s->setOrigin(walls[i + 1] * .5f);
		s->setFillColor(Color::White);
		spritesWall.push_back(s);

		// Create a static physics body for the wall
		auto b = CreatePhysicsBox(*world, false, *s);
		bodiesWall.push_back(b);
	}

	// Create Boxes
	for (int i = 1; i < 11; ++i) {
		// Create SFML shapes for each box
		auto s = new RectangleShape();
		s->setPosition(Vector2f(i * (gameWidth / 12.f), gameHeight * .7f));
		s->setSize(Vector2f(50.0f, 50.0f));
		s->setOrigin(Vector2f(25.0f, 25.0f));
		s->setFillColor(Color::White);
		spritesBox.push_back(s);

		// Create a dynamic physics body for the box
		auto b = CreatePhysicsBox(*world, true, *s);
		// Give the box a spin
		b->ApplyAngularImpulse(5.0f, true);
		bodiesBox.push_back(b);

	}
}

void Update() {
	static sf::Clock clock;
	float dt = clock.restart().asSeconds();
	//Check colour clock
	sf::Time elapsed1 = colourClock.getElapsedTime();
	sf::Time elapsed2 = sizeClock.getElapsedTime();
	srand(time(NULL));
	// Step Physics world by Dt (non-fixed timestep)
	world->Step(dt, velocityIterations, positionIterations);

	for (int i = 0; i < bodiesBox.size(); ++i) {
		// Sync Sprites to physics position
		spritesBox[i]->setPosition(invert_height(bv2_to_sv2(bodiesBox[i]->GetPosition())));
		// Sync Sprites to physics Rotation
		spritesBox[i]->setRotation((180 / b2_pi) * bodiesBox[i]->GetAngle());
		auto spriteColour = spritesBox[i]->getFillColor();


		if (elapsed1.asSeconds() > 0.5) {
			//Creating random numbers
			int randNum1 = (rand() % 3) + 1;
			int randNum2 = (rand() % 3) + 1;
			int randNum3 = (rand() % 3) + 1;
			int randNum4 = (rand() % 10) + 1;

			//This randomizes the size
			spritesBox[i]->setFillColor(sf::Color(spriteColour.r + dt * 100000 * randNum1, spriteColour.g + dt * 100000 * randNum2, spriteColour.b + dt * 100000 * randNum3));
			//This randomizes the size
			colourClock.restart();
		}

		//if (elapsed2.asSeconds() > 0.1) {
		//	//Creating random numbers
		//	static int randSize[] = { -1, 1 };
		//	int randNum5 = randSize[rand() % 2];
		//	spritesBox[i]->setScale(spritesBox[i]->getScale().x + 0.1 * randNum5, spritesBox[i]->getScale().y + 0.1 * randNum5);
		//	sizeClock.restart();
		//}
	}
}

void Render() {
	for (int i = 0; i < bodiesBox.size(); ++i) {
		Renderer::queue(spritesBox[i]);
	}
	for (int i = 0; i < bodiesWall.size(); ++i) {
		Renderer::queue(spritesWall[i]);
	}
	Renderer::render();
}

int main() {
	init();
	RenderWindow window(VideoMode(gameWidth, gameHeight), "Physics");
	Renderer::initialise(window);
	while (window.isOpen()) {
		window.clear();
		Update();
		Render();
		window.display();
	}
	return 0;
}