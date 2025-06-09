#pragma once
class GameScriptManager; 
class GameScript : public Component
{
public:
	static ComponentType GetType();
	GameScript();
	virtual ~GameScript();

	virtual void OnColliderEnter(PhysicsLit::RigidBodyPrimitive*) = 0;
	virtual void OnColliderStay(PhysicsLit::RigidBodyPrimitive*) = 0;
	virtual void OnColliderExit(PhysicsLit::RigidBodyPrimitive*) = 0;
};

