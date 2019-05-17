/**
 * file: simulation.cc
 * 
 * authors:	Cem Keske
 * 			Emre Yazici
 */
#include "simulation.h"
#include "error.h"
#include "define.h"
#include "player.h"
#include "map.h"
#include "ball.h"
#include "assert.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <cmath>
#include <algorithm>

typedef uint64_t Floyd_Dist;
typedef std::vector<std::vector<Floyd_Dist>> Floyd_Matrix;

/**
* This approximation works until we have 84 nbCells.
* If we have 85 cells we'll have 7225 steps in our longest road and
* 7225*10000 > 14142*(all diagonals in grid). In this case we'll have a false
* result. There's no oveflow since nbCells < 429496.
*/
static constexpr Floyd_Dist dist_coefficient(100000);
static constexpr Floyd_Dist sqrt2_const(141421);


/// ===== SIMULATION ===== class declaration ///

class Simulation {	
	
	private:
		// ===== Fields =====
		size_t nb_cells_;
		size_t max_dist_;
		Length player_radius_;
		Length player_speed_;
		Length ball_radius_;
		Length ball_speed_;
		Length marge_jeu_;
		Length marge_lecture_;
		Counter player_cooldown_per_t_;
		
		Simulation_State state_;
		
		bool success_;
							
		Map map_;
		std::vector<Player> players_;
		std::vector<Ball> balls_;
		
		Floyd_Matrix floyd_matrix_;
		
		/**
		 * In order to hide the inner modules from the gui we decided to use custom
		 * data structures for each object to be drawn.
		 */
		vec_player_graphics player_graphics_;		
		vec_ball_bodies ball_bodies_;		
		vec_obstacle_bodies obstacle_bodies_;		
		
	public:
	
		// ===== Constructor =====
		
		Simulation(std::vector<std::string> const&);
		
		
		// ===== Public Methods =====
		
		Simulation_State state() const;
		void state(Simulation_State);
		
		bool success() const;
		
		const std::vector<Player>& players() const;
		const std::vector<Ball>& balls() const;
		const Rectangle_map& obstacles() const;
		
		void nb_cells(size_t);
		bool initialise_player(double, double, Counter, Counter);
		bool initialise_ball(double, double, Angle);
		void initialise_dimensions(size_t);
	
		//signed input to test negative values. counter is for error report.
		bool initialise_obstacle(int, int, Counter);	
		
		bool test_collisions();
		void print_floyd();
		
		// ===== Manipulators =====
		
		const vec_player_graphics& player_graphics() const;
		const vec_ball_bodies& ball_bodies() const;
		const vec_obstacle_bodies& obstacle_bodies() const;
		
		
		// ===== Updaters =====
		
		void update(double delta_t);
		void update_graphics();
		
		
		// ===== Utilities =====
		
		bool is_over() const;		
		bool save(const std::string &o_file_path) const;
		
	private:
				
		bool test_center_position(double x,double y) const;
		bool detect_all_ball_player_collisions() const;
		bool detect_all_ball_obstacle_collisions() const;		
		bool detect_initial_player_collisions() const ;
		bool detect_initial_ball_collisions() const ;
		
		void initialise_floyd_matrix();
		void update_floyd();
		
		Floyd_Dist get_neighbor_distance(size_t x1, size_t y1, size_t x2, size_t y2);
		Floyd_Dist get_dist(size_t x1, size_t y1, size_t x2, size_t y2);
		
		void set_dist(size_t x1, size_t y1, size_t x2, size_t y2, Floyd_Dist dist);
		void init_dist_to_neighbors (size_t x1, size_t y1);
		
		Coordinate player_floyd_target(const Player&, bool&); 
		Coordinate get_cell_center(size_t, size_t);
		std::pair<size_t, size_t> get_grid_position(Coordinate const&);
		std::vector<std::pair<size_t,size_t>> obstacles_around(size_t x1, size_t y1);
		
		void update_player_targets();
		void update_player_directions();
		void update_player_positions();
		void update_ball_positions();
		void perform_player_actions();
		
		void update_player_graphics();
		void update_ball_bodies(); 
		void update_obstacle_bodies(); 
		
		void handle_ball_collisions();
		void handle_ball_player_collisions(Ball& ball);
		void take_player_life(size_t &player_index);
		
		bool detect_ball_player_collision(const Ball&, const Player&);
		bool detect_ball_ball_collision(const Ball& first, const Ball& second);
		
		void remove_collided_balls();
		void remove_dead_players();
		void remove_out_of_bounds();
		
		void remove_obstacle(size_t, size_t);

};


/// ===== READER ===== class declaration ///

/**
 * This class is used when reading a data from files.
 * It can be used to read specific datas and initialize the current simulation.
 * (eg. only import new players to the current simulation)
 */

/**
 * Enums directly related to a class are declared along with the class, and not on the
 * top of the document, to improve readability and modularity of the code.
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
		bool read_file(std::ifstream&, Simulation&,bool only_one_state = false);
		
	
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
	
};




//======================== Definitions ========================//


/// ===== SIMULATOR ===== ///



void Simulator::exec_parameters(const std::unordered_map<std::string, bool>& 
								exec_param) {
	execution_parameters() = exec_param;
}

const std::unordered_map<std::string, bool>& Simulator::exec_parameters() {
	return execution_parameters();
}

/**
 * We plan on making it possible to run multiple simulations simultaneously. This 
 * function will then return the index in active_sims() of the function currently
 * displayed in gui
 */
