/**
 * file: map.cc
 * 
 * authors:	Cem Keske
 * 			Emre Yazici
 */
#include "map.h"
#include "define.h"
#include <assert.h>

/// ===== MAP ===== ///



// ===== Map initializer =====


void Map::initialise_map(size_t nbCell) {
	nb_obstacles_ = 0;
	grid_ = Grid(nbCell); 
	size_ = nbCell;
	for(auto& col: grid_)
		col.resize(nbCell);
}

// ===== Accessors & Manipulators ===== ///

size_t Map::max_index() const {return size_ - 1;}

size_t Map::nb_obstacles() const {return nb_obstacles_;}


bool Map::is_free(size_t line, size_t col) const {
	assert(line < size_ && col < size_);	// parameter test, for debug
	
	return grid_[line][col] == Cell::FREE;
}

bool Map::is_obstacle(size_t line, size_t col) const {
	assert(line < size_ && col < size_);	// parameter test, for debug
	
	return grid_[line][col] == Cell::OBSTACLE;
}

const Rectangle& Map::obstacle_body(size_t line, size_t col) const {
	assert(x >= 0 && y >= 0 && x <= max_index() && y <= max_index());
	assert(is_obstacle(line, col));
	
	return obstacles_.at(std::make_pair(line, col));
}

const Rectangle_map& Map::obstacle_bodies() const {
	return obstacles_;
}

// ===== Utility methods =====

void Map::add_obstacle(size_t line, size_t col) {
	assert(line < size_ && col < size_);	// parameter tests, for debug
	assert(is_free(line, col));
	
	grid_[line][col] = Cell::OBSTACLE;
	create_obstacle(line,col);
	
	nb_obstacles_++;
}

void Map::remove_obstacle(size_t line, size_t col) {
	assert(line < size_ && col < size_);	// parameter tests, for debug
	assert(is_obstacle(line,col));
	
	grid_[line][col] = Cell::FREE;
	destroy_obstacle(line,col);
	
	nb_obstacles_--;
}

/**
 * Creates a rectangle corresponding to obstacle at index ("line", "col") and stores
 * it in "obstacles_". 
 */
void Map::create_obstacle(size_t line, size_t col) {
	Length rectangle_side = SIDE / size_;	// A = SIDE / nb_cell
	double bottom_left_x = -DIM_MAX + col*rectangle_side;
	double bottom_left_y = DIM_MAX - (line+1)*rectangle_side;
	Rectangle rect({bottom_left_x, bottom_left_y}, rectangle_side, rectangle_side);
	obstacles_.emplace(std::make_pair(line, col), rect);
}

void Map::destroy_obstacle(size_t line, size_t col) {
	obstacles_.erase(std::make_pair(line ,col));
}
