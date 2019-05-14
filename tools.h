/**
 * file: tools.h
 * 
 * authors:	Cem Keske
 * 			Emre Yazici
 */
#ifndef TOOLS_H_INCLUDED
#define TOOLS_H_INCLUDED

#define NDEBUG

/// ===== INCLUDES ===== ///

#include <string>

/// ===== TYPEDEFS ===== ///

typedef double Length; 
typedef double Angle;
typedef unsigned int Counter; //any type that counts something...

/// ===== STRUCTS ===== ///

/// COLOR ///
/**
 * Using convention [0,1]
 */
struct Color {
	double r;
	double g;
	double b;
};

/// COORDINATE ///

struct Coordinate {

	double x;
	double y;
	
	// ===== Constructors =====
	
	Coordinate();
	Coordinate(double x, double y);
	
	// ===== Utilites =====
	
	Length distance(Coordinate const&) const;	
	std::string to_string() const;	//[x, y]
	Coordinate symmetric_x_axis() const;
	Coordinate symmetric_y_axis() const;
	
	
	// ===== Member operator overloads =====
	
	Coordinate& operator+=(Coordinate const&);
	Coordinate& operator-=(Coordinate const&);
	
};

// ===== Non-member operator overloads =====

const Coordinate operator+(Coordinate, Coordinate const&);
const Coordinate operator-(Coordinate, Coordinate const&);


/// ===== CLASSES ===== ///




/// VECTOR ///
/**
 * A geometric vector on origin pointing to a coordinate.
 */
class Vector {
	
	private:
		
		Coordinate pointed_;

	public:
		
		// ===== Constructors =====
		
		Vector();
		Vector(Angle); //unit vector
		Vector(double x, double y); 
		Vector(Coordinate const& pointed);
		Vector(Coordinate const& start_point, Coordinate const& end_point);
		
		// ===== Accessors =====
		
		/**
		 * Returns the angle between the horizontal line and the vector in [-pi,pi].
		 */
		Angle angle() const;
		
		/**
		 * Returns a coordinate equivalent to the vector. (no renormalization).
		 */
		Coordinate const& pointed() const;
		
		/**
		 * Returns the euclidean length of the vector.
		 */
		Length length() const;
		
		/**
		 * Returns a perpendicular unit vector. 
		 * (Rotated pi/2 radians in positive direction.)
		 */
		Vector get_perpendicular() const;
		Vector get_unit() const;
		
		// ===== Manipulators =====
		
		void angle(Angle);
		void pointed(Coordinate const& pointed);
		void length(Length);
		
		/**
		 * Sets the vector's length to "1".
		 */ 
		void make_unit();
		
		// ===== Utilities =====
		
		/**
		 * Utilizes the format as coordinate.
		 */ 
		std::string to_string() const;
		
		bool is_perpendicular(Vector const&, Angle tolerance);
		
		// ===== Member operator overloads =====
		
		Vector& operator+=(Vector const&);
		Vector& operator-=(Vector const&);
		Vector& operator*=(double);
		Vector& operator/=(double);
};

// ===== Non-member operator overloads =====

const Vector operator+(Vector, Vector const&);
const Vector operator-(Vector, Vector const&);
const Vector operator*(double, Vector);
const Vector operator*(Vector, double);
const Vector operator/(Vector, double);
		


/// SEGMENT ///

class Segment {
	private:
		Coordinate point_;
		Coordinate point_b_;
	
	public:
	
		// ===== Constructors =====
		
		Segment(double x1, double y1, double x2, double y2);
		Segment(Coordinate const& p1, Coordinate const& p2);
		
		// ===== Accessors =====
		
		const Coordinate& point_a() const;
		const Coordinate point_b() const;
		const Vector& body() const;
		Vector direction() const;
		Length length() const;
		Length length_squared() const;
		
		// ===== Utilities =====
		
