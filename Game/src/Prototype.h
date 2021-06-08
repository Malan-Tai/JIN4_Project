#pragma once
#include <memory>

class Actor;

// interface for the Prototype design pattern.
// actors, inheriting from Prototype, are stocked in a map with an ID,
// and are then clone to be added into the world
class Prototype
{
public:
	virtual ~Prototype() = default;
	virtual std::unique_ptr<Actor> clone() const = 0;
};