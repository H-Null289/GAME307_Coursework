#include "VelocityMatch.h"

SteeringOutput VelocityMatch::getSteering()
{
    SteeringOutput result;
    result.linear = (target->getVel() - character->getVel()) / timeToTarget;
    if (VMath::mag(result.linear) > maxAccel) {
        result.linear = VMath::normalize(result.linear);
        result.linear *= maxAccel;
    }
    result.angular = 0;
    return result;
}

VelocityMatch::VelocityMatch(Body* _character, Body* _target, float _maxAccel, float _timeToTarget)
{
    character = _character;
    target = _target;
    maxAccel = _maxAccel;
    timeToTarget = _timeToTarget;
}
