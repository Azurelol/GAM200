#include "PlayerController.h"
#include "../../CoreComponents.h"

namespace DCEngine {

	void PlayerController::Initialize()
	{
		Connect(Daisy->getMouse(), Events::MouseDown, PlayerController::OnMouseDownEvent);
		Connect(Daisy->getMouse(), Events::MouseUp, PlayerController::OnMouseUpEvent);
		Connect(Daisy->getKeyboard(), Events::KeyDown, PlayerController::OnKeyDownEvent);
		Connect(SpaceRef, Events::LogicUpdate, PlayerController::OnLogicUpdateEvent);
		TransformRef = dynamic_cast<GameObject*>(owner_)->getComponent<Transform>(); // ew
		RigidBodyRef = dynamic_cast<GameObject*>(owner_)->getComponent<RigidBody>();
	}

	void PlayerController::Serialize(Json::Value & root)
	{
	}

	void PlayerController::Deserialize(Json::Value & root)
	{
	}

	void PlayerController::OnMouseDownEvent(Events::MouseDown * event)
	{

	}
	void PlayerController::OnMouseUpEvent(Events::MouseUp * event)
	{

	}

	void PlayerController::OnKeyDownEvent(Events::KeyDown* event) 
	{
		switch (event->Key) 
		{

		case Keys::W:
			RigidBodyRef->setVelocity(RigidBodyRef->getVelocity() + Real3(0, JumpPower, 0));
			PrintTranslation();
			break;
		case Keys::A:
			RigidBodyRef->setVelocity(RigidBodyRef->getVelocity() + Real3(-MoveSpeed, 0, 0));
			PrintTranslation();
			break;
		case Keys::D:
			RigidBodyRef->setVelocity(RigidBodyRef->getVelocity() + Real3(MoveSpeed, 0, 0));
			PrintTranslation();
			break;
		}
	}

	void PlayerController::OnLogicUpdateEvent(Events::LogicUpdate * event)
	{

	}



	void PlayerController::PrintTranslation()
	{
		//trace << Owner()->Name() << "::Transform.Translation(" << TransformRef->Translation.x	<< ", " << TransformRef->Translation.y << ", " << TransformRef->Translation.z << ")\n";
	}

	void PlayerController::PrintVelocity()
	{
		Real3 vel = RigidBodyRef->getVelocity();
		//trace << Owner()->Name() << "::RigidBody.Velocity(" << vel.x << ", " << vel.y<< ", " << vel.z << ")\n";
	}
}