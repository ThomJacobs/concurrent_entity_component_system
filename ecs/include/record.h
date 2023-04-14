#pragma once

//Includes:
#include "type_definitions.h"

namespace np_ecs
{
	class World;
	class IComponent;

	struct ITask
	{
	public:
		//Attributes:
		entity_t entity;
		IComponent* component;
		flag_t component_flag;

		//Constructor:
		ITask(entity_t entity, flag_t flag, IComponent* component = nullptr);

		//Methods:
		virtual void process(World* world) = 0;
	};

	struct DeleteComponentTask : public ITask
	{
	public:
		DeleteComponentTask(entity_t entity, flag_t flag, IComponent* component = nullptr) : ITask(entity, flag, component) { }
		void process(World* world);
	};

	struct AddComponentTask : public ITask
	{
	public:
		AddComponentTask(entity_t entity, flag_t flag, IComponent* component = nullptr) : ITask(entity, flag, component) { }
		void process(World* world);
	};
}