size_t& Simulator::current_sim_index() {
	static size_t current_sim_index_(0);	// initalized to indicate first sim
	return current_sim_index_;
}

/**
 * Wrapper function that contains the static execution_parameters field. For internal
 * use of Simulator class only.
 */
std::unordered_map<std::string, bool>& Simulator::execution_parameters() {
	static std::unordered_map<std::string, bool> execution_parameters_;
	return execution_parameters_;
}

/**
 * Wrapper function for active simulations.
 * This vector holds up to two simulation instances. During reading of a new 
 * simulation (with Open button) it will be added in this vector.
 * Note that in case of a successful reading, the new simulation object is moved to 
 * active_sims[0] and the data in the previous simulation is lost.
 */
std::vector<Simulation>& Simulator::active_sims() {

	static std::vector<Simulation> active_sims_;
	return active_sims_;
}

/**
 * Creates a new simulation with give io file(s) and replaces the current sim. 
 * 
 * This function when modified in the future will enable us to implement multiple 
 * simultaneous simulations. 
 */
bool Simulator::create_simulation(std::vector<std::string> const& io_files)	{
	
	bool success(false);

	if (active_sims().empty()) {
		active_sims().emplace_back(Simulation(io_files));
		success = active_sims()[current_sim_index()].success();
		if(success == false) {
			active_sims().pop_back();		// erase all data in case of bad file
		}
	} else {
		active_sims().push_back(Simulation(io_files));
		if ((success = active_sims().back().success())) // assigment intentional
			std::swap(active_sims()[current_sim_index()], active_sims().back());
			active_sims().pop_back();
			// for the moment old sim is destroyed. new sim is moved to index of the 
			// precedent.
	}								
	assert(active_sims.size()==1);
	return success;
}

/**
 * Imports an input file path and passes the call to create a new simulation with the
 * file on given path.
 */
bool Simulator::import_file(const std::string& file_path) {
	std::vector<std::string> io_files = {file_path};
	return create_simulation(io_files);
}

bool Simulator::empty() {
	return active_sims().empty();
}

/**
 * Returns the corresponding enum for the state of the active simulation.
 * The case of a player being trapped is not implemented yet.
 */
Simulation_State Simulator::active_simulation_state() {
	if(active_sims().empty())
		return NO_GAME;
	return active_sims()[current_sim_index()].state();
}

/**
 * Returns a vector containing player bodies along with their remeaning life counters
 * (for gui draw and color determination, respectively). 
 */
const vec_player_graphics& Simulator::fetch_player_graphics() {
	return active_sims()[current_sim_index()].player_graphics();
}

const vec_ball_bodies& Simulator::fetch_ball_bodies() {
	return active_sims()[current_sim_index()].ball_bodies();
}

const vec_obstacle_bodies& Simulator::fetch_obstacle_bodies() {
	
	return active_sims()[current_sim_index()].obstacle_bodies();
	
}

/**
 * Updates the active simulation (at index 0 of active_sims_) 
 */
void Simulator::update_active_sim(double delta_t) {
	active_sims()[current_sim_index()].update(delta_t);
}

/**
 * Update all active sims. Multiple active simulations are not implemented yet, so this
 * function behaves the  same as the one above.
 */
void Simulator::update_all_sims(double delta_t) {
	for (auto& sim: active_sims()) {
		sim.update(delta_t);
	}
}

/**
 * Saves the current simulation to given path 
 */
void Simulator::save_simulation(const std::string &file_path) {
	active_sims()[current_sim_index()].save(file_path);
}

/// ===== SIMULATION ===== ///

// ===== Constructor ===== 

Simulation::Simulation(std::vector<std::string>const& io_files) {
		// set base types to debug values. They must be properly initialised during
		// file import.															 
		nb_cells_ = 0;
		player_radius_ = -1.;
		player_speed_ = -1;
		ball_radius_ = -1.;
		ball_speed_ = -1;
		marge_jeu_ = -1.;
		marge_lecture_ = -1.;
		player_cooldown_per_t_ = 1;
		max_dist_ = -1;
		success_ = false;
		state(GAME_READY);
		
	if(Simulator::exec_parameters().at("Error")) {	//
		if(io_files.empty())
			exit(0);
		
		Reader reader(BEGIN);
		if(reader.import_file(io_files[0], *this) == false)
			exit(0);
		else
			success_ = true;
	} else {
		// if there are files to import to simulation
		if (io_files.size() > 0) {
			Reader reader(BEGIN);
			if(reader.import_file(io_files[0], *this) == true)
				success_ = true;
		}
		
		if (Simulator::exec_parameters().at("Step")){
			update(DELTA_T);
			save(io_files.back());
			return;
		}
		update_graphics();
		initialise_floyd_matrix();
	}
}

// ===== Public methods ===== 


const std::vector<Player>& Simulation::players() const {return players_;}

const std::vector<Ball>& Simulation::balls() const {return balls_;}

const Rectangle_map& Simulation::obstacles() const {return map_.obstacle_bodies();}

const vec_player_graphics& Simulation::player_graphics() const {
	return player_graphics_;
}

const vec_ball_bodies& Simulation::ball_bodies() const {
	return ball_bodies_;
}

const vec_obstacle_bodies& Simulation::obstacle_bodies() const {
	return obstacle_bodies_;
}

