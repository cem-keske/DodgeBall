/**
 * file: map.h
 * 
 * authors:	Cem Keske
 * 			Emre Yazici
 */
#ifndef MAP_H_INCLUDED
#define MAP_H_INCLUDED
#include "tools.h"
#include <vector>
#include <map>

/// ===== TYPEDEF ===== ///

typedef std::map<std::pair<size_t, size_t>, Rectangle> Rectangle_map;

/// ===== ENUM ===== ///
/**
 * A cell is either empty or occupied by an obstacle. Enum class is preferred to ensure
 * each cell is given a value corresponding to its state (not any arbitrary boolean). 
 */  
enum class Cell : bool{
	FREE,
	OBSTACLE
};
// We need first the enum declaration to define this type
typedef std::vector<std::vector<Cell>> Grid; 

/// ===== MAP ===== ///

class Map{
	private:
		Grid grid_;							
		Rectangle_map obstacles_;	// geometrical representations of obstacles
		
		size_t size_;				// This is memorised to eliminiate the -
									//  need to compute grid.size() every time
		
	public:
	
		// ===== Initialiser =====
		
		/**
		 * A map object must always be initialised with this method before using!
		 */
		void initialise_map(size_t nb_cell);		 
		
		// ===== Accessors =====
		
		size_t max_index() const;			// maximum possible line or column index 
		bool is_free(size_t line, size_t col) const;
		bool is_obstacle(size_t line, size_t col) const; 
		const Rectangle& obstacle_body(size_t, size_t) const;
		
		/** 
		 * In the future installments this method may be replaced by another means
		 * of iterating over obstacle bodies to further solidify encapsulation.
		 */
		const Rectangle_map& obstacle_bodies() const;
		
		// ===== Utilities
		
		void add_obstacle(size_t line, size_t col);
		void remove_obstacle(size_t, size_t);
		
	private:
		/**
		 * These two functions are used to create the geometrical representations of
		 * obstacles into "obstacles_"
		 */
		void create_obstacle(size_t line, size_t col);
		void destroy_obstacle(size_t, size_t);
		
};





#endif
