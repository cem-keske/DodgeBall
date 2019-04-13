/**
 * file: tools.cc
 * 
 * authors:	Cem Keske
 * 			Emre Yazici
 */
#include <cmath>
#include "tools.h"
#include "assert.h"

/// ===== LOCAL (MODULE) FUNCTION FORWARD DECLARATIONS ===== ///

/**
 * Returns the closest double value to "to_bound" from the interval [max,min].
 */ 
static double bound(double to_bound, double min, double max);


/// ===== COORDINATE ===== ///


// ===== Constructors =====

Coordinate::Coordinate() = default;

Coordinate::Coordinate(double x, double y) : x(x), y(y) {}

// ===== Utilites =====

Length Coordinate::distance(Coordinate const& r_operand) const {
	return std::sqrt((x-r_operand.x)*(x-r_operand.x) + 
					 (y-r_operand.y)*(y-r_operand.y));
}

std::string Coordinate::to_string() const {
	return "[" + std::to_string(this->x) + ", " + std::to_string(this->y) + "]";
}

Coordinate Coordinate::symmetric_x_axis() const{
	return {x,-y};
}

Coordinate Coordinate::symmetric_y_axis() const{
	return {-x,y};
}
// ===== Operator Overloads =====

Coordinate& Coordinate::operator+=(Coordinate const& rhs){
	this->x += rhs.x;
	this->y += rhs.y;
	return *this;
}

Coordinate& Coordinate::operator-=(Coordinate const& rhs){
	this->x -= rhs.x;
	this->y -= rhs.y;
	return *this;
}

const Coordinate operator+(Coordinate lhs, Coordinate const& rhs) {
	return lhs+=rhs;
}

const Coordinate operator-(Coordinate lhs, Coordinate const& rhs) {
	return lhs-=rhs;
}


/// ===== VECTOR ===== ///


// ===== Constructors =====

Vector::Vector() = default;

Vector::Vector(double x, double y) : pointed_{x, y}{}

Vector::Vector(double angle) : pointed_{std::cos(angle), std::sin(angle)}{}

Vector::Vector(Coordinate const& pointed): pointed_(pointed){}

Vector::Vector(Coordinate const& start_point, Coordinate const& end_point) : 
			   pointed_(start_point - end_point){}


// ===== Accessors & Manipulators=====

Coordinate const& Vector::pointed() const {
	return pointed_;
}

Length Vector::length() const {
	return std::sqrt(pointed_.x*pointed_.x + pointed_.y*pointed_.y);
}

Angle Vector::angle() const{
	return atan2(pointed_.y , pointed_.x);
}

void Vector::make_unit(){
	this->length(1.0);
}

void Vector::length(Length length){
	(*this) *= (length / this->length());
}


// ===== Utilities =====


Vector Vector::get_perpendicular() const {
	return Vector(angle() + M_PI_2);
}

std::string Vector::to_string() const {
	return pointed_.to_string();
}

bool Vector::is_perpendicular(Vector const& vec, Angle tolerance){
	Angle diff = abs(vec.angle() - this->angle());
	tolerance = (tolerance>0) ? tolerance : (-tolerance); //positive tolerance
	return 	(abs(diff-(M_PI_2)) < tolerance || abs(diff-(3*M_PI_2)) < tolerance);
	
}

// ===== Operator Overloads =====

Vector& Vector::operator+=(Vector const& rhs) {
	this->pointed_+= rhs.pointed_;
	return *this;
}

Vector& Vector::operator-=(Vector const& rhs) {
	this->pointed_-= rhs.pointed_;
	return *this;
}

Vector& Vector::operator*=(double number) {
	this->pointed_.x *= number;
	this->pointed_.y *= number;
	return *this;
}

Vector& Vector::operator/=(double number) {
	this->pointed_.x /= number;
	this->pointed_.y /= number;
	return *this;
}

const Vector operator+(Vector lhs, Vector const& rhs) {
	return lhs += rhs;
}

const Vector operator-(Vector lhs, Vector const& rhs) {
	return lhs -= rhs;;
}

const Vector operator/(Vector vec, double number){
	return vec/=number;
}

const Vector operator*(Vector vec, double number){
	return vec*=number;
}

const Vector operator*(double number, Vector vec){
	return vec*=number;
}


