#include "SpriteData.h"

void SpriteData::setImage(std::string file)
{
    texture_loaded = false;
    source_image = file;
}

bool SpriteData::load()
{
    if (texture_loaded) return false;
    return texture_loaded = texture.loadFromFile(source_image);
}

bool SpriteData::load(std::string file)
{
    if (texture_loaded) return false;
    source_image = file;
    return texture_loaded = texture.loadFromFile(source_image);
}
