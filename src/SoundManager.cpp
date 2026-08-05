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
    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile(path))
    {
        return;
    }
    sf::Sound sound;
    sound.setBuffer(buffer);
    
    buffers.emplace_back(buffer);
    sounds.emplace(name, sound);
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

