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

/// ===== SIMULATION ===== class declaration ///

class Simulation {	
	
	private:
		// ===== Fields =====
		size_t nb_cells_;
		Length player_radius_;
		Length player_speed_;
		Length ball_radius_;
		Length ball_speed_;
		Length marge_jeu_;
		Length marge_lecture_;
		bool success_;
				
		Map map_;
		std::vector<Player> players_;
		std::vector<Ball> balls_;
		
		/**
		 * In order to hide the inner modules from the gui we decided to use custom
		 * data structures for each object to be drawn. For the time being they copy
		 * the geometrical data of each player, ball and obstacle. 
		 * 
		 * We intend to implement a better way to pass on these values without copying.  
		 */
		vec_player_graphics player_graphics_;		
		vec_ball_bodies ball_bodies_;		
		vec_obstacle_bodies obstacle_bodies_;		

	public:
	
		// ===== Constructor =====
		
		Simulation(std::vector<std::string> const&);

		
		// ===== Public Methods =====
		
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
		
		
		// ===== Accessor Methods for Graphics =====
		
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
		
		void update_player_targets();
		void update_player_directions();
		void update_player_positions();
		
		void update_player_graphics();
		void update_ball_bodies(); 
		void update_obstacle_bodies(); 
		
		void handle_ball_collisions();
		
		void remove_obstacle(size_t, size_t);
		//void remove_player(size_t);
		//void remove_ball(size_t);

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
	if(active_sims()[current_sim_index()].success())
		if(active_sims()[current_sim_index()].is_over())
			return GAME_OVER;
		return GAME_READY;
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
		success_ = false;

	if(Simulator::exec_parameters().at("Error")) {	//
		if(io_files.empty()) { 
			exit(0);
		}
		
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

/**
 * Updates the simulation and makes all necessary calculation
 */
void Simulation::update(double delta_t) {
	std::cout << "Updating targets..." << std::endl;
	update_player_targets();
	std::cout << "Updating directions..." << std::endl;
	update_player_directions();
	std::cout << "Updating positions..." << std::endl;
	update_player_positions();
	update_graphics();
}

void Simulation::update_graphics() {
	update_player_graphics();
	update_ball_bodies();
	update_obstacle_bodies();
}

/**
 * For each player its target is determined by finding the closest player.
 */
void Simulation::update_player_targets() {
	size_t players_size(players_.size());
	for(size_t i(0); i < players_size; ++i) {
		
		Length min_distance(DIM_MAX*DIM_MAX);
		
		for(size_t j(0); j < players_size; ++j) {
			if (i == j) continue;
			Length distance(Tools::distance(players_[i].body().center(),
											players_[j].body().center()));
			if (distance < min_distance) {
				min_distance = distance;
				players_[i].target(&(players()[j]));
			}
		}
	}
}

void Simulation::update_player_directions() {
	
	for (auto& player : players_) {
		
		bool target_seen(true);
		
		for(const auto& obs : obstacles()) {
			Length tolerance_w_radius(player_radius_ + marge_jeu_);
			std::cout << "Line: 442, " << player.target()->body().center().to_string() <<  std::endl;
			bool intersects= (Tools::segment_connected(obs.second, 
													  player.body().center(), 
													  player.target()->body().center(),
													  tolerance_w_radius));
			if(intersects)
				target_seen = false;
			std::cout << "Target seen: " << target_seen << std::endl;	
		}
		
		if (target_seen)
			player.direction(Vector(player.target()-> body().center() - 
									player.body().center()));
			std::cout << "Direction " << Vector(player.body().center(), 
									player.target()-> body().center()).to_string() << std::endl;

	}
}

void Simulation::update_player_positions() {
	size_t nb_players(players_.size());
	Vector to_move;
	bool can_move(true);
	
	for(size_t i(0); i < nb_players; ++i) {
		to_move = DELTA_T * player_speed_*players_[i].direction();
		for(size_t j(0); j < nb_players; ++j) {
			if (i == j) continue;
			if (Tools::intersect(players_[i].body(), players_[j].body(), marge_jeu_))
				can_move = false;
		}
		if (can_move)		
			players_[i].move(to_move);
		can_move = true;
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
		std::cout <<  "Line: 495... player body: " << players_[i].body().center().to_string() << std::endl;
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
	size_t nb_players(players_.size());

	std::vector<size_t> collided_balls;
	
	for(size_t i(0); i < nb_balls; ++i) {
		
		for(size_t j(0); j < nb_players; ++j) {
			if(Tools::intersect(players_[j].body(), balls_[i].geometry(), marge_jeu_)){
				std::swap(players_.back(), players_[j]);
				players_.pop_back();
				--nb_players;
				--i;
			}
		}
		
		for(auto obs : obstacles()) {
			if(Tools::intersect(obs.second, balls_[i].geometry(), marge_jeu_)){
				map_.remove_obstacle(obs.first.first, obs.first.second);
			}
		}
		
	}
	
	// Sort indexes to facilitate removal from balls_ vector
	std::sort(collided_balls.begin(), collided_balls.end());
	
	size_t nb_collided(collided_balls.size());
	for(size_t i(nb_collided - 1); i >= 0; ++i) {
		std::swap(balls_.back(), balls_.at(collided_balls[i]));
		balls_.pop_back();
	}
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
	if(test_center_position(x,y) == false){ //angle can be positive and negative
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
	for(auto const& obs_value : map_.obstacle_bodies()){ //iterate over obstacle pairs
		for(size_t i(0); i<nb_balls; ++i){
			if(Tools::intersect(obs_value.second,balls_[i].geometry(),marge_lecture_)){
				std::cout << COLL_BALL_OBSTACLE(i+1) << std::endl; 
				return false;
			}
		}
	}
	return true;
}

void Simulation::initialise_dimensions(size_t nb_cells) {
	this->nb_cells(nb_cells);
	
	player_radius_ = COEF_RAYON_JOUEUR * (SIDE/nb_cells);
	player_speed_ = COEF_VITESSE_JOUEUR * (SIDE/nb_cells);
		
	ball_radius_ = COEF_RAYON_BALLE * (SIDE/nb_cells);
	ball_speed_ = COEF_VITESSE_BALLE * (SIDE/nb_cells);
	
	marge_jeu_= COEF_MARGE_JEU * (SIDE/nb_cells);
	marge_lecture_= (COEF_MARGE_JEU/2) * (SIDE/nb_cells);
	
	map_.initialise_map(nb_cells_);

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

bool Simulation::is_over() const {
	// to write
	return false;
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

