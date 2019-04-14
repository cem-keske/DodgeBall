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
 */
enum Predefined_Color {
	RED,
	ORANGE,
	YELLOW,
	GREEN
};

/**
 * Enumeration of possiible simulation states.
 */
enum Simulation_State {
	NO_GAME,
	GAME_READY,
	PLAYER_TRAPPED,
	GAME_OVER
};


/**
 * Tuple constains the circle corresponding to player body, angle of cooldown arc
 * and the preferred color for the player (according to lives left)
 */
typedef std::vector<std::tuple<Circle, Angle, Predefined_Color>> vec_player_graphics;
typedef std::vector<Circle>		vec_ball_bodies;
typedef std::vector<Rectangle>	vec_obstacle_bodies;



class Simulation; //forward declaration necessary
/**
 * This is a helper class to make it possible to move the declaration of Simulation 
 * to .cc file. This way we do not need to include (and thus export) the inner modules.
 * Using this class, we ensure encapsulation during communication with gui and projet.
 * Simulator will provide all necessary links between gui and simulation.
 */
class Simulator{

	public:
		
		/// sets execution parameters to be used for all simulation objects
		static void exec_parameters(const std::unordered_map<std::string, bool>&);
		
		/// corresponding accessor
		static const std::unordered_map<std::string, bool>& exec_parameters();
	
		/**
		 * Creates a new simulation. If not successful, the previous state of the
		 * game is conserved. 
		 */
		static bool create_simulation(std::vector<std::string> const& io_files);							  
		
		static bool import_file(const std::string&);
		static bool empty();
		static Simulation_State active_simulation_state();
		
		/**
		 * Accessors to simulation's geometry 
		 */							  			
		static const vec_player_graphics& fetch_player_graphics(); 
		static const vec_ball_bodies& fetch_ball_bodies();
		static const vec_obstacle_bodies& fetch_obstacle_bodies();
		
		static void update_active_sim(double delta_t);
		static void update_all_sims(double delta_t);
		
		static void save_simulation(const std::string&);
	
	private:
		
		/**
		 * Static wrapper functions containing data fields for internal use of 
		 * simulator.
		 * 
		 * Static fields are wrapped to ensure proper initialization.(we encountered
		 * buggy behaviour otherwise as there are no default constructor for them)
		 * 
		 * current_sim_index is also wrapped for style reasons to make the code uniform
		 */
		static size_t& current_sim_index();
		static std::unordered_map<std::string, bool>& execution_parameters();
		static std::vector<Simulation>& active_sims();

};	

#endif
