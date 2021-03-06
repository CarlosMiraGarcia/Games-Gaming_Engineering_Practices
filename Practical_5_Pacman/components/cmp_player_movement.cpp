#include "cmp_player_movement.h"
#include "..\Pacman.h"
#include "..\game.h"
#include "cmp_pickup.h"

using namespace std;
using namespace sf;

PlayerMovementComponent::PlayerMovementComponent(Entity* p)
	: ActorMovementComponent(p) {
	_speed = 200.f;
	offset = 6.f;
}

void PlayerMovementComponent::update(double dt) {
	Time timeElapsed = powerUpTimer.getElapsedTime();

	if (_parent->isPowerUp() && _isTimerFinished) {
		powerUpTimer.restart();
		setSpeed(getSpeed() * 2);
		_isTimerFinished = false;
		timeElapsed = powerUpTimer.getElapsedTime();
	}

	if (_parent->isPowerUp() && timeElapsed.asSeconds() > 8) {
		setSpeed(getSpeed() / 2);
		powerUpTimer.restart();
		_isTimerFinished = true;
		_parent->setPowerUp(false);
	}

	Vector2f parent_pos = _parent->getPosition();
	//Left
	if (Keyboard::isKeyPressed(controls[0])) {
		if (validMove(parent_pos + Vector2f(-_playerSize, -offset)) &&
			validMove(parent_pos + Vector2f(-_playerSize, offset))) {
			_direction = Vector2f(-1.f, 0.f);
		}
	}
	//Right
	if (Keyboard::isKeyPressed(controls[1])) {
		if (validMove(parent_pos + Vector2f(_playerSize, -offset)) &&
			validMove(parent_pos + Vector2f(_playerSize, offset))) {
			_direction = Vector2f(1.f, 0.f);
		}
	}
	//Up
	if (Keyboard::isKeyPressed(controls[2])) {
		if (validMove(parent_pos + Vector2f(-offset, -_playerSize)) &&
			validMove(parent_pos + Vector2f(offset, -_playerSize))) {
			_direction = Vector2f(0.f, -1.f);
		}
	}
	//Down
	if (Keyboard::isKeyPressed(controls[3])) {
		if (validMove(parent_pos + Vector2f(-offset, _playerSize)) &&
			validMove(parent_pos + Vector2f(offset, _playerSize))) {
			_direction = Vector2f(0.f, 1.f);
		}
	}

	ActorMovementComponent::move(_direction * (float)(_speed * dt));
	_direction = Vector2f(0.f, 0.f);
}