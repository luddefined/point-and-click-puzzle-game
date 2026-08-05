#include "GameRoom.h"
#include <iostream>


GameRoom::~GameRoom()
{
    // This will clear the contents of the data structures
    // Likely not needed if the data structures used smart pointers but Oh Well 
    std::cout << "destroying\n";

    requestPlaySound.dispose();

    // Clear world_objects
    for (auto& pair : world_objects)
    {
        delete pair.second;
    }
    world_objects.clear();

    // Clear static_objects
    for (auto& object : static_objects)
    {
        delete object;
    }
    static_objects.clear();

    // dragged_key shouldn't have to be deleted since it only points to already existing memory
    // If I am wrong Lol
}
// Default update - loop through all world_objects and update them
// Should run before the room's update itself
void GameRoom::update() 
{
    // Iterate through all world objects and do their update 
    // std::cout << "Game room update\n";
    for (auto& pair : world_objects)
    {
        pair.second->update();
    }
}

int GameRoom::GetControlState()
{
    return current_control;
}

// To use in user input
// Does the check for key dragging
void GameRoom::inputDragKey()
{
    // Loop through objects at the mouse's position, try convert to a key and toggle its dragging

    Key* target_key = nullptr;
    sf::Vector2f target_point = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));

    // Get keys which are touching the button
    for (auto& name : GameRoom::getObjectsAtPoint(target_point))
    {
        // Check if there is a key (horrifically confused on what is a pointer and reference at this point btw)
        target_key = dynamic_cast<Key*>(world_objects[name]);
        if (!target_key) continue;

        // Have a key, so exit loop
        break;
    }

    // Start dragging the key
    if (target_key)
    {
        if (!target_key->draggable->drag_window) target_key->draggable->drag_window = &window;
        target_key->draggable->setDragged(true);
        dragged_key = target_key;
    }
}

// To use in user input
// Releases dragged key
void GameRoom::inputReleaseDraggedKey()
{
    if (dragged_key)
    {
        std::cout << "release\n";
        dragged_key->draggable->setDragged(false);
        dragged_key = nullptr;
    }
}

// To use in user input
// Try to interact with door (open it)
bool GameRoom::inputInteractWithDoor()
{
    // --- Get door at the user's mouse ---
    Door* target_door = nullptr;
    sf::Sprite* door_sprite = nullptr;
    sf::Vector2f target_point = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));

    // Loop through objects at the cursor and see if one of them is a door
    for (auto& name : GameRoom::getObjectsAtPoint(target_point))
    {
        // Check for door
        target_door = dynamic_cast<Door*>(world_objects[name]);
        if (!target_door) continue;

        // Beyond this point a door was fond
        // Retrieve the sprite of the door 
        // Try find another door if sprite cannot be obtained
        door_sprite = &target_door->getSprite();
        if (!door_sprite) continue;     

        // Door setup was successful - exit loop
        break;
    }
    // End if door does not exist
    if (!target_door) return false;   

    // Check if the door is already open
    // Exit if this is the case
    if (target_door->isOpen()) return true;

    // --- Get a key on the door and check if it touches ---
    Key* target_key;

    // Loop through world objects and see if any keys are touching the door
    for (auto& [_, object] : world_objects)
    {
        // Try convert object to key
        // If not a key, continue the loop
        target_key = dynamic_cast<Key*>(object);
        if (!target_key) continue;

        // Beyond this point a key exists
        // Check if its sprite exists and touches door
        // If key does not touch door try another key
        if (auto* key_sprite = &target_key->getSprite())
        {
            bool collides = key_sprite->getGlobalBounds().intersects(door_sprite->getGlobalBounds());
            if (!collides) continue;
        }
        // Sprite not found - try another key
        else continue;

        // Beyond this point a key touches the door
        // Try to open this door and return the result (for now)
        
        if (target_door->tryOpen(*target_key))
        {
            requestPlaySound.invoke("door");
            return true;
        }
    }

    // Beyond this point there was no key touching the door 
    return false;
}

