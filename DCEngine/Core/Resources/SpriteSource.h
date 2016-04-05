/*****************************************************************************/
/*!
@file   SpriteSource.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   10/04/2015
@brief  The SpriteSource resource encapsulates an image file amd the texture
        data generated from it.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#pragma once
#include "ResourceReference.h"

#define USE_SFML_TEXTURE 0

#if (USE_SFML_TEXTURE)
#include <SFML\Graphics\Texture.hpp>
#else
#include "Texture2D.h"
#endif

namespace DCEngine {

  class SpriteSource;
  using SpriteSourceRawPtr = SpriteSource*;
  using SpriteSourcePtr = std::shared_ptr<SpriteSource>;

  class SpriteSource : public Resource {
  public:

    #if(DCE_USE_ZILCH_INTERNAL_BINDING) 
    ZilchDeclareDerivedType(SpriteSource, Resource);
    #endif

    // Generated by Graphics System
    // Texture cut
    int MinX = 0;
    int MaxX = 0;
    int MinY = 0;
    int MaxY = 0;
    // PicWidth and PicHeight already initialized. Do not change it.
    int PicWidth = 0;
    int PicHeight = 0;

    // Framerate
    float FrameRate = 0.0f;
    float PixelsPerUnit = 0.0f;
    //Animation    
    int ColumnCount = 1;
    int RowCount = 1;
    int TotalFrame = 1;
    bool Smoothing = false;
    bool Looping = false;
    bool Fill = false; // Should be enum

    // Properties
    DCE_DEFINE_PROPERTY(String, AssetPath);
    DCE_DEFINE_PROPERTY(float, FrameRate);
    DCE_DEFINE_PROPERTY(float, PixelsPerUnit);
    DCE_DEFINE_PROPERTY(int, ColumnCount);
    DCE_DEFINE_PROPERTY(int, RowCount);
    DCE_DEFINE_PROPERTY(bool, Smoothing);
    DCE_DEFINE_PROPERTY(bool, Looping);
    DCE_DEFINE_PROPERTY(bool, Fill);
    DCE_DEFINE_PROPERTY(int, TotalFrame);

    // Methods
    static SpriteSourceRawPtr Find(std::string);
    SpriteSource(std::string spriteFile);
    static std::string Extension() { return ".SpriteSource"; }
    void LoadTexture();    

    #if (USE_SFML_TEXTURE)
    sf::Texture& getTexture();
    #else
    Texture2D& getTexture();
    #endif   
    

  private:
    std::string AssetPath;
    #if (USE_SFML_TEXTURE)
    std::unique_ptr<sf::Texture> TextureObj;
    #else
    std::unique_ptr<Texture2D> TextureObj;
    #endif
    
    
    

  };

  //using SpriteSourcePtr = SpriteSource*;



}