void Simulation::state(Simulation_State state) {
	state_ = state;
}

void Simulation::initialise_dimensions(size_t nb_cells) {
	
	nb_cells_ = nb_cells;
	
	player_radius_ = COEF_RAYON_JOUEUR * (SIDE/nb_cells);
	player_speed_ = COEF_VITESSE_JOUEUR * (SIDE/nb_cells);
		
	ball_radius_ = COEF_RAYON_BALLE * (SIDE/nb_cells);
	ball_speed_ = COEF_VITESSE_BALLE * (SIDE/nb_cells);
	
	marge_jeu_= COEF_MARGE_JEU * (SIDE/nb_cells);
	marge_lecture_= (COEF_MARGE_JEU/2) * (SIDE/nb_cells);
	
	Floyd_Dist nb_cells2 (nb_cells * nb_cells);

	max_dist_ = nb_cells2 * dist_coefficient * dist_coefficient;

	floyd_matrix_.resize(nb_cells2, std::vector<Floyd_Dist>(nb_cells2, max_dist_));

	map_.initialise_map(nb_cells_);
}

void Simulation::initialise_floyd_matrix() {
	size_t floyd_size(nb_cells_* nb_cells_);
	for (size_t i(0); i < nb_cells_; ++i) {
		for (size_t j(0); j < nb_cells_; ++j) {
			if (map_.is_free(i, j)) {
				init_dist_to_neighbors(i, j);
			}
		}
	}
	for(size_t i(0); i < floyd_size; ++i){
		floyd_matrix_[i][i]=0;
	}
	
	update_floyd();
}

void Simulation::print_floyd() {
	
	size_t matrix_size(floyd_matrix_.size());
	for(size_t i(0); i< nb_cells_*nb_cells_; ++i)
		std::cout << "\t"<<  i / nb_cells_ << "," << i % nb_cells_ << "\t";
	std::cout << std::endl;
	for (size_t i(0); i < matrix_size; ++i) {
		std::cout << i / nb_cells_ << "," << i % nb_cells_ << "\t";
		for (size_t j(0); j < matrix_size; ++j) {
			std::cout <<  floyd_matrix_[i][j] << "\t";
		}
		std::cout << std::endl;
	}
}

void Simulation::update_floyd(){
	size_t floyd_size(floyd_matrix_.size());
	Floyd_Dist sum(0);
	for(size_t k(0); k < floyd_size; ++k) {
		for(size_t i(0); i < floyd_size; ++i) {
			for(size_t j(0); j < floyd_size; ++j) { //symmetric matrix
				sum = floyd_matrix_[k][i] + floyd_matrix_[k][j];  
				if(sum < floyd_matrix_[j][i]){
					floyd_matrix_[j][i] = sum;		
				}	
			}	
		}		
	}
}

/**
 * This method is functional only if there's no obstacle in both (x1,y1) and (x2,y2).
 * (x1,y1) and (x2,y2) are neighbor coordinates from the grid.
 * 
 */
Floyd_Dist Simulation::get_neighbor_distance(size_t x1, size_t y1, 
											 size_t x2, size_t y2) {	
	int delta_x = x1 - x2;
	int delta_y = y1 - y2;
	
	if (map_.is_obstacle(x1, y1) || map_.is_obstacle(x2, y2)) return max_dist_;
	
	if(delta_x == 0 || delta_y == 0) { //horizontal/vertical
	 return dist_coefficient;
	}
	
	//be sure that there's no obstacle on the left and right
	if(delta_y < 0){ 
		if(map_.is_obstacle(x1, y1+1))
			return max_dist_;
	} else if(map_.is_obstacle(x1, y1-1)){ 
		return max_dist_;
	} 
	
	//be sure that there's no obstacle on the top or bottom
	if(delta_x < 0){
		if(map_.is_obstacle(x1 + 1, y1))
			return max_dist_;
	} else if(map_.is_obstacle(x1 - 1, y1)) {
		return max_dist_;
	}
	//no obstacles on sides, return sqrt(2) constant
	return sqrt2_const;
}

/**
 * Returns the stored value of distance between two indexes in floyd matrix.
 */ 
Floyd_Dist Simulation::get_dist(size_t x1, size_t y1, size_t x2, size_t y2){
	return floyd_matrix_[x1 * nb_cells_ + y1][x2 * nb_cells_ + y2];
} 


/**
 * Sets both of the indexes i.e from one square to another and vice-versa. 
 */
void Simulation::set_dist(size_t x1, size_t y1, size_t x2, size_t y2, 
						  Floyd_Dist dist){
	floyd_matrix_[x1 * nb_cells_ + y1][x2 * nb_cells_ + y2] = dist; //first
	floyd_matrix_[x2 * nb_cells_ + y2][x1 * nb_cells_ + y1] = dist; //second
} 


void Simulation::init_dist_to_neighbors (size_t x1, size_t y1){

	bool right_free(y1 < nb_cells_ - 1);
	bool bottom_free(x1 < nb_cells_ - 1);
	bool left_free(y1 != 0);

	if(right_free) {
		set_dist(x1, y1, x1, y1+1, get_neighbor_distance(x1, y1, 
					 x1, y1+1));	
		if(bottom_free){
			set_dist(x1, y1, x1+1, y1+1, get_neighbor_distance(x1, y1, 
					 x1+1, y1+1));
		 }
	}	
	if(bottom_free) {
		set_dist(x1, y1, x1+1, y1, get_neighbor_distance(x1, y1, 
				 x1+1, y1));
		if(left_free) {
		set_dist(x1, y1, x1+1, y1-1, get_neighbor_distance(x1, y1, 
				 x1+1, y1-1));
		}
	}
}

