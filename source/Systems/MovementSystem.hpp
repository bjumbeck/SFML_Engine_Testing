#pragma once

#include <SFML/System/Vector2.hpp>
#include "System.hpp"
#include "Components/Component.hpp"
#include "Components/TransformableComponent.hpp"
#include "Components/MovementComponent.hpp"
#include "Components/SteeringComponent.hpp"

class MovementSystem : public System<MovementSystem>
{
    public:
        MovementSystem() = default;

        // System overrides
        void configure(EventManager& eventManager) override;
        void update(EntityManager& entityManager, EventManager& eventManager, const sf::Time& deltaTime) override;

    private:
        // Steering Functionality
        // TODO: Seperate this into another system in the future possibly?
        // TODO: Figure out a better design for the steering functions
        // to many parameters but also don't wanna pass entity manager all the time.
        sf::Vector2f calculateSteering(Entity& entity, EntityManager& entityManager);

        sf::Vector2f seekBehavior(const ComponentPtr<SteeringComponent>& steering,
                                  const ComponentPtr<MovementComponent>& movement,
                                  const ComponentPtr<TransformableComponent>& transform);

        sf::Vector2f fleeBehavior(const ComponentPtr<SteeringComponent>& steering,
                                  const ComponentPtr<MovementComponent>& movement,
                                  const ComponentPtr<TransformableComponent>& transform);

        sf::Vector2f arriveBehavior(const ComponentPtr<SteeringComponent>& steering,
                                    ComponentPtr<MovementComponent>& movement,
                                    const ComponentPtr<TransformableComponent>& transform);
};