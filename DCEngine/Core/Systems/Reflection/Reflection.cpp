#include "Reflection.h"

namespace DCEngine {
  
  namespace Systems {

    /**************************************************************************/
    /*!
    @brief  Reflection system constructor.
    */
    /**************************************************************************/
   Reflection::Reflection() : System(std::string("ReflectionSystem"), EnumeratedSystem::Reflection) {
     DCTrace << "*Using Zilch to bind Object classes and provide reflection \n";
     ZilchHandler.reset(new ZilchInterface());
    }

   /**************************************************************************/
   /*!
   @brief  Initializes the Reflection system.
   */
   /**************************************************************************/
    void Reflection::Initialize()
    {
      ZilchHandler->Initialize();
    }

    /**************************************************************************/
    /*!
    @brief  Updates the Reflection system.
    */
    /**************************************************************************/
    void Reflection::Update(float dt)
    {
    }

    /**************************************************************************/
    /*!
    @brief  Terminates the Reflection system.
    */
    /**************************************************************************/
    void Reflection::Terminate()
    {
      ZilchHandler->Terminate();
    }


    /**************************************************************************/
    /*!
    @brief  Returns a container containing all the bound components
    in the engine.

    */
    /**************************************************************************/
    std::vector<Zilch::BoundType*> Reflection::AllComponents()
    {
      std::vector<Zilch::BoundType*> componentTypes;

      // Loop through every library
      for (auto library : Handler()->Dependencies.all()) {
        // Grab a container of all the bound types in the library
        auto types = library->BoundTypes.all();
        // For every type in the library
        ZilchForEach(auto type, types) {
          // If the type is a component
          if (Zilch::TypeBinding::IsA(type.second, Component::ZilchGetStaticType()))
            componentTypes.push_back(type.second);
        }
      }
      return componentTypes;
    }
    
    ZilchInterface* Reflection::Handler()
    {
      return ZilchHandler.get();
    }




  }


}