/// ===== Rectangle ===== ///


// ===== Constructors =====

Rectangle::Rectangle(Coordinate const &bottom_left_, Length base, Length height)
					:bottom_left_(bottom_left_),base_(base),height_(height) { 
	assert(base>0 && height>0);
}

Rectangle::Rectangle(Coordinate const& vertex_a, Coordinate const& vertex_c)
					 :bottom_left_(vertex_a),
	 				  base_(abs(vertex_a.x - vertex_c.x)),
					  height_(abs(vertex_a.y - vertex_c.y)) {}
					  

// ===== Accessors & Manipulators =====

Length Rectangle::height() const {return height_;}

Length Rectangle::base() const {return base_;}

const Coordinate& Rectangle::bottom_left() const {return bottom_left_;}	

const Coordinate& Rectangle::top_left() const {
	return Coordinate{0,height_} += bottom_left_;
}						  
					
					
double Rectangle::y_up() const {
	return bottom_left_.y + height_;
}
double Rectangle::y_down() const {
	return bottom_left_.y;
}
double Rectangle::x_left() const {
	return bottom_left_.x;
}
double Rectangle::x_right() const {
	return bottom_left_.x + base_;
}

void Rectangle::bottom_left(Coordinate const& coordinate) {
	bottom_left_ = coordinate;
}

void Rectangle::height(Length height) {
	assert(height > 0);
	height_ = height;
}

void Rectangle::base(Length base) {
	assert(base > 0);
	base_ = base;
}

// ===== Utilities =====

bool Rectangle::contains(Coordinate const& coord) const {
	return (x_left() <= coord.x && coord.x <= x_right() &&
			y_down() <= coord.y && coord.y <= y_up());
};


/// ===== CIRCLE ===== ///

// ===== Constructors =====

Circle::Circle(Length radius) : Circle({0,0}, radius) {}

Circle::Circle(Coordinate const& center, Length radius): center_(center), 
														 radius_(radius) {
	assert(radius > 0);
}

// ===== Accessors & Manipulators =====

const Coordinate& Circle::center() const {return center_;}

Length Circle::radius() const {return radius_;}

void Circle::center(Coordinate const& center) {center_ = center;}

void Circle::radius(Length radius) {
	assert(radius > 0);
	radius_ = radius;
}


/// ===== Tools namespace ===== ///

const Color Tools::color_red() {return {0.93, 0, 0};} 		

const Color Tools::color_orange() {return {1, 0.28, 0.09};}

const Color Tools::color_yellow() {return {1, 0.91, 0};}

const Color Tools::color_green() {return {0, 0.94, 0};}

const Color Tools::color_blue() {return {0.19, 0.38, 0.92};}

const Color Tools::color_brown() {return {0.349,0.153,0.125};}

const Color Tools::color_black() {return {0,0,0};}

const Color Tools::color_white() {return {1,1,1};}



bool Tools::intersect(const Rectangle& rect_one, const Rectangle& rect_two, 
					  Length tolerance) {
	// rectangle intersection to write
	return false;
}

bool Tools::intersect(Circle const& circ_one,Circle const& circ_two,Length tolerance){
	if (tolerance < 0) tolerance = (-tolerance); // sign of tolerance doesn't matter
	Length sum_of_r = circ_one.radius() + circ_two.radius();
	return (circ_one.center()).distance(circ_two.center()) <= 
			sum_of_r + tolerance;
}

double bound(double to_bound, double min, double max) {
	to_bound = std::min(max, to_bound);
	to_bound = std::max(min, to_bound);		
	return to_bound;	
}

double Tools::distance(Coordinate const& c1, Coordinate const& c2){
	return c1.distance(c2);
}

bool Tools::intersect(Rectangle const& rectangle, Circle const& circle, Length tol){
	if(tol<0) tol= (-tol);
	Coordinate center(circle.center());
	Coordinate closest_point(bound(center.x, rectangle.x_left(), rectangle.x_right()),
							 bound(center.y, rectangle.y_down(), rectangle.y_up()));
							 
	//the closest point to the center of the circle on the rectangle is now at (x,y)
	return	(rectangle.contains(center) || 
			distance(center, closest_point) < (circle.radius() + tol));
}
