#include "SoundManager.h"
#include <iostream>
#include <format>

SoundManager::SoundManager()
{
    
}

SoundManager::~SoundManager()
{
    dispose();
}

void SoundManager::addSound(string name, string path)
{
    auto* buffer = new sf::SoundBuffer;
    if (!buffer->loadFromFile(path)) return;
    
    buffers.emplace_back(buffer);
    sounds.emplace(name, sf::Sound(*buffer));
}

void SoundManager::play(string name, float pitch)
{
    if (!sounds.contains(name)) return;
    sounds[name].setPitch(pitch);
    sounds[name].play();
}


// Remove all loaded sounds.
void SoundManager::dispose()
{
    sounds.clear();
    buffers.clear();
}

// Get a sound loaded from the manager.
sf::Sound* SoundManager::operator[](string item)
{
    return &sounds[item];
}
