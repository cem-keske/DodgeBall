/**
 * file: simulation.h
 * 
 * authors:	Cem Keske
 * 			Emre Yazici
 * 
 */
#ifndef SIMULATION_H_INCLUDED
#define SIMULATION_H_INCLUDED

#include "tools.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

/// SIMULATION ///

class Simulation {	
	
	private:
		size_t nb_cells_;
		Length player_radius_;
		Length player_speed_;
		Length ball_radius_;
		Length ball_speed_;
		Length marge_jeu_;
		Length marge_lecture_;
		
		std::unordered_map<std::string, bool> execution_parameters_;
		
		bool success_;
	
	public:
		// ===== Constructor =====
		
		Simulation(std::unordered_map<std::string, bool> const&, 
				   std::vector<std::string> const&);
		
		// ===== Public Methods =====
		bool success() const;
		void nb_cells(size_t);
		bool initialise_player(double, double, Counter, Counter);
		bool initialise_ball(double, double, Angle);
		void initialise_dimensions(size_t);
	
		//signed input to test negative values. counter is for error report.
		bool initialise_obstacle(int, int, Counter);	
		
		bool test_collisions();
		
		bool save(const std::string &o_file_path) const;
		
	private:
		
		//bool detect_collision(Player const&, Player const&);
		//bool detect_collision(Player const&, Ball const&);
		//bool detect_collision(Ball const&, Ball const&);
		//bool detect_collision(Player const&, Map const&);
		
		bool test_center_position(double x,double y) const;
		bool detect_all_ball_player_collisions() const;
		bool detect_all_ball_obstacle_collisions() const;		
		bool detect_initial_player_collisions() const ;
		bool detect_initial_ball_collisions() const ;

};
	

#endif