		Vector get_perpendicular() const;
		
};
	
	
/// RECTANGLE ///
/** 
 * Rectangle specified by its base, bottom left vertex and height.
 * Oblique rectnagles are not supported in this version.
 * 
 * Visualisation of vertexes:
 * 
 * d----c
 * |	|
 * a----b
 */
class Rectangle {
	
	private:
		Coordinate bottom_left_;
		Length base_;
		Length height_;
	
	public:
		
		// ===== Constructors =====
		
		Rectangle(Coordinate const& bottom_left, Length base, Length height);
		
		/**
		 * vertex_a is the bottom left corner and vertex_c is the top right corner.
		 */
		Rectangle(Coordinate const& vertex_a, Coordinate const& vertex_c);
				
		// ===== Accessors =====
		
		Length height() const;
		Length base() const;
		const Coordinate& bottom_left() const;
		const Coordinate& bottom_right() const;
		const Coordinate& top_left() const;
		const Coordinate& top_right() const;
		double y_up() const;	//upper bound
		double y_down() const;	//lower bound
		double x_left() const;	//left bound
		double x_right() const;	//right bound
			
		// ===== Manipulators =====
		
		void bottom_left(Coordinate const&);
		void height(Length);
		void base(Length);
		
		// ===== Utilities =====
		/**
		 * Points on borders considered inside the rectangle.
		 */
		bool contains(Coordinate const&) const; // zero tolerance
		bool contains(Coordinate const&, Length tol) const;
};

/// CIRCLE ///
/**
 * A circle at a coordinate.
 */
class Circle {
	protected:
		Coordinate center_;
		Length radius_;
	
	public:
		
		// ===== Constructors =====
		 
		Circle(Length radius); //at origin
		Circle(Coordinate const& center, Length radius);
		Circle(Circle const& circle);
		
		// ===== Accessors =====
		
		const Coordinate& center() const;
		Length radius() const;
		
		// ===== Manipulators =====
		
		void center(Coordinate const&);
		void radius(Length);
};


/// ===== NAMESPACES FOR UTILITY FUNCTIONS ===== ///

namespace Tools {	
	
		//some color constants
		constexpr Color COLOR_RED 		= {1.0,		0.0,	0.0};
		constexpr Color COLOR_ORANGE 	= {1.0,		0.53,	0.09};
		constexpr Color COLOR_YELLOW 	= {1.0,		0.91,	0.0};
		constexpr Color COLOR_GREEN 	= {0.0,		0.94,	0.0};
		constexpr Color COLOR_BLUE 		= {0.19,	0.38,	0.92};
		constexpr Color COLOR_BROWN 	= {0.349,	0.153,	0.125};
		constexpr Color COLOR_BLACK 	= {0.0,		0.0,	0.0};
		constexpr Color COLOR_WHITE 	= {1.0,		1.0,	1.0};
		
		/**
		 * WARNING!!
		 * If a shape's into another shape, we consider the intersection exists.
		 */ 
		bool intersect(Circle const&, Circle const&, Length tolerance);
		bool intersect(Rectangle const&, Circle const&, Length tolerance);
		bool intersect(Rectangle const&, Segment const&, Length tolerance);
		
		/**
		 * Returns true if two coordinates can be connected by a segment without
		 * intersecting the rectangle.
		 */ 
		bool segment_connected(Rectangle const& rectangle, Coordinate const& a,
							   Coordinate const& b, Length tolerance);
		
		Length distance(Coordinate const&, Coordinate const&);
		//used for fast calculation
		Length dist_squared(Coordinate const&, Coordinate const&); 
		
		//the segment is considered as a line
		Coordinate closest_point(Segment const&, Coordinate const&);
		
		/**
		 * Returns true if the point C is on the segment AB.
		 * Points A, B and C must be the same line !!!
		 */ 
		bool can_be_on(Segment const& a_b, Coordinate const& c);
		
		/**
		 * Returns true if c is between a and b.
		 */ 
		 bool is_between(double a, double b, double c);
};


#endif
