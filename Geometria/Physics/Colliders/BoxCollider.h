#pragma once

#ifndef BEHAVIOUR_H
#include "Behaviour.h"
#endif

#ifndef GEOMETRIA_H
#include "geometria.h"
#endif

#ifndef PHYSICSMANAGER_H
#include "../PhysicsManager.h"
#endif

#define BOXCOLLIDER_H
struct BoxCollider : public ScriptBehaviour
{
	AllowCloning(BoxCollider);

	physx::PxRigidStatic* boxStatic = nullptr;
	physx::PxRigidDynamic* boxDynamic = nullptr;

	physx::PxShape* boxShape = nullptr;

	Vector3 size = Vector3(1);
	Vector3 offset = Vector3(0);

	bool _isTrigger = false;

	bool _setTriggerOnUpdate = false;

	BoxCollider()
	{
	}

	BoxCollider(Vector3 getSize) : size(getSize)
	{
	}

	void OnStart();
	void OnInspector();
	void OnSave();
	void OnUpdate();
	void OnDestroy();
	void OnTransformChange(int value);
	void SetScale(Vector3 size);
	void SetOffset(Vector3 offset);
	void SetTrigger(bool t);
};
