/**
 * file: player.cc
 * 
 * authors:	Cem Keske
 * 			Emre Yazici
 */
#include "player.h"

/// ===== PLAYER ===== ///

// ===== Constructor =====

Player::Player(double x,double y, Length radius, Counter lives, Counter cooldown) 
			   : body_({x,y}, radius), lives_(lives), cooldown_(cooldown) {}
			   	   
// ===== Accessors and manipulators =====

const Coordinate& Player::position() const {return body_.center();}

const Circle& Player::body() const {return body_;} 

Counter Player::lives() const {return lives_;}

Counter Player::cooldown() const {return cooldown_;}

void Player::position(const Coordinate& position) {
	body_.center(position);
}

void Player::lives(Counter lives){
	lives_ = lives;
};

void Player::cooldown(Counter cooldown){
	cooldown_ = cooldown;
};
