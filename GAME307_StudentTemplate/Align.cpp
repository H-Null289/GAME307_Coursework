#include "Align.h"

// mapToRange function is borrowed from this website --> https://rosettacode.org/wiki/Map_range

template<typename tVal>

tVal mapToRange(std::pair<tVal, tVal> a, std::pair<tVal, tVal> b, tVal inVal)
{
	tVal inValNorm = inVal - a.first;
	tVal aUpperNorm = a.second - a.first;
	tVal normPosition = inValNorm / aUpperNorm;

	tVal bUpperNorm = b.second - b.first;
	tVal bValNorm = normPosition * bUpperNorm;
	tVal outVal = b.first + bValNorm;

	return outVal;
}

SteeringOutput Align::getSteering()
{
	SteeringOutput result;
	auto rotation = (target->getOrientation() - character->getOrientation());

	// maps angle ranging from 0 to 360, to -180 to 180, fmod is there to make sure the angle doesn't go past 360
	rotation = mapToRange({ 0.0, 2 * M_PI }, { -M_PI, M_PI }, std::fmod((rotation + M_PI), (2 * M_PI)));

	float rotationSize = abs(rotation);
	if (rotationSize < targetRadius) {
		return SteeringOutput();
	}

	float targetRotation;
	if (rotationSize > slowRadius) {
		targetRotation = maxRotation;
	}
	else {
		targetRotation = (maxRotation * rotationSize) / slowRadius;
	}

	targetRotation *= rotation / rotationSize;
	result.angular = targetRotation - character->getRotation();
	result.angular /= timetoTarget;

	if (abs(result.angular) > maxAngularAccel) {
		result.angular /= (abs(result.angular));
		result.angular *= maxAngularAccel;
	}
	return result;
}

Align::Align(Body* _character, Body* _target, 
	float _maxAngularAccel, float _maxRotation,
	float _targetRadius, float _slowRadius, float _timetoTarget)
{
	character = _character;
	target = _target;
	maxAngularAccel = _maxAngularAccel;
	maxRotation = _maxRotation;
	targetRadius = _targetRadius;
	slowRadius = _slowRadius;
	timetoTarget = _timetoTarget;
}
