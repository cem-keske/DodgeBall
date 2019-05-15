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
		Vector direction_;
		const Player *target_;
		
		bool target_seen_;
	
	public:	
	
		// ===== Constructors =====
		
		Player(double x, double y, Length r, Counter lives, Counter cooldown);
		
		// ===== Accessors =====
		
		Counter lives() const;
		Counter cooldown() const;

		const Coordinate& position() const;
		const Circle& body() const; 
		const Vector& direction() const;
		const Player* target() const;
		
		bool target_seen() const;

		// ===== Manipulators =====
		
		void lives(Counter);
		void cooldown(Counter);
		void direction(const Vector&);
		void position(const Coordinate&);
		void target(const Player*);
		
		void target_seen(bool);
				
		// ===== Methods =====
		
		void move(const Vector&);
		void cool_down(Counter);
		void take_life();
};


#endif
