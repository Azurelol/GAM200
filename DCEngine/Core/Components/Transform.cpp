#include "Transform.h"
#include "EngineReference.h"

namespace DCEngine {

  void Transform::Initialize() {
    if (TRACE_COMPONENT_INITIALIZE)
      trace << Owner()->Name() << "::" << ObjName << "::Initialize\n";
    WorldRotation.z = 0;
  }

  void Transform::Serialize(Json::Value & root) {
  }

  void Transform::Deserialize(Json::Value & root) {
  }
}