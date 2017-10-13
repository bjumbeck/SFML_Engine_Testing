#pragma once

#include <SFML/System/Vector2.hpp>
#include "Entity/Entity.hpp"

enum BehaviorType
{
    None = 0x00000,
    Seek = 0x00002,
    Flee = 0x00004,
    Arrive = 0x00008,
};

// TODO: Refactor a lot of the information in here out and into the SteeringSystem
struct SteeringComponent
{
    public:
        enum Deceleration
        {
            Slow = 3,
            Normal = 2,
            Fast = 1,
        };

    public:
        int behaviorFlags;

        // Seek
        sf::Vector2f seekTarget;

        // Flee
        sf::Vector2f fleeTarget;
        float fleePanicDistance;

        // Arrive
        sf::Vector2f arrivePosition;
        Deceleration arriveDeceleration;

        // Pursuit
        Entity pursuitTarget;
};