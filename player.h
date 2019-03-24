/**
 * file: player.h
 * 
 * authors:	Cem Keske
 * 			Emre Yazici
 */
#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED
#include "tools.h"

/// PLAYER ///

class Player{
	
	private: 
	
		Circle body_;
		Counter lives_;
		Counter cooldown_;
	
	public:	
	
		// ===== Constructors =====
		
		Player(double x, double y, Length r, Counter lives, Counter cooldown);
		
		// ===== Accessors =====
		
		const Coordinate& position() const;
		const Circle& body() const; 
		Counter lives() const;
		Counter cooldown() const;

		// ===== Manipulators =====

		void position(const Coordinate&);
		void lives(Counter);
		void cooldown(Counter);
				
		// ===== Methods =====
		
		void move(); //To implement in further versions...
};


#endif