Coordinate Simulation::player_floyd_target(const Player& player, bool& trapped) {
	using index_pair = std::pair<size_t, size_t>;
	index_pair player_pos(get_grid_position(player.position()));
	index_pair target_pos(get_grid_position(player.target()-> position()));
	size_t player_x(player_pos.first), player_y(player_pos.second);
	size_t target_x(target_pos.first), target_y(target_pos.second);
	std::vector<index_pair> obs_around(obstacles_around(player_x, player_y));		
	size_t max_index(nb_cells_ - 1);
	size_t floyd_target_x(0), floyd_target_y(0);
	Floyd_Dist min_distance(max_dist_);
	Floyd_Dist distance(0);
	Length tolerance_w_radius(player_radius_ + marge_jeu_);
	std::cout << "--------Player position: [" << player_pos.first << "; " << player_pos.second << " ]----------" << std::endl;
	std::cout << "Target position: [" << target_pos.first << "; " << target_pos.second << " ]" << std::endl;
	for (int i(-1); i <= 1; ++i ) {	// check neighbours
		if (max_index < player_x + i || player_x + i < 0) continue; //check bounds
		for(int j(-1); j <= 1; ++j) {
			if (max_index < player_y + j || player_y + j < 0) continue;
			distance = get_dist(player_x + i, player_y + j, target_x, target_y);
			bool will_collide(false);
			std::cout << "Floyd distance for [" << player_x+i << ", " << player_y + j << "] : " << distance << std::endl;
			if (distance < min_distance){
				for (const auto& obs_pos : obs_around) {
					std::cout << "Obstacles around ... " << std::endl;

					if (Tools::segment_not_connected(obstacles().at(obs_pos),
													 player.position(),
													 get_cell_center(player_x + i, 
																	 player_y + j), 
													 tolerance_w_radius)) {
						std::cout << "Cannot Seee...." << std::endl;
						will_collide = true;
						break;
					}
				}
				if(will_collide == false) {
					floyd_target_x = player_x + i;
					floyd_target_y = player_y+ j;
					min_distance = distance;
				}
			}
		}
	}
	if(min_distance >= max_dist_) {
		trapped = true;
		std::cout << "this player is trapped..." << std::endl;
		return player.position();
	}
	
	std::cout << "F_target position: [" << floyd_target_x << "; " << floyd_target_y << std::endl;

	return get_cell_center(floyd_target_x, floyd_target_y);
}


std::pair<size_t, size_t> Simulation::get_grid_position(Coordinate const& coord){
	static constexpr double center_pos(DIM_MAX / SIDE);
	return std::pair<size_t,size_t>((size_t)((center_pos - coord.y/SIDE) * nb_cells_),
		   ((size_t)((coord.x/SIDE + center_pos) * nb_cells_)));
}

Coordinate Simulation::get_cell_center(size_t x, size_t y) {
	static constexpr double side_coeff(SIDE / DIM_MAX);
	Length half_square(DIM_MAX / nb_cells_);
	return {half_square * ((1 + side_coeff*y) - nb_cells_),
			half_square * (nb_cells_ - (1 + side_coeff*x))};
}

std::vector<std::pair<size_t,size_t>> Simulation::obstacles_around(size_t x, 
																   size_t y){
	static constexpr size_t neighbor_number(8);
	std::vector<std::pair<size_t,size_t>> obstacle_vec;
	obstacle_vec.reserve(neighbor_number);
	bool left_side(y==0);
	bool right_side(y==(nb_cells_-1));
	bool on_top(x==0);
	bool on_bottom(x==(nb_cells_-1));
	if(left_side == false){
		if(map_.is_obstacle(x,y-1)) 
			obstacle_vec.push_back(std::pair<size_t,size_t>(x, y-1));
		if(on_top == false && map_.is_obstacle(x-1,y-1))
			obstacle_vec.push_back(std::pair<size_t,size_t>(x-1, y-1));
		if(on_bottom == false && map_.is_obstacle(x+1,y-1))
			obstacle_vec.push_back(std::pair<size_t,size_t>(x+1, y-1));
	}
	if(right_side == false){
		if(map_.is_obstacle(x,y+1)) 
			obstacle_vec.push_back(std::pair<size_t,size_t>(x,y+1));
		if(on_top == false && map_.is_obstacle(x-1,y+1))
			obstacle_vec.push_back(std::pair<size_t,size_t>(x-1,y+1));
		if(on_bottom == false && map_.is_obstacle(x+1,y+1))
			obstacle_vec.push_back(std::pair<size_t,size_t>(x+1,y+1));
	}
	if(on_bottom == false && map_.is_obstacle(x+1, y))
		obstacle_vec.push_back(std::pair<size_t,size_t>(x+1,y));
	if(on_top == false && map_.is_obstacle(x-1, y))
		obstacle_vec.push_back(std::pair<size_t,size_t>(x-1,y));

	return obstacle_vec;
}   


