#ifndef INTERSECTION_H
#define INTERSECTION_H

class Intersection {
private:
	bool intersected;
	float distance;
public:

	Intersection(bool intersected, float distance) : distance(distance) { this->intersected = intersected; }
};

#endif