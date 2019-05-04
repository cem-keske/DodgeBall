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
	length(1.0);
}

void Vector::length(Length length){
	(*this) *= (length / this->length());
}


// ===== Utilities =====


Vector Vector::get_perpendicular() const {
	return Vector(angle() + M_PI_2);
}

Vector Vector::get_unit() const {
	return Vector(angle());
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

/// ===== Segment ===== ///

// ===== Constructors =====

Segment::Segment(double x1, double y1, double x2, double y2):
				 point_(x1,y1),
				 point_b(x2,y2){}
				 
Segment::Segment(Coordinate const& p1, Coordinate const& p2):
				 Segment(p1.x,p1.y,p2.x,p2.y){}
		
// ===== Accessors =====
		
const Coordinate& Segment::point_a() const{
	return point_;
}

const Coordinate Segment::point_b() const{
	return point_b;
}

Vector Segment::direction() const{
	return Vector(point_.x - point_b.x, point_.y - point_b.y);
}
		
Length Segment::length() const{
	return distance(point_, point_b);
}

Length Segment::length_squared() const{
	return dist_squared(point_, point_b);
}
		
// ===== Utilities =====
		
Vector Segment::get_perpendicular() const{
	return direction.get_perpendicular();
} //unit vector

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

const Coordinate& Rectangle::bottom_right() const {
	return Coordinate{base_,0} += bottom_left_;
}
const Coordinate& Rectangle::top_left() const {
	return Coordinate{0,height_} += bottom_left_;
}				

const Coordinate& Rectangle::top_right() const {
	return Coordinate{base_,height_} += bottom_left_;
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

bool Rectangle::contains(Coordinate const& coord, Length tol) const {
	return (x_left() - tol <= coord.x && coord.x <= x_right() + tol &&
			y_down() - tol <= coord.y && coord.y <= y_up() 	  + tol);
};


/// ===== CIRCLE ===== ///

// ===== Constructors =====

Circle::Circle(Length radius) : Circle({0,0}, radius) {}

Circle::Circle(Circle const& circle): Circle({circle.center_.x,circle.center_.y},
											 circle.radius_){}

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


bool Tools::intersect(Rectangle const& rec, Segment const& seg, Length tol){
	Coordinate risky_point_a;
	Coordinate risky_point_b;
	Length tol_squared(tol*tol);
	Length len_squared(seg.length_squared());
	if((seg.direction().pointed().y * seg.direction().pointed().x) > 0){ 
		//slope's positive: only check top-left and bottom-right corner
		risky_point_a = closest_point(seg, rec.top_left());
		risky_point_b = closest_point(seg, rec.bottom_right());
		if(dist_squared(risky_point_a, seg.point_a()) > len_squared + tol_squared ||
		   dist_squared(risky_point_a, seg.point_b()) > len_squared + tol_squared ||
		   dist_squared(risky_point_b, seg.point_a()) > len_squared + tol_squared ||
		   dist_squared(risky_point_b, seg.point_b()) > len_squared + tol_squared){
			   return false; //the risky points are not on the segment
		   }
		return rec.contains(risky_point_a, tol) || rec.contains(risky_point_b, tol) ||
			   dist_squared(risky_point_a,rec.top_left()) <= tol_squared ||
			   dist_squared(risky_point_b,rec.bottom_right()) <= tol_squared;
		
	} else {
		//slope's negative: only check bottom-left and top-right corner
		risky_point_a = closest_point(seg, rec.bottom_left());
		risky_point_b = closest_point(seg, rec.top_right());
		
		return rec.contains(risky_point_a, tol) || rec.contains(risky_point_b, tol) ||
			   dist_squared(risky_point_a, rec.bottom_left()) <= tol_squared ||
			   dist_squared(risky_point_b, rec.top_right()) <= tol_squared;
	}
}


Length Tools::distance(Coordinate const& c1, Coordinate const& c2){
	return c1.distance(c2);
}

Length Tools::dist_squared(Coordinate const& c1, Coordinate const& c2){
	Length delta_x(c1.x-c2.x);
	Length delta_y(c1.y-c2.y);
	
	return delta_x*delta_x + delta_y*delta_y;
}


Coordinate Tools::closest_point(Segment const& seg, Coordinate const& coord){
	
	/**
	 * The formula to find the closest point on a segment 
	 * to another point given is used.
	 * 
	 * For the graphical and mathematical demonstration:
	 * --> desmos.com/calculator/fjawls
	 * --> The vector AC is projected on AB to determine the point.
	 */ 
	Length delta_x(seg.point_a().x - seg.point_b().x);
	Length delta_y(seg.point_a().y - seg.point_b().y);
	Length scale_x(coord.x - seg.point_a().x);
	Length scale_y(coord.y - seg.point_a().y);
	
	assert(delta_x != 0. || delta_y != 0.); //not both zero
	
	double vec_constant((scale_x*delta_x + scale_y*delta_y) /
						(delta_x*delta_x + delta_y*delta_y));
	return {seg.point_a().x + delta_x * vec_constant,
			seg.point_a().y + delta_y * vec_constant};
}

bool Tools::intersect(Rectangle const& rectangle, Circle const& circle, Length tol){
	if(tol<0) tol= (-tol);
	Coordinate center(circle.center());
	Coordinate closest_point(bound(center.x, rectangle.x_left(), rectangle.x_right()),
							 bound(center.y, rectangle.y_down(), rectangle.y_up()));
							 
	//the closest point to the center of the circle on the rectangle is now at (x,y)
	return	(rectangle.contains(center,tol) || 
			distance(center, closest_point) < (circle.radius() + tol));
}

double bound(double to_bound, double min, double max) {
	to_bound = std::min(max, to_bound);
	to_bound = std::max(min, to_bound);		
	return to_bound;	
}

