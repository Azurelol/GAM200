#pragma once
#include "Object.h"

#include "..\Objects\Entity.h"

namespace DCEngine {

  class Module : public Object {
  public:
    Module(std::string name);
    ~Module();
    EntityVec ActiveDelegateHolders; 

  private:

  };
  

}