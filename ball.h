/**
 * file: ball.h
 * 
 * authors:	Cem Keske
 * 			Emre Yazici
 */
#ifndef BALL_H_INCLUDED
#define BALL_H_INCLUDED
#include "tools.h"


class Ball {
	private:	
		
		Vector direction_;		//this will also be the velocity of the movement...
		Circle geometry_;		
	
	public:
	
		// ===== Constructors =====
		
		Ball(Vector const& direction, Length radius);
		Ball(Vector const& direction, Length radius, Coordinate const& position);
		Ball(Angle, double x, double y, Length radius);
		
		// ===== Accessors =====
		
		const Coordinate& position() const;
		Length radius() const;
		const Vector& direction() const;
		const Circle& geometry() const;
		
		// ===== Manipulators =====
		
		void geometry(Circle const&);
		void position(Coordinate const&);
		void radius(Length);
		void direction(Vector const&);
		
		// ===== Methods =====
		
		void move(Vector const&);
};

#endif
