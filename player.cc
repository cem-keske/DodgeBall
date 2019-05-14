/**
 * file: player.cc
 * 
 * authors:	Cem Keske
 * 			Emre Yazici
 * 
 */
#include "player.h"

/// ===== PLAYER ===== ///


// ===== Constructor =====

Player::Player(double x,double y, Length radius, Counter lives, Counter cooldown) 
			   : body_({x,y}, radius), lives_(lives), cooldown_(cooldown) {}
			   	 
			   	   
// ===== Accessors and manipulators =====

Counter Player::lives() const {return lives_;}

Counter Player::cooldown() const {return cooldown_;}

const Coordinate& Player::position() const {return body_.center();}

const Circle& Player::body() const {return body_;} 

const Vector& Player::direction() const {return direction_;}

const Player* Player::target() const {return target_;}

void Player::lives(Counter lives){
	lives_ = lives;
};

void Player::cooldown(Counter cooldown){
	cooldown_ = cooldown;
};

void Player::position(const Coordinate& position) {
	body_.center(position);
}

void Player::direction(const Vector& direction) {
	if(direction.length() == 1.)
		direction_ = direction;
	else
		direction_= direction.get_unit();
}

void Player::target(const Player* target) {
	target_ = target;
}


// ===== Methods =====void move(const Vector&);

void Player::move(const Vector& move_vec) {
	body_.center((position() + move_vec).pointed());
}