void Simulation::update(double delta_t) {

	if(state() != GAME_READY) return;
	
	if (players_.size() < 2) {
		state(GAME_OVER);
		return;
	}
	update_player_targets();
	update_player_directions();
	update_player_positions();
	perform_player_actions();	
	update_ball_positions();
	handle_ball_collisions();
	remove_collided_balls();
	remove_dead_players();
	update_graphics();
	
	if (players_.size() < 2) {
		state(GAME_OVER);
	}
}

void Simulation::update_graphics() {
	update_player_graphics();
	update_ball_bodies();
	update_obstacle_bodies();
}

void Simulation::update_player_targets() {
	
	size_t players_size(players_.size());
	for(size_t i(0); i < players_size; ++i) {
		Length min_distance2(DIM_MAX*DIM_MAX*DIM_MAX*(double)DIM_MAX);

		for(size_t j(0); j < players_size; ++j) {
			if (i == j) continue;
			Length distance2(Tools::dist_squared(players_[i].body().center(),
												 players_[j].body().center()));
			if (distance2 < min_distance2) {
				min_distance2 = distance2;
				players_[i].target(&(players_[j]));
			}
		}
	}
}

void Simulation::update_player_directions() {
	
	if (players_.size() < 2) return;
	
	for (auto& player : players_) {
		
		bool intersects(false);
		for(const auto& obs : obstacles()) {
			Length tolerance_w_radius(player_radius_ + marge_jeu_);
			if (Tools::segment_not_connected(obs.second, player.body().center(), 
										 player.target()->body().center(),
										 tolerance_w_radius)) {
				intersects = true;
				break;							 
			}
		}
		player.target_seen(!intersects);
		
		if (player.target_seen()) {
			Vector to_target(player.target()->body().center()-player.body().center());
			player.direction(Vector(to_target));
		}
		else {
			bool trapped(false);
			Vector to_target (player_floyd_target(player, trapped) - 
							  player.body().center());
			if (to_target.length() <= marge_jeu_) 
				player.direction(Vector(0,0));
			else 
				player.direction(Vector(to_target));
			if (trapped) state(PLAYER_TRAPPED);
		}
	}
}

/**
 * Player directions must be udated before using this function.
 */ 
void Simulation::update_player_positions() {
	size_t nb_players(players_.size());
	Vector to_move;
	Length dist_per_t(DELTA_T * player_speed_);
	bool can_move(true);
	
	for(size_t i(0); i < nb_players; ++i) {
		to_move = dist_per_t*players_[i].direction().get_unit();
				
		for(size_t j(0); j < nb_players; ++j) {
			if (i == j) continue;
			if (Tools::intersect(players_[i].body(), players_[j].body(), 
								 marge_jeu_ + dist_per_t))
				can_move = false;
		}
		if (can_move)		
			players_[i].move(to_move);
		can_move = true;
	}
}

void Simulation::update_ball_positions() {
	static Length ball_dist_per_t(ball_speed_*DELTA_T);
	for(auto& ball : balls_) {
		ball.move(ball_dist_per_t*ball.direction().get_unit());
	}
}

void Simulation::perform_player_actions() {
	
	Coordinate ball_pos;
	
	for (auto& player : players_) {
		player.cool_down(player_cooldown_per_t_);
		
		if (player.target_seen() == false) continue;
		
		if((player.cooldown() >= MAX_COUNT)) {
			ball_pos = player.position() + 
					   (player.direction().get_unit()*(player_radius_ + ball_radius_ + 
						marge_jeu_ * 2)).pointed();
					   
			initialise_ball(ball_pos.x, ball_pos.y, player.direction().angle());
			player.cooldown(0);						
		}	
	}
}

void Simulation::update_player_graphics() {
		
	size_t nb_players(players().size());
	
	// resize to nb_players : we need only this many instances. fill if necessary with
	// dummies
	player_graphics_.resize(nb_players, std::make_tuple(nullptr, 0, RED));	
	
	double arc_angle;	// angle of the arc corresponding to cooldown counter 
						// of a player
		
	for(size_t i(0); i < nb_players; ++i) {
		
		auto player_color = static_cast<Predefined_Color>(players_[i].lives()-1);
		
		// alpha = 2*pi * (cooldown / max cooldown) 
		arc_angle = 2*M_PI*(players_[i].cooldown()/(double) MAX_COUNT);

		// modify existing values
		player_graphics_[i] = std::make_tuple(&players_[i].body(), arc_angle, 
											  player_color);	
	}
}

void Simulation::update_ball_bodies() {
	
	size_t nb_balls(balls().size());
	
	// resize ball_bodies_ and fill with dummies if necessary
	ball_bodies_.resize(nb_balls, nullptr);
	
	for(size_t i(0); i < nb_balls; ++i) {
		ball_bodies_[i] = &balls_[i].geometry();	
	}
}

void Simulation::update_obstacle_bodies() {
	
	size_t nb_obstacles(obstacles().size());
	// resize obstacle_bodies_ and fill with dummies if necessary
	obstacle_bodies_.resize(nb_obstacles, nullptr);	
	size_t counter(0);
	for(const auto& obs : obstacles()) {
		obstacle_bodies_[counter] = &obs.second;		
		++counter;
	}
}

