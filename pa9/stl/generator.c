#include <stdio.h>
#include "3d.h"

int main(int argc, char **argv) {
    Scene3D* star = Scene3D_create();
    char* directions[] = {"up", "down", "left", "right", "forward", "backward"};
    Coordinate3D origin = (Coordinate3D){100, 100, 100};
    for (int i = 0; i <= 5; i ++) {
        Object3D* object = Object3D_create_pyramid(origin, 0, 0, directions[i]);
        Scene3D_append(star, object);
         Object3D* object = Object3D_create_cuboid(origin, 20,20,20);
        Scene3D_append(star, object);
    }
    origin = (Coordinate3D){-50, -50, -50};
    Object3D* object = Object3D_create_cuboid(origin, 20, 30, 100);
    Scene3D_append(star, object);
    Object3D* object = Object3D_create_pyramid(origin, 20, 30, directions[0]);
    Scene3D_append(star, object);
    Object3D* object = Object3D_create_pyramid(origin, 20, 30, directions[1]);
    Scene3D_append(star, object);
    Object3D* object = Object3D_create_pyramid(origin, 20, 30, directions[2]);
    Scene3D_append(star, object);
    Object3D* object = Object3D_create_pyramid(origin, 20, 30, directions[3]);
    Scene3D_append(star, object);
    origin = (Coordinate3D){130, 130, 130};
    Object3D* object = Object3D_create_cuboid(origin, 20, 30, 100);
    Scene3D_append(star, object);
    Object3D* object = Object3D_create_pyramid(origin, 20, 30, directions[1]);
    Scene3D_append(star, object);
    Object3D* object = Object3D_create_pyramid(origin, 20, 30, directions[5]);
    Scene3D_append(star, object);
    Object3D* object = Object3D_create_pyramid(origin, 20, 30, directions[4]);
    Scene3D_append(star, object);
    Object3D* object = Object3D_create_pyramid(origin, 20, 30, directions[2]);
    Scene3D_append(star, object);
    Scene3D_write_stl_text(star, "starFish.stl");
    Scene3D_destroy(star);
    return 0;
}