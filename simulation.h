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


/**
 * This is a helper class to make it possible to move the declaration of Simulation 
 * to .cc file. This way we do not need to include (and thus export) the inner modules.
 * Using this class, we ensure encapsulation during communication with gui and projet.
 * Simulator will provide all necessary links between gui and simulation.
 */
class Simulator{
	
	public:
		/**
		 * Creates a new simulation. If not successful, the previous state of the
		 * game is conserved. 
		 */
		static void create_simulation(std::unordered_map<std::string, bool> const&
									  exec_parameters, 
									  std::vector<std::string> const& io_files);
		/**
		 * Accessors to simulation's geometry 
		 */							  		
		static const std::vector<std::pair<Circle, Color>>& get_player_bodies(); 
		static const std::vector<Circle>& get_ball_bodies();
		static const std::vector<Rectangle>& get_obstacle_bodies();
};	

#endif
