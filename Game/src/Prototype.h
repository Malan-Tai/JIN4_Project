#pragma once
#include <memory>

class Actor;

class Prototype
{
public:
	virtual ~Prototype() = default;
	virtual std::unique_ptr<Actor> clone() const = 0;
};