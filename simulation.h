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
 * We have decided to use shared_ptr to avoid unnecessary copying of relatively 
 * heavier Circle and Rectangle objects.
 */
typedef std::vector<std::pair<std::shared_ptr<const Circle>,
							  Predefined_Colors>>  	vec_player_graphics;
typedef std::vector<std::shared_ptr<const Circle>> 	vec_ball_bodies;
typedef std::vector<std::shared_ptr<Rectangle>>   	vec_obstacle_bodies;


/**
 * Enumeration of colors that will be used to determine player colors with respect to
 * lives left. Choice of color for other objects are left to the user.
 * */
enum class Predefined_Colors {
	RED,
	ORANGE,
	YELLOW,
	GREEN
};



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
		static const vec_player_graphics& get_player_bodies(); 
		static const vec_ball_bodies& get_ball_bodies();
		static const vec_obstacle_bodies& get_obstacle_bodies();
};	

#endif
