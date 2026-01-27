#include "world.h"
#include "raylib.h"
#include "string.h"
#include "planet.h"
#include "stdlib.h"
#include <stdio.h>
#include "raymath.h"

World world_create(Planet *planets, int planet_count)
{
    World world = {
        .planets = planets,
        .planet_count = planet_count,
        .gravity_strength = 50.0f,
        .deltaTime = 0.1f,
    };

    world.count_g = 0;
    world.count_ng = 0;

    for (int i = 0; i < world.planet_count; i++) {
        if(world.planets[i].has_gravity == true) {
            world.gravity_planet_indexs[world.count_g++] = i;
        }

        if(world.planets[i].has_gravity == false) {
            world.non_gravity_planet_indexs[world.count_ng++] = i;
        }
    }

    Vector3 up = (Vector3){0.0f, 1.0f, 0.0f};
    int sun = world.gravity_planet_indexs[0];

    for(int j = 0; j < world.count_ng; j++) {
        int p = world.non_gravity_planet_indexs[j];
        Vector3 r = Vector3Subtract(world.planets[p].position, world.planets[sun].position);

        float distance = Vector3Length(r);
        Vector3 perpendicular_direction = Vector3Normalize(Vector3CrossProduct(up, r)); 

        float speed = sqrt(world.gravity_strength * world.planets[sun].mass / distance);
        world.planets[p].velocity = Vector3Scale(perpendicular_direction, speed);
        world.planets[p].acceleration = (Vector3) {0, 0, 0};
   }

    return world;
}

void world_draw(World *world) {
    for (int i = 0; i < world->planet_count; i++) {
        planet_draw(&world->planets[i]);
    }
}

void world_update(World *world)
{
    Vector3 direction;
    float distance;
    world->count_g = 0;
    world->count_ng = 0;

    for (int i = 0; i < world->planet_count; i++) {
        planet_update(&world->planets[i]); // This is visual only
        if(world->planets[i].has_gravity == true) {
            world->gravity_planet_indexs[world->count_g++] = i;
        }

        if(world->planets[i].has_gravity == false) {
            world->non_gravity_planet_indexs[world->count_ng++] = i;
        }
    }


    for(int i = 0; i < world->count_ng; i++) {
        Vector3 total_force = (Vector3){0.0f, 0.0f, 0.0f};
        int p = world->non_gravity_planet_indexs[i];
        Vector3 old_acceleration = world->planets[p].acceleration;
        Vector3 r;

        for(int j = 0; j < world->count_g; j++) {

            int g = world->gravity_planet_indexs[j];
            r = Vector3Subtract(world->planets[g].position, world->planets[p].position);
            direction = Vector3Normalize(r);
            distance = Vector3Length(r);

            if (distance < 0.0001f) { // stop divide by zero errors
                continue;
            }

            float strength = world->gravity_strength * world->planets[g].mass / (distance * distance);

            Vector3 force = Vector3Scale(direction, strength);
            total_force = Vector3Add(total_force, force);

        }
        world->planets[p].acceleration = Vector3Scale(total_force, 1/world->planets[p].mass);
        world->planets[p].position = Vector3Add(world->planets[p].position, Vector3Add(Vector3Scale(world->planets[p].velocity, world->deltaTime), Vector3Scale(old_acceleration, (0.5f * world->deltaTime * world->deltaTime))));
        world->planets[p].velocity = Vector3Add(world->planets[p].velocity, Vector3Scale(Vector3Add(old_acceleration, world->planets[p].acceleration), world->deltaTime * 0.5f));
    }
}

void world_destroy(World *world) {
    for(int i = 0; i < world->planet_count; i++) {
        planet_destroy(&world->planets[i]);
    }
    
    free(world->planets);
    world->planets = NULL;
}