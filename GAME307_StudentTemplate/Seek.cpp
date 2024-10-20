#include "Seek.h"

SteeringOutput Seek::getSteering()
{
        auto result = SteeringOutput();
        // get direction to target
        result.linear = target->getPos() - character->getPos();
            // give full acceleration
            /*result.linear.normalize();*/
        result.linear = VMath::normalize(result.linear);
        result.linear *= maxAccel;
        result.angular = 0;
    return result;
    return SteeringOutput();
}

Seek::Seek(Body* _character, Body* _target, float _maxAccel)
{
    character = _character;
    target = _target;
    maxAccel = _maxAccel;
}
