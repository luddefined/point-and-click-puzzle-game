#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H
 
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include <unordered_map>
#include <vector>
#include <random>
#include "string"

using string = std::string;

class SoundManager
{
    public:
    SoundManager();
    ~SoundManager();
    
    static inline constexpr float BASE_PITCH = 1.f;
    static inline constexpr float PITCH_RANGE = 0.05f;

    void addSound(string name, string path);
    void play(string name, float pitch = BASE_PITCH);
    void dispose();

    // Geta a randomised pitch value to use.
    inline float getRandomPitch(float base, float range)
    {
        static std::random_device rnd;
        static std::mt19937 gen(rnd());
        std::uniform_real_distribution<float> dist(base - range, base + range);
        float val = dist(gen);
        return val;
    }


private:
    std::vector<sf::SoundBuffer> buffers;
    std::unordered_map<string, sf::Sound> sounds;
    
};

#endif