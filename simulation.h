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
#include "player.h"
#include "map.h"
#include "ball.h"
#include <string>
#include <vector>
#include <unordered_map>

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

		Map map_;
		std::vector<Player> players_;
		std::vector<Ball> balls_;

	
	public:
		// ===== Constructor =====
		
		Simulation(std::unordered_map<std::string, bool> const&, 
				   std::vector<std::string> const&);
		
		// ===== Public Methods =====
		void nb_cells(size_t);
		bool initialise_player(double, double, Counter, Counter);
		bool initialise_ball(double, double, Angle);
		void initialise_dimensions(size_t);
	
		//signed input to test negative values. counter is for error report.
		bool initialise_obstacle(int, int, Counter);	
		
		bool test_collisions();

	private:

		bool detect_collision(Player const&, Player const&);
		bool detect_collision(Player const&, Ball const&);
		bool detect_collision(Ball const&, Ball const&);
		bool detect_collision(Player const&, Map const&);
		bool test_center_position(double x,double y) const;
		bool detect_all_ball_player_collisions() const;
		bool detect_all_ball_obstacle_collisions() const;		
		bool detect_initial_player_collisions() const ;
		bool detect_initial_ball_collisions() const ;

};
	

/// READER ///
/**
 * This class will be useful when reading a data from files.
 * It can be used to read specific datas and initialize the current simulation.
 */

enum ReaderState {
	BEGIN,
	READ_NB_CELLS,
	READ_PLAYERS,
	READ_OBSTACLES,
	READ_BALLS,
	SUCCESS
};


class Reader {
	
	private:
		ReaderState reader_state_;
		
	public:
		// ===== Constructor =====

		Reader(ReaderState);

		// ===== Accessors =====
		
		ReaderState reader_state() const;

		// ===== Manipulators
		
		void reader_state(ReaderState);

		// ===== Public Methods =====
		
		bool import_file(std::string const&, Simulation&);
		bool read_file(std::ifstream&, Simulation&,bool = false);
		
	
	private:
		// ===== Private Static Functions =====
		
		/// Reads the next line in the file (provided by "istream") ignoring comments
		static bool next_data_line(std::ifstream&, std::string&);
		static bool read_nb_cells(std::ifstream&, Simulation&);
		static bool read_players(std::ifstream&, Simulation&);
		static bool read_obstacles(std::ifstream&, Simulation&);
		static bool read_balls(std::ifstream&, Simulation&);
		static void finalise_reading(ReaderState &actual_state);
		static void print_error_state(ReaderState); /// for debugging

		// ===== Private Methods =====
	
	
};



#endif
