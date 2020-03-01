#include "ParticleContactResolutionSystem.h"
#include "ParticleComponent.h"
#include "ForceAccumulatorComponent.h"

namespace Reality
{
	ParticleContactResolutionSystem::ParticleContactResolutionSystem()
	{

	}

	void ParticleContactResolutionSystem::Update(float deltaTime)
	{
		auto contactEvents = getWorld().getEventManager().getEvents<ParticleContactEvent>();

		for (auto contactEvent : contactEvents)
		{
			ResolveVelocity(contactEvent, deltaTime);
		}
	}
	float ParticleContactResolutionSystem::CalculateSeperatingVelocity(ParticleContactEvent & contact)
	{
		Vector3 velocityA = contact.entityA.hasComponent<ParticleComponent>() ? contact.entityA.getComponent<ParticleComponent>().velocity : Vector3(0, 0, 0);
		Vector3 velocityB = contact.entityB.hasComponent<ParticleComponent>() ? contact.entityB.getComponent<ParticleComponent>().velocity : Vector3(0, 0, 0);

		Vector3 relativeVelocity = velocityA - velocityB;
		return glm::dot(relativeVelocity, contact.normal);
	}
	void ParticleContactResolutionSystem::ResolveVelocity(ParticleContactEvent & contact, float deltaTime)
	{
		float seperatingVelocity = CalculateSeperatingVelocity(contact);
		if (seperatingVelocity > 0)
		{
			return;
		}

		bool isValidA = contact.entityA.hasComponent<ForceAccumulatorComponent>();
		bool isValidB = contact.entityB.hasComponent<ForceAccumulatorComponent>();

		float invM1 = isValidA ? contact.entityA.getComponent<ForceAccumulatorComponent>().inverseMass : 0;
		float invM2 = isValidB ? contact.entityB.getComponent<ForceAccumulatorComponent>().inverseMass : 0;

		float newSeperationVelocity = -contact.restitution * seperatingVelocity;
		float deltaVel = newSeperationVelocity - seperatingVelocity;

		float totalInverseMass = invM1 + invM2;
		if (totalInverseMass <= 0)
		{
			return;
		}

		float impulse = deltaVel / totalInverseMass;
		Vector3 impulsePerIMass = contact.normal * impulse;

		if (contact.entityA.hasComponent<ParticleComponent>())
		{
			contact.entityA.getComponent<ParticleComponent>().velocity += impulsePerIMass * invM1;
		}

		if (contact.entityB.hasComponent<ParticleComponent>())
		{
			contact.entityB.getComponent<ParticleComponent>().velocity += impulsePerIMass * invM2;
		}
	}
}