void Simulation::handle_ball_collisions() {
	size_t nb_balls(balls_.size());
	if (nb_balls == 0) return;
	
	for(size_t i(0); i < nb_balls; ++i) {
		
		if(test_center_position(balls_[i].geometry().center().x, 
								balls_[i].geometry().center().y) == false) {
			balls_[i].collided(true);	//collided with game frame
		}
		
		for(size_t j(i + 1); j < nb_balls; ++j) {
			if (detect_ball_ball_collision(balls_[i], balls_[j])) {
				balls_[i].collided(true);
				balls_[j].collided(true);
			}
		}
		
		handle_ball_player_collisions(balls_[i]);
			
		for(auto &obs : obstacles()) {
			if(Tools::intersect(obs.second, balls_[i].geometry(), marge_jeu_)){
				remove_obstacle(obs.first.first, obs.first.second);
				balls_[i].collided(true);
			}
		}
	}
}

void Simulation::handle_ball_player_collisions(Ball& ball) {
	
	bool player_collided(false);
	size_t nb_players(players_.size());
	
	for(size_t j(0); j < nb_players; ++j) {
		player_collided = detect_ball_player_collision(ball, players_[j]);
		if(player_collided) {
			take_player_life(j);
			ball.collided(true);
		}
	}
}

bool Simulation::detect_ball_player_collision(const Ball& ball, const Player& player){
	return Tools::intersect(ball.geometry(), player.body(), marge_jeu_);
}

bool Simulation::detect_ball_ball_collision(const Ball& first, const Ball& second) {
	return Tools::intersect(first.geometry(), second.geometry(), marge_jeu_);
}

void Simulation::take_player_life(size_t &player_index) {
	players_[player_index].take_life();
}

void Simulation::remove_collided_balls() {
	
	size_t nb_balls(balls_.size());
	
	for (size_t i(0); i < nb_balls;) {
		if(balls_[i].collided()) {
			std::swap(balls_[i], balls_.back());
			balls_.pop_back();
			--nb_balls;
		} else {
			++i;
		}
	}
	
}

void Simulation::remove_dead_players() {
	
	size_t nb_players(players_.size());
	
	for (size_t i(0); i < nb_players;) {
		if(players_[i].lives() < 1 || players_[i].lives() > MAX_TOUCH) {
			std::swap(players_[i], players_.back());
			players_.pop_back();
			--nb_players;
		} else {
			++i;
		}
	}
}

void Simulation::remove_obstacle(size_t x, size_t y) {
	map_.remove_obstacle(x, y);
	initialise_floyd_matrix();
	update_floyd();
}

bool Simulation::initialise_obstacle(int x, int y, Counter counter){
	if(x < 0 || (unsigned)x > map_.max_index()) {
		std::cout << OBSTACLE_VALUE_INCORRECT(x) << std::endl;
		return false;
	}
	
	if(y < 0 || (unsigned)y > map_.max_index()) {
		std::cout << OBSTACLE_VALUE_INCORRECT(y) << std::endl;
		return false;
	}
	
	if(map_.is_obstacle(x, y)) {
		std::cout << MULTI_OBSTACLE(x, y) << std::endl;
		return false;
	}
	map_.add_obstacle(x,y);
	
	size_t nb_players(players_.size());	
	for(size_t i(0); i < nb_players; ++i) {
		if(Tools::intersect(map_.obstacle_body(x,y), 
		   players_[i].body(), marge_lecture_)) {
			std::cout << COLL_OBST_PLAYER(counter, (i+1)) << std::endl;
			return false;
		} 
	}
	return true;
}

bool Simulation::initialise_ball (double x, double y, Angle alpha){
	if(test_center_position(x,y) == false){
		std::cout << BALL_OUT(balls_.size() + 1) << std::endl;
		return false;	
	}
	balls_.push_back(Ball(alpha, x, y, ball_radius_));
	return true;
}

bool Simulation::initialise_player (double x, double y, Counter lives, 
									Counter cooldown){

	if(test_center_position(x,y) == false) {
		std::cout << PLAYER_OUT(players_.size() + 1) << std::endl;
		return false;
	}
	
	players_.push_back(Player(x, y, player_radius_, lives, cooldown));
	return true;
}

bool Simulation::test_center_position(double x,double y) const {
	if (x < -DIM_MAX || y < -DIM_MAX || x > DIM_MAX || y > DIM_MAX) {
		return false;
	}
	return true;
}

bool Simulation::success() const {
	return success_;
}

void Simulation::nb_cells(size_t nb_cells){
	nb_cells_ = nb_cells;
}

bool Simulation::detect_initial_player_collisions() const {
	size_t nb_players = players_.size();
	for(size_t i(0); i < nb_players; ++i) {
		for(size_t j(i+1); j < nb_players; ++j) {
			if(Tools::intersect((players_[j]).body(), (players_[i]).body(),
								marge_lecture_)) {
				std::cout << PLAYER_COLLISION(i+1, j+1) << std::endl;
				return false;
			}
		}
	}
	return true;
}
 
bool Simulation::detect_initial_ball_collisions() const {
	size_t nb_balls = balls_.size();
	for(size_t i(0); i < nb_balls; ++i){
		for(size_t j(i+1); j < nb_balls; j++){
			if(Tools::intersect(balls_[i].geometry(), 
								balls_[j].geometry(), marge_lecture_)){
				std::cout << BALL_COLLISION(i+1,j+1) << std::endl;
				return false;								
			}	
		}	
	}
	return true;
}

