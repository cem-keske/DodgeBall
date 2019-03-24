/**
 * file: ball.cc
 * 
 * authors:	Cem Keske
 * 			Emre Yazici
 */
#include "ball.h"

/// ===== BALL ===== ///


// ===== Constructors =====

Ball::Ball(Vector const &direction, Length radius) : direction_(direction), 
													 geometry_(radius) {}										

Ball::Ball(Vector const &direction, Length radius, Coordinate const& position) 
		   : direction_(direction), geometry_(position, radius) {}
		   
Ball::Ball(Angle angle, double x, double y, Length radius)
		   : direction_(angle), geometry_({x,y},radius) {}


// ===== Accessors & Manipulators =====

const Coordinate& Ball::position() const {return geometry_.center();}

Length Ball::radius() const {return geometry_.radius();}

const Vector& Ball::direction() const {return direction_;}

const Circle& Ball::geometry() const {return geometry_;}

void Ball::geometry(Circle const& circle){geometry_ = circle;}

void Ball::position(Coordinate const &position) {geometry_.center(position);}

void Ball::radius(Length radius) {geometry_.radius(radius);}

void Ball::direction(Vector const &direction) {direction_ = direction;}
