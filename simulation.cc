/**
 * file: simulation.cc
 * 
 * authors:	Cem Keske
 * 			Emre Yazici
 */
#include "simulation.h"
#include "error.h"
#include "define.h"
#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <fstream>



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
 * "simulation" object.
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
 * 	Must only be used to print errors. (unused ifdef #NDDEBUG)
 */
void print_error_state(ReaderState error_state) {
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





/// ===== SIMULATION ===== ///

// ===== Constructor ===== 

Simulation::Simulation(std::unordered_map<std::string,bool>const& execution_parameters,
					   std::vector<std::string>const& io_files) : execution_parameters_
																 (execution_parameters)
																 {
		// set base types to debug values. They must be properly initialised during
		// file import.															 
		nb_cells_ = 0;
		player_radius_ = -1.;
		player_speed_ = -1.;
		ball_radius_ = -1.;
		ball_speed_ = -1.;
		marge_jeu_ = -1.;
		marge_lecture_ = -1.;

	if(execution_parameters_["Error"]) {	//
		if(io_files.empty()) { 
			#ifndef NDEBUG
			std::cout << "No input file. Exiting...";
			#endif
			exit(0);
		}

		Reader reader(BEGIN);
		if(reader.import_file(io_files[0], *this) == false)
			exit(0);
	}
}

// ===== Public methods ===== 

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