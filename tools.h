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
		
	
	
/// RECTANGLE ///
/** 
 * Rectangle specified by its base, bottom-left vertex and height.
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
		 * Points on borders considered like inside the rectangle.
		 */
		bool contains(Coordinate const&) const;
};

/// CIRCLE ///
/**
 * A circle at a coordinate.
 */
class Circle {
	private:
		Coordinate center_;
		Length radius_;
	
	public:
		
		// ===== Constructors =====
		 
		Circle(Length radius); //at origin
		Circle(Coordinate const& center, Length radius);
		
		// ===== Accessors =====
		
		const Coordinate& center() const;
		Length radius() const;
		
		// ===== Manipulators =====
		
		void center(Coordinate const&);
		void radius(Length);
};


/// ===== NAMESPACES FOR UTILITY FUNCTIONS ===== ///

namespace Tools {	
			
		// some color constants	
		Color color_red(); 		
		Color color_orange(); 
		Color color_yellow(); 
		Color color_green();	
	
		bool intersect(Rectangle const&, Rectangle const&, Length tolerance);
		bool intersect(Circle const&, Circle const&, Length tolerance);
		bool intersect(Rectangle const&, Circle const&, Length tolerance);
		double distance(Coordinate const&, Coordinate const&);
		
};


#endif
