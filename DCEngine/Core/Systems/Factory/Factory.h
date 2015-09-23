/*****************************************************************************/
/*!
\file   Factory.h
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   9/20/2015
\brief  The Factory system handles the creation of all game objects and 
        components, from serialized data. 
*/
/******************************************************************************/
#pragma once
#include "../System.h"

// Libraries
#include <set>
// Headers
#include "Serializer.h"
#include "../../Objects/ObjectsInclude.h"
#include "../../Objects/Entities/EntitiesInclude.h"
#include "../../ComponentsInclude.h"

namespace DCEngine {

  // Forward declarations
  class Engine;
  
  namespace Systems {

    class Factory : public System {    
      friend class Engine;

    public:            
      
      GameObjectPtr CreateGameObject(Space& space, bool init); //!< Creates a default gameObj
      GameObjectPtr CreateGameObject(const std::string& gameObjName, const Space& space, bool init);
      
      ComponentPtr CreateComponent(const std::string& compName, bool init);

      ResourcePtr CreateResource(const std::string& resourceName, bool init);

      void DestroyGameObject(GameObject& gameObj);
      void DestroyAllObjects(); //!< Destroys all objects

      void LoadResources(); //!< Load project resources into different maps
      void LoadLevel(std::string& levelName);

      // !< When components are constructed they register themselves to the map.
      template <typename ComponentClass> void RegisterComponent(std::string& componentName); 


    private:

      unsigned LastGameObjectId; //!< Incrementally generate unique IDs
      std::map<std::string, std::type_index> ComponentClassMap; 
      

      GameObjectVec gameObjVec; //!< Container of active GameObjects
      ComponentVec componentVec; //!< Container of active Components


      std::set<GameObject*> gameObjsToBeDeleted; 

      /* Functions */
      Factory();
      void Initialize();
      void Update(float dt); //!< Delete all objects in the to-be-deleted list
      void Terminate();
      GameObjectPtr BuildAndSerialize(const std::string& fileName);
      void DeserializeLevel(const std::string& levelName);  //!< Loads a level, from a level map

      template <typename ComponentClass> ComponentPtr CreateComponent(Entity& owner, bool init);


    }; 

    /* Templates */
    template<typename ComponentClass>
    inline ComponentPtr Factory::CreateComponent(Entity& owner, bool init)
    {
      //return new(ComponentClass)
    }
  }

  using FactoryPtr = std::unique_ptr<Systems::Factory>;
}