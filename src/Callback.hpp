#ifndef CALLBACK_H
#define CALLBACK_H

#include <functional>
#include <vector>

// Simple callback object that can hold functions to invoke by other classes.
template<typename T>
class Callback
{
    // To handle clean-up the owner will just manually dispose this (hopefully that works...)
    // Could potentially add a remove function but this project has no need for it as the existence of sounds is contained in the 'scene'.
public:
    using func = std::function<void(T)>;

    // Add a new method to the callback.
    template<typename obj>
    void add(obj* object, void (obj::*func)(T))
    {
        methods.push_back(
            [object, func](T value)
            {
                (object->*func)(value);
            }
        );
    };

    // Invoke the methods added to this callback.
    void invoke(T value)
    {
        for (auto &&func : methods)
        {
            func(value);
        }
    }

    // Remove all subscriptions (God I Hope This Works).
    void dispose()
    {
        methods.clear();
    }

private:
    std::vector<func> methods;

};

#endif