// To use on user input
// Key merge function
// Returns the new key name
std::string GameRoom::inputMergekeys()
{
    // This will have the user merge keys which touch eachother
    // Only two keys will be merged. The order of the merged key reference depends on which key the user clicks first
    // The position of the key will be where the users mouse is

    std::vector<std::string> key_names;
    sf::Vector2f target_point = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
    Key* first_key = nullptr;
    Key* second_key = nullptr;
    sf::Sprite* first_sprite = nullptr;
    sf::Sprite* second_sprite = nullptr;

    // Check if cursor is on key
    for (auto& name : GameRoom::getObjectsAtPoint(target_point))
    {
        // Loop through objects until one of them is a key
        first_key = dynamic_cast<Key*>(world_objects[name]);
        if (!first_key) continue;
       
        // Check if this key is touching the cursor
        // If touching add to key names and exit loop
        // Otherwise clear pointer
        if (first_sprite = &first_key->getSprite())
        {
            bool touched = first_sprite->getGlobalBounds().contains(target_point);
            if (touched)
            {
                key_names.push_back(name);
                break;
            }
            else
            {
                first_sprite = nullptr;
            }
        }
    }
    // If there is no key then exit
    if (!first_key) return "";

    // Beyond this point a valid key is found
    // Check if two keys are touching 
    for (auto& [name, object] : world_objects)
    {
        second_key = dynamic_cast<Key*>(object);
        if (!second_key) continue;

        // If second key is the same as first, try another key
        if (second_key == first_key)
        {
            second_key = nullptr;
            continue;
        }

        if (&second_key == &first_key)
        {
            second_key = nullptr;
            continue;
        }

        // If this key cannot be merged, try another key
        std::string mask1 = first_key->merge_mask;
        std::sort(mask1.begin(), mask1.end());
        std::string mask2 = second_key->merge_mask;
        std::sort(mask2.begin(), mask2.end());
        if (mask1 != mask2)
        {
            second_key = nullptr;
            continue;
        }

        // Check if the keys are touching
        // If touching add name to vector
        // Otherwise clear pointer
        if (second_sprite = &second_key->getSprite())
        {
            bool collides = first_sprite->getGlobalBounds().intersects(second_sprite->getGlobalBounds());
            if (collides)
            {
                key_names.push_back(name);
                break;
            }
            else
            {
                second_sprite = nullptr;
            }
        }
    }
    // If there is no second key (touching key) then exit
    if (!second_key) return "";

    // Beyond this point there are two keys to combine
    // Create new key name and try create new key
    if (key_names.size() != 2) return "";   // For some reason it'll say two sprites are touching when they aren't and for some reason this fixes it
    std::string new_name = key_names[0] + key_names[1];
    bool merge_attempt = tryMergeObjectsIntoOneKey(key_names, new_name);

    // If merge was successful return the new key name for future use
    // Otherwise return an empty string
    if (merge_attempt)
    {
        world_objects[new_name]->getSprite().setPosition(target_point);
        return new_name;
    }
    else return "";
}

// Activates on right click
// Changes whatever the left click does
void GameRoom::inputChangeControls()
{
    inputReleaseDraggedKey();
    if (++current_control > 3) current_control = 1;
    std::cout << "Current controls : " << current_control << std::endl;
}

// Try to combine objects in world_objects into a singular object (Key). May not always create the key if objects are not valid entries
// Returns whether it was able to merge objects. The key_reference of the new key is dependant on the order of objects
bool GameRoom::tryMergeObjectsIntoOneKey(std::vector<std::string> objects, std::string name)
{
    // Assumes that all objects are valid key names in world_objects
    // If this block fails, return false
    try
    {
        // New key reference. If this remains unchanged throughout loop then return false
        std::string new_key_ref;
        bool created = false;

        // Delete entries in world_objects and get their key_reference
        for (std::string& identifier : objects)
        {
            // Delete object in world_objects
            // Add to the new key reference 
            GameObject* object = world_objects[identifier];
            if (Key* old_key = dynamic_cast<Key*>(object))
            {
                new_key_ref += old_key->key_reference;
                delete old_key;
                world_objects.erase(identifier);

                bool created = true;
            }
        }

        // No key made - exit loop
        if (created) return false;

        // Create a new key
        world_objects[name] = new Key();
        static_cast<Key*>(world_objects[name])->key_reference = new_key_ref;
        requestPlaySound.invoke("merge");
        return true;
    }
    catch (std::exception &error)
    {
        // Unable to create key for whatever reason
        // Output error
        std::cout << error.what() << std::endl;
        return false;
    }
}

// Returns names of objects which are touching point
std::vector<std::string> GameRoom::getObjectsAtPoint(sf::Vector2f point)
{
    // Vector of object names in world_objects 
    // Each element is for an object which touches
    std::vector<std::string> touched;
    
    // Loop through world objects and compare positions to point
    for (auto& pair : world_objects)
    {
        // If sprite exists
        if (auto* sprite = &pair.second->getSprite())
        {
            // If point is in sprite, add its name to touched objects
            if (sprite->getGlobalBounds().contains(point))
            {
                touched.push_back(std::ref(pair.first));
            }
        }
    }

    return touched;
}

// It's about to get crazy
// Returns an ordered vector of game object references to draw
// The first element is the smallest number, the last element is the largest number
std::vector<std::reference_wrapper<GameObject>> GameRoom::updateRenderOrder()
{
    std::vector<std::reference_wrapper<GameObject>> render_order;
    
    // Add all elements to the render_order
    for (auto& object : static_objects)
    {
        render_order.push_back(std::ref(*object));
    }
    for (auto& pair : world_objects)
    {
        render_order.push_back(std::ref(*pair.second));
    }

    // Reorder the render order
    std::sort(render_order.begin(), render_order.end(), [](const GameObject& a, const GameObject& b)
        { return a.layer < b.layer; });

    return render_order;
}



