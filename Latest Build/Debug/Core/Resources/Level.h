/*****************************************************************************/
/*!
\file   Level.h
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   8/5/2015
\brief  A level is a resource that stores a set of GameObjects that can be loaded
        into a space.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#pragma once
#include "ResourceReference.h"
#include "..\Objects\Entities\GameObject.h"

namespace DCEngine {
  
  class Level : public Resource {
  public:
    Level(std::string name);
    Level(std::string name, std::string serializedData);
    ~Level();
    static std::string Extension() { return ".Level"; }
    //void Save(std::string& serializedData);
    bool Load();
    const std::string& Get();

    void AddGameObject(GameObjectPtr gameObject);
    GameObjectVec GameObjects; //!< Container for entities in the level.   

  private:

  };
  
  using LevelPtr = std::shared_ptr<Level>;
  using LevelHandle = std::string;

}
