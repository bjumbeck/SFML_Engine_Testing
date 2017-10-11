#include <SFML/Graphics/RenderTarget.hpp>
#include "RenderSystem.hpp"
#include "Entity/EntityManager.hpp"
#include "Components/Component.hpp"
#include "Components/RenderableComponent.hpp"
#include "Components/TransformableComponent.hpp"

RenderSystem::RenderSystem(sf::RenderTarget& target)
    : renderTarget(target)
{}

void RenderSystem::configure(EventManager& eventManager)
{}

void RenderSystem::update(EntityManager& entityManager, EventManager& eventManager, const sf::Time& deltaTime)
{
    // Not needed for now.
}

void RenderSystem::render(EntityManager& entityManager)
{
    ComponentPtr<RenderableComponent> renderComp;
    ComponentPtr<TransformableComponent> transComp;
    for (Entity entity : entityManager.getEntitiesWithComponents(renderComp, transComp))
    {
        sf::RenderStates states = sf::RenderStates::Default;

        sf::Transform statesTransform;
        statesTransform.translate(transComp->position);
        statesTransform.scale(transComp->scale);
        statesTransform.rotate(transComp->rotation);

        states.transform *= statesTransform;
        states.texture = renderComp->texture;
        renderTarget.draw(renderComp->vertexArray, states);
    }
}