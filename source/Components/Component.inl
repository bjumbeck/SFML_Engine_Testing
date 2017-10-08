#include "Entity/EntityManager.hpp"

template <typename CompType>
inline BaseComponent::Family Component<CompType>::family()
{
    static Family family = familyCounter++;
    assert(family < MAX_COMPONENTS);

    return family;
}

template <typename CompType, typename EManager>
inline bool ComponentPtr<CompType, EManager>::valid() const
{
    return entityManager && 
           entityManager->valid(owningEntityId) &&
           entityManager->template hasComponent<CompType>(owningEntityId);
}

template <typename CompType, typename EManager>
inline void ComponentPtr<CompType, EManager>::remove()
{
    assert(valid());

    entityManager->template remove<CompType>(owningEntityId);
}

template <typename CompType, typename EManager>
inline CompType* ComponentPtr<CompType, EManager>::get()
{
    assert(valid());

    return entityManager->template getComponentPtr<CompType>(owningEntityId);
}

template <typename CompType, typename EManager>
inline const CompType* ComponentPtr<CompType, EManager>::get() const
{
    assert(valid());
    
    return entityManager->template getComponentPtr<CompType>(owningEntityId);
}

template <typename CompType, typename EManager>
inline ComponentPtr<CompType, EManager>::operator bool() const
{
    return valid();
}

template <typename CompType, typename EManager>
inline CompType* ComponentPtr<CompType, EManager>::operator->()
{
    assert(valid());

    return entityManager->template getComponentPtr<CompType>(owningEntityId);
}

template <typename CompType, typename EManager>
inline const CompType* ComponentPtr<CompType, EManager>::operator->() const
{
    assert(valid());

    return entityManager->template getComponentPtr<CompType>(owningEntityId);
}

template <typename CompType, typename EManager>
bool ComponentPtr<CompType, EManager>::operator==(const ComponentPtr<CompType>& rhs) const
{
    return entityManager == rhs.entityManager && 
           owningEntityId == rhs.owningEntityId;
}

template <typename CompType, typename EManager>
bool ComponentPtr<CompType, EManager>::operator!=(const ComponentPtr<CompType>& rhs) const
{
    return !(*this == other);
}