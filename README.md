# OOP_Project

## Idea
The main Idea of this project is to have a self-driving car. The user sets specific information about the world (or stays with the default ones) and then chooses the location they want the car to get to.

## Description
First off notes of what I have NOT implemented
    - Confidence
    - Move to multiple destinations
    - The whole Navigation System is part of the car, not an external part
Everything else has been done, the car sees objects, moves accordingly etc. except for one small issue, the car can *visually* "eat" an object and then make it appear again. In reality, the Object is there but because of the way I make the car move, it acts as if it gets rid of the object. This is only prominent with Bikes and Cars as Traffic Lights, Stop signs and Parked Vechiles don't move. PS I'm not sure if I understood ticks correctly.

To make the code easier to read and follow, I have seperated it into 4 main parts:
    • Vector2.h: What was used for positions and directions. It's basically just a 2D vector similar to how its defined in math
    • WorldObject.h: Where I define every class except (AutomaticCar and World)
    • World.h: Where I define the world and the way it updates independent of the AutomaticCar
    • ACar.h: Where I define the car and everything about it
        - Different classes for sensors

The project.cpp just brings it to life. While not completely user-friendly, it works and is crystal clear.