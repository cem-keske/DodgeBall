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
#include <tuple>

/**
 * Enumeration of colors that will be used to determine player colors with respect to
 * lives left. Choice of color for other objects are left to the user.
 * */
enum Predefined_Color : unsigned{
	RED,
	ORANGE,
	YELLOW,
	GREEN
};

/**
 * We have decided to use shared_ptr to avoid unnecessary copying of relatively 
 * heavier Circle and Rectangle objects.
 * 
 * Tuple constains the circle corresponding to player body, angle of cooldown arc
 * and the preferred color for the player (according to lives left)
 */
typedef std::vector<std::tuple<std::shared_ptr<const Circle>, Angle,
							   Predefined_Color>>  			vec_player_graphics;
typedef std::vector<std::shared_ptr<const Circle>> 			vec_ball_bodies;
typedef std::vector<std::shared_ptr<const Rectangle>>		vec_obstacle_bodies;




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
		static const std::unique_ptr<vec_player_graphics>& fetch_player_graphics(); 
		static const std::unique_ptr<vec_ball_bodies>& fetch_ball_bodies();
		static const std::unique_ptr<vec_obstacle_bodies>& fetch_obstacle_bodies();
};	

#endif