bool Simulation::detect_all_ball_player_collisions() const {
	size_t nb_balls = balls_.size();
	size_t nb_players = players_.size();
	for(size_t i(0); i<nb_balls; ++i){
		for(size_t j(0); j<nb_players; ++j){
			if(Tools::intersect(players_[j].body(), 
								balls_[i].geometry(), marge_lecture_)) {
				std::cout << PLAYER_BALL_COLLISION(j+1,i+1) << std::endl;
				return false;					
			}
		}
	}
	return true;
}

bool Simulation::detect_all_ball_obstacle_collisions() const {
	size_t nb_balls = balls_.size();
	for(auto const& obs_value : map_.obstacle_bodies()){
		for(size_t i(0); i<nb_balls; ++i){
			if(Tools::intersect(obs_value.second,balls_[i].geometry(),marge_lecture_)){
				std::cout << COLL_BALL_OBSTACLE(i+1) << std::endl; 
				return false;
			}
		}
	}
	return true;
}

bool Simulation::test_collisions() {
	if(detect_initial_player_collisions() == false)
		return false;
	if(detect_initial_ball_collisions() == false)
		return false;
	if(detect_all_ball_player_collisions() == false)
		return false;
	if(detect_all_ball_obstacle_collisions() == false)
		return false;
		
	return true;
}

Simulation_State Simulation::state() const {
	
	return state_;
}

/**
 * Saves the current state of the simulation to a given file path. Exporting will be
 * done in a straightforward way so there is no need for another class as in Reader.
 */
bool Simulation::save(const std::string &o_file_path) const {
	std::ofstream o_file(o_file_path);
	if (!o_file) return false;
	std::ostringstream os_stream;	//to reach the file only once at the end
	
	os_stream << "# nbCell" << "\n\t" << nb_cells_ << "\n\n";
	
	os_stream << "# number of players" << "\n\t" << players_.size() << "\n\n";
	os_stream << "# position of players" << "\n\t";
	for (auto const& player : players_) {
		os_stream << player.body().center().x << "\t" << player.body().center().y;
		os_stream << "\t" << player.lives() << "\t" << player.cooldown() << "\n\t";
	}
	os_stream << "\n";
	
	os_stream << "# nbObstacles" << "\n\t" << map_.nb_obstacles() << "\n\n";
	os_stream << "# position of obstacles" << "\n";
	for(size_t i(0); i < nb_cells_; ++i) {
		for(size_t j(0); j < nb_cells_; ++j) {
			if(map_.is_obstacle(i, j)) {
				os_stream << "\t" << i << "\t" << j << "\n";
			}
		}
	}
	os_stream << "\n";
	
	os_stream << "# nbBalls" << "\n\t" << balls_.size() << "\n\n";
	os_stream << "# position of balls" << "\n\t";
	for (auto const& ball : balls_) {
		os_stream << ball.geometry().center().x << "\t" << ball.geometry().center().y;
		os_stream << "\t" << ball.direction().angle() << "\n\t";	
	}
	os_stream << "\n# file saved successfully";
	
	o_file << os_stream.str();
	if(!o_file) return false;
	
	o_file.close();		
	return true;
}




/// ===== READER ===== ///

// ===== Constructor ======

Reader::Reader(ReaderState reader_state) : reader_state_(reader_state) {}

// ==== Accessors & Manipulators ======

ReaderState Reader::reader_state() const {return reader_state_;}

void Reader::reader_state(ReaderState reader_state) {reader_state_ = reader_state;}

// ===== Public Methods =====

/**
 * Reads and imports a complete data file 
 * from "file_adress" to the given "simulation" object.
 * 
 * Failures due to reading are traited depending on the execution parameters of
 * simulator.
 * eg. The program stops if an error is occured in Error mode, waits for another input
 * if not in error mode.
 */
bool Reader::import_file(std::string const& file_adress, Simulation& simulation) {

	std::ifstream in_file(file_adress);
	if(in_file.fail()) { //file reading is not possible, failbit occured
		#ifndef NDEBUG
		std::cout << "File : \"" << file_adress << "\" can't be read." << std::endl;
		#endif
		in_file.close();
		return false;
	}
	if(read_file(in_file, simulation) == false){
		#ifndef NDEBUG
		print_error_state(reader_state_);
		#endif
		in_file.close();
		return false;
	}
	
	in_file.close();
	return true;
}


/**
 * Reads simulation data from "in_file" to given simulation starting from the given
 * state of the simulation.
 * 
 * This function can also be used to import a specific type of data from a file
 * to the simulation (e.g.only obstacle data), if "only_one_state" is given true.
 * (This will also be used to manipulate & test the current simulation in the future!)
 * 
 * Returns true if the reading is successful.
 */
bool Reader::read_file(std::ifstream& in_file, Simulation& simulation, 
					  bool only_one_state){
		switch(reader_state_){
			case BEGIN: reader_state_ = READ_NB_CELLS;
			
			case READ_NB_CELLS: 
				if(read_nb_cells(in_file, simulation)==false) return false;
				if(only_one_state) {
					finalise_reading(reader_state_);
					break;
				}//only one step, success
				reader_state_ = READ_PLAYERS; 	//next step
				
			case READ_PLAYERS:	
				if(read_players(in_file, simulation)==false) return false;				
				if(only_one_state) {
					finalise_reading(reader_state_);
					break;
				}//only one step, success				
				reader_state_=READ_OBSTACLES;	//next step
			
			case READ_OBSTACLES:
				if(read_obstacles(in_file, simulation)==false) return false;	
				if(only_one_state) {
					finalise_reading(reader_state_);
					break;
				}//only one step, success
				reader_state_ = READ_BALLS; //next step
			
			case READ_BALLS:
				if(read_balls(in_file, simulation)==false)
					return false;
				reader_state_=SUCCESS;
			
			case SUCCESS:
				if(simulation.test_collisions() == false) return false;
				std::cout << FILE_READING_SUCCESS << std::endl;
				return true;
		}
	return false;
}

