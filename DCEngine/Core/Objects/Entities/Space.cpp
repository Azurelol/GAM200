/******************************************************************************/
/*!
\file   Space.cpp
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   7/30/2015
\brief  Spaces are containers for game objects. When a level is loaded into
a space, all the game objects from that level are put into the space.
Each space has its own instances of the core systems of the engine.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/

#include "Space.h"

// Space-level components
#include "../../Components/GraphicsSpace.h"
#include "../../Components/PhysicsSpace.h"
#include "../../Components/TimeSpace.h"
#include "../../Components/SoundSpace.h"
#include "../../Components/CameraViewport.h"

// Binding needs to know what a GameSession is for the Space CTOR.
#include "GameSession.h"
#include "../../Engine/Engine.h"

namespace DCEngine {
    

  
  /**************************************************************************/
  /*!
  \brief  Constructor for the Space class.
  */
  /**************************************************************************/
  Space::Space(std::string name, GameSession& gamesession) : Entity(name), GameSessionRef(&gamesession) {
    if (TRACE_ON && TRACE_CONSTRUCTOR)
      DCTrace << ObjectName << "::Space - Constructor \n";
    type_ = EntityType::Space;
  }
  
  /**************************************************************************/
  /*!
  \brief  Destructor. Clears the entities vector.
  */
  /**************************************************************************/
  Space::~Space() {        
    
  }

  /**************************************************************************/
  /*!
  \brief  Initializes the space.
  */
  /**************************************************************************/
  void Space::Initialize() {
    DCTrace << "|" << ObjectName << "::Initialize| \n";

    // Add Space-type components
    AddComponent<SoundSpace>();
    AddComponent<TimeSpace>();
    AddComponent<PhysicsSpace>();
    AddComponent<CameraViewport>();
    AddComponent<GraphicsSpace>();
    //AddComponent(ComponentPtr(new SoundSpace(*this)));
    //AddComponent(ComponentPtr(new TimeSpace(*this)));
    //AddComponent(ComponentPtr(new PhysicsSpace(*this)));
    //AddComponent(ComponentPtr(new CameraViewport(*this)));
    //AddComponent(ComponentPtr(new GraphicsSpace(*this)));

    // Initialize Space-components
    for (auto &component : ComponentsContainer) {
      component->Initialize();
    }        

    DCTrace << "[" << ObjectName << "::Initialize - Initializing all GameObjects...] \n";
    // Initialize all entities (in effect, initializing all attached components)
    for (auto gameObject : GameObjectContainer) {
      // TEMPORARY: Should space, gamesession be even set this way?
      //            Should they not be set on the constructor?
      //            The problem is the Level object is constructing them,
      //            at the moment.
      gameObject->SpaceRef = this;
      gameObject->GamesessionRef = GameSessionRef;
      gameObject->Initialize();
    }
  }

  /**************************************************************************/
  /*!
  \brief  Terminates the Space.
  */
  /**************************************************************************/
  void Space::Terminate()
  {
    DestroyAll();
  }

  /**************************************************************************/
  /*!
  \brief  Updates every system in the space.
  */
  /**************************************************************************/
  void Space::Update(float dt) {
    if (TRACE_ON && TRACE_UPDATE)
      DCTrace << ObjectName << "::Update \n";
  }

  /**************************************************************************/
  /*!
  @brief Serializes a Space.
  @param builder A reference to the JSON builder.
  @note  This will serialize the entity's properties, then its components.
  */
  /**************************************************************************/
  void Space::Serialize(Zilch::JsonBuilder & builder)
  {
    // Serialize the underlying Entity object, which includes its components.
    Entity::Serialize(builder);
    // Grab a reference to the Zilch Interface
  }

  /**************************************************************************/
  /*!
  @brief Deserializes the space.
  @param properties A pointer to the object containing the properties.
  @note  This will deserialize the space's properties, then its components.
  */
  /**************************************************************************/
  void Space::Deserialize(Zilch::JsonValue * properties)
  {

  }


  /**************************************************************************/
  /*!
  @brief  Destroy all the objects in the space.
  @note   
  */
  /**************************************************************************/
  void Space::DestroyAll()
  {
    if (TRACE_GAMEOBJECT_ADD)
      DCTrace << ObjectName << "::DestroyAll - Removing all objects from the space.\n";
    // For every GameObject in the space
    for (auto object : GameObjectContainer) {
      // Mark the object for destruction on next frame
      DCTrace << " - " << object->Name() << "\n";
      Daisy->getSystem<Systems::Factory>()->MarkGameObject(*object);
      //RemoveObject(*object);
    }
    GameObjectContainer.clear();

  }

  /**************************************************************************/
  /*!
  @brief  Saves a level, creating a Level resource.
  @param  level A reference to the string containing the level name.
  @return A string containing the serialized level data as JSON.
  */
  /**************************************************************************/
 LevelPtr Space::SaveLevel(const std::string & level)
  {
    return Daisy->getSystem<Systems::Factory>()->BuildLevel(level, *this);
  }

  /**************************************************************************/
  /*!
  @brief  Loads a sample, hard-to-the-coded level.
  */
  /**************************************************************************/
  void Space::LoadSampleLevel(LevelPtr level)
  {
    if (TRACE_ON)
      DCTrace << ObjectName << " Space::LoadLevel - Loading " << level->Name() << " level.\n";

    // Set it as the current level
    CurrentLevel = level;

    DestroyAll();

    // Load GameObjects into the space
    for (auto gameObject : CurrentLevel->GameObjects) {
      AddObject(gameObject);
    }

    // Initialize every GameObject
    for (auto gameObject : GameObjectContainer) {
      gameObject->Initialize();
    }
  }

  /**************************************************************************/
  /*!
  @brief Loads a level, a container for entities, into the space. 
  @todo  Clean up comments.
  */
  /**************************************************************************/
  void Space::LoadLevel(LevelPtr level) {
    if (TRACE_ON)
      DCTrace << ObjectName << " Space::LoadLevel - Loading " << level->Name() << " level.\n";
      
    // If a NULL ptr was passed
    if (!level) {
      DCTrace << ObjectName << " Space::LoadLevel - Invalid level pointer \n";
    }

    // Clear the current objects from the space
    DestroyAll();
    // Set it as the current level
    CurrentLevel = level;
    // Build all the GameObjects from the level
    Daisy->getSystem<Systems::Factory>()->BuildFromLevel(level, *this);

    //return;
    // Deserialize the space
    // Clear its current components
    // Load new ones from the archetype
  }

  /**************************************************************************/
  /*!
  @brief  Loads a level, a container for entities, into the space.
  @param  level A handle to the level resource.
  */
  /**************************************************************************/
  void Space::LoadLevel(std::string & level)
  {
    DCTrace << ObjectName << " Space::LoadLevel - Loading " << level << "\n";

    // Request the Content system for a pointer to the specified level resource
    auto levelPtr = Daisy->getSystem<Systems::Content>()->getLevel(level);
    LoadLevel(levelPtr);
  }

  /**************************************************************************/
  /*!
  \brief  Reloads the current level.
  */
  /**************************************************************************/
  void Space::ReloadLevel()
  {
    // First, destroy all current objects in the space
    DestroyAll();
    // Load the level again
    LoadLevel(CurrentLevel);
  }



  /**************************************************************************/
  /*!
  @brief  Creates a GameObject, adds it to the space.
  @return A pointer to the GameObject that was added.
  @note   This function requests the GameObject to be created through
          the object factory.
  */
  /**************************************************************************/
  GameObjectPtr Space::CreateObject() {
    // Calls the object factory to create the object from an archetype
    auto gameObject = Daisy->getSystem<Systems::Factory>()->CreateGameObject("Transform", *this, true);
    GameObjectContainer.push_back(gameObject);
    return gameObject;
  }

  /**************************************************************************/
  /*!
  @brief  Creates a GameObject from an Archetype and adds it to the space.
  @return A pointer to the GameObject that was added.
  \note   This function requests the entity to be created through
  the object factory.
  */
  /**************************************************************************/
  GameObjectPtr Space::CreateObject(ArchetypePtr archetype)
  {
    // Creates the GameObject from an Archetype
    auto gameObject = Daisy->getSystem<Systems::Factory>()->CreateGameObject(archetype, *this, true);
    //GameObjectContainer.push_back(gameObject);
    return gameObject;
  }

  /**************************************************************************/
  /*!
  \brief  Finds a GameObject in the current space and returns a pointer to it.
  \return A pointer to the GameObject that was added. NULL if the GameObject
          was not found.
  */
  /**************************************************************************/
  GameObject* Space::FindObjectByName(const std::string name) {
    // Search through the space's gameobjects
    for (auto gameObj : GameObjectContainer) {
      if (gameObj->Name() == name)
        return gameObj;
    }
    // No match was found.
    return NULL;
  }


  /**************************************************************************/
  /*!
  @brief  Returns a container containing pointers to all the GameObjects
          in the space.
  @return A reference to the container of active GameObject.
  */
  /**************************************************************************/
  GameObjectVec* Space::AllObjects()
  {
    return &GameObjectContainer;
  }

  /**************************************************************************/
  /*!
  @brief  Adds an entity directly to the space.
  */
  /**************************************************************************/
  void Space::AddObject(GameObjectPtr gameObject) {
    GameObjectContainer.push_back(gameObject);
    //ChildrenContainer.push_back(dynamic_cast<Entity*>(gameObject.get()));

    if (TRACE_GAMEOBJECT_ADD)
      DCTrace << ObjectName << "::AddObject - Added " << gameObject->Name() << " to the space.\n";
  }

  /**************************************************************************/
  /*!
  @brief  Removes the GameObject from the Space.
  @param  A reference to the GameObject.
  */
  /**************************************************************************/
  void Space::RemoveObject(GameObject & gameObj)
  {
    // Remove the GameObject from the space's list of GameObjects
    for (auto gameObjPtr : GameObjectContainer) {
      if (gameObjPtr == &gameObj) {
        std::swap(gameObjPtr, GameObjectContainer.back());
        GameObjectContainer.pop_back();
        if (TRACE_GAMEOBJECT_ADD)
          DCTrace << ObjectName << "::RemoveObject - Removed " << gameObj.Name() << " from the space.\n";
        break;
      }
    }

    // Mark the object for destruction on next frmae
    Daisy->getSystem<Systems::Factory>()->MarkGameObject(gameObj);


  }

  
  GameSession& Space::getGameSession() {
    return *GameSessionRef;
  }

} // DCEngine