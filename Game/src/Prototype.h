#pragma once

class Prototype
{
public:
	virtual ~Prototype() = default;
	virtual Prototype* clone() const = 0;
};