// ===== Private Static Methods =====

/**
 * Sets the given string to the next line in file that possibly contains a data.
 * 
 * Returns true if the reading is succesful.
 */
bool Reader::next_data_line(std::ifstream& from, std::string& data_line){
	do {
		if(!getline(from >> std::ws, data_line)){ //error? when reading the next line
			return false;
		}
	}while(data_line[0]=='#');
	//line containing possible data is in "data_line"
	return true;
}


/**
 * Reads the data for the number of cells from the given file to the "simulation".
 * 
 * The next data on "in_file" must be the data for the nb_cells when calling this
 * function. 
 * 
 * Numeric values and the grid a initialised after reading of "nb_cells"
 */
bool Reader::read_nb_cells(std::ifstream& in_file, Simulation& simulation){
	std::string line;
	if(next_data_line(in_file,line) ==  false)
		return false;
	int nb_cells(0);
	std::istringstream i_string(line);
	if(!(i_string >> nb_cells) || nb_cells<MIN_CELL) {
		#ifndef NDEBUG
		std::cout << "Invalid cell number: " << nb_cells << std::endl;
		#endif
		return false;
	}
	simulation.initialise_dimensions((size_t)nb_cells); // no internal tests necessary
	return true;
}

/**
 * Reads the data for the players from the given file to the "simulation".
 * 
 * The next data on "in_file" must be the data for the players when calling this
 * function.
 */
bool Reader::read_players(std::ifstream& in_file, Simulation& simulation){
	std::string line;
	if(next_data_line(in_file, line) == false)
		return false;
	int nb_players(0);
	std::istringstream i_string(line);
	if(!(i_string >> nb_players) || nb_players<0){
		#ifndef NDEBUG
		std::cout << "Invalid number of players." << std::endl;
		#endif
		return false;
	}
	
	double x(0), y(0);
	Counter nbt(0);
	Counter count(0);
	
	for(int i(0); i<nb_players; ++i){
		if(!next_data_line(in_file, line)) return false;
		
		i_string.clear();
		i_string.str(line);
		
		if(!(i_string >> x >> y >> nbt >> count)) return false;
		if (simulation.initialise_player(x, y, nbt, count) == false) return false;
	}
	return true;
}

/**
 * Reads the data for the obstacles from the given file to the "simulation".
 * 
 * The next data on "in_file" must be the data for the obstacles when calling this
 * function.
 */
bool Reader::read_obstacles(std::ifstream& in_file, Simulation& simulation){
	std::string line;
	if(next_data_line(in_file,line) == false) return false;
	
	std::istringstream i_string(line);
	int nb_obstacles(0);
	if(!(i_string >> nb_obstacles) || nb_obstacles < 0){
		#ifndef NDEBUG
		std::cout << "Invalid nb obstacles : " << std::endl; //for debugging
		#endif
		return false;
	}
		
	int x(0), y(0);
	for(int i(0); i < nb_obstacles; ++i) {
		if(next_data_line(in_file,line) == false) return false;
		
		i_string.clear();
		i_string.str(line);
		
		if(!(i_string >> x >> y)) return false; //line and column
		if (simulation.initialise_obstacle(x, y, (i+1)) == false) return false;
	}
	
	return true;
}

/**
 * Reads the data for the balls from the given file to the "simulation".
 * 
 * The next data on "in_file" must be the data for the balls when calling this
 * function.
 */
bool Reader::read_balls(std::ifstream& in_file, Simulation& simulation){
	std::string line;
	if(next_data_line(in_file,line) == false) return false;
	
	std::istringstream i_string(line);
	int nb_balls(0);
	if(!(i_string >> nb_balls)){
		return false;
	}
	
	double x(0), y(0);
	double angle(0);
	for(int i(0); i < nb_balls; ++i) {
		if(next_data_line(in_file,line) == false) return false;
	
		i_string.clear();
		i_string.str(line);
		
		if(!(i_string >> x >> y >> angle)) return false; //line and column
		if (simulation.initialise_ball(x, y, angle) == false) return false;
	}	
	return true;
	
}

void Reader::finalise_reading(ReaderState &actual_state) {
	actual_state=SUCCESS; 
	std::cout << FILE_READING_SUCCESS << std::endl;
}

/**
 *	Prints the error state of the given reader to the console.
 * 	Must only be used to print errors. (unused ifdef #NDEBUG)
 */
void Reader::print_error_state(ReaderState error_state) {
	const static std::array<std::string,(SUCCESS+1)> state_strings = {
	"BEGIN","READ_CELLS","READ_PLAYERS","READ OBSTACLES","READ_BALLS","SUCCESS"
};
	if(error_state == SUCCESS){
		std::cout <<"No error : " << state_strings[error_state] << std::endl;
	}
	else{
		std::cout << "Error in state : " << state_strings[error_state] << std::endl;
	}
}

