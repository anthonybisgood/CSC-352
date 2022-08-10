/**
 * CSC 352
 * @file 3d.c
 * @author Anthony Bisgood anthonybisgood@email.arizona.edu
 * @brief This program creates and stores object base files (stl) from objects made of structs.
 * The user can choose to create objects made from triangles in a 3D coordinate plane. The objects in the 
 * scene are saved in memory. Each object is created through multiple triangles, and each triangle is 
 * given 3 coordinates in 3D space. 
 * @version 0.1
 * @date 2022-04-19
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "3d.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

/**
 * @brief Creates a new scne by callocing memory to the size of a Scene3D object, 
 * also allocates memory for the pointer array starting at size 5
 *
 * 
 * @return Scene3D* a pointer to a new scene
 */
Scene3D *Scene3D_create() {
    //Creates object array of size 5
    Object3D **object = calloc(5, sizeof(Object3D*));
    // creates new scene
    Scene3D *scene = calloc(1, sizeof(Scene3D));
    if (scene == NULL || object == NULL) {printf("FAILED TO ALLOCATE MEMORY\n"); exit(1);}
    scene -> objects = object;
    scene -> size = 5;
    scene -> count = 0;
    return scene;
}

/**
 * @brief frees the memory allocated in scene, including all objects, and any triangles 
 * that have been allocated memory in those objects
 * 
 * @param scene the seen we are freeing up in memory
 */
void Scene3D_destroy(Scene3D* scene) {
    Object3D **objects = scene -> objects;
    // frees all the memory allocated to objects in objects array
    for (int i = 0; i < scene->size; i++) {
        if (objects[i] != NULL) {
            // frees all the triangle nodes
            Triangle3DNode *root = objects[i] -> root;
            while (root != NULL) {
                Triangle3DNode *temp = root -> next;
                free(root);
                root = temp;
            }
        }
        free(objects[i]);
    }
    free(objects);
    free(scene);
}

/**
 * @brief Adds objects to the scene struct. if the array of objects in scene is full, then it 
 * will expand the array by 2* its current capacity
 * 
 * @param scene the scene we are adding an object to 
 * @param object the object we are adding to the scene
 */
void Scene3D_append(Scene3D* scene, Object3D* object) {
    long size = scene -> size;
    long count = scene -> count;
    if (count >= size - 1) {
        scene -> size = size * 2;
        // reallocs the objects pointer array to 2 times the current size * the size of a pointer
        Object3D **newList = realloc(scene -> objects, scene -> size * sizeof(Object3D*));
        if (newList == NULL) {printf("FAILED TO REALLOCATE MEMORY\n"); exit(1);}
        // sets field to new list
        scene -> objects = newList;
    }
    // frees the old list
    scene -> objects[count] = object;
    scene -> count += 1;
}

/**
 * @brief Writes the scenes with all their objects to the file name indicated in 
 * stl format. 
 * 
 * @param scene the scene we want to write onto the file 
 * @param file_name the file we are writing on
 */
void Scene3D_write_stl_text(Scene3D* scene, char* file_name) {
    FILE *fp = fopen(file_name, "w");
    if (fp == NULL) {printf("COULD NOT OPEN FILE: %s\n", file_name); fclose(fp); exit(1);}
    Object3D **object = scene -> objects;
    fprintf(fp, "solid scene\n");
    // for each object
    for (int i = 0; i < scene -> count; i++) {
        Object3D *currObj = object[i];
        Triangle3DNode *root = currObj -> root;
        while (root != NULL) {
            fprintf(fp, "  facet normal 0.0 0.0 0.0\n");
            fprintf(fp, "    outer loop\n");
            Triangle3D triangle = root -> triangle;
            fprintf(fp, "    vertex %0.5f %0.5f %0.5f\n", triangle.a.x, triangle.a.y, triangle.a.z);
            fprintf(fp, "    vertex %0.5f %0.5f %0.5f\n", triangle.b.x, triangle.b.y, triangle.b.z);
            fprintf(fp, "    vertex %0.5f %0.5f %0.5f\n", triangle.c.x, triangle.c.y, triangle.c.z);
            root = root -> next;
            fprintf(fp, "    endloop\n");
            fprintf(fp, "  endfacet\n");
        }
    }
    fprintf(fp, "endsolid scene\n");
    fclose(fp);
}

/**
 * @brief flips the array representing the verticies of the triangle if necessary
 * flip 0,1 1,0 2,3 3,2
 * 
 * @param array the array we are trying to flip
 */
void flipSides(Coordinate3D *array) {
    Coordinate3D temp = array[0];
    array[0] = array[1];
    array[1] = temp;
    temp = array[3];
    array[3] = array[2];
    array[2] = temp;
}

/**
 * @brief Get the Pyramid Vertices object. Stores the verticies in toRet, toRet[0,1,2,3] are the base
 * and Ret[5] is the height in respect to the width, height, orientation, and origin
 * 
 * @param origin the center of the base of the pyramid
 * @param orientation the way the pyramid is rotated/facing
 * @param toRet The verticies of the pyramid, in respect to width, height, and orientation
 * @param width the width of the base of the pyramid
 * @param height the height of the pyramid
 */
void getPyramidVertices(
Coordinate3D origin, char *orientation, Coordinate3D *toRet, double width, double height) {
    double orgX = origin.x;
    double orgY = origin.y;
    double orgZ = origin.z;
    // y cordinates stay on the same plane if up or down
    if (strcmp(orientation, "up") == 0|| strcmp(orientation, "down") == 0) {
        // sets the height of the point of the pyramid
        if (strcmp(orientation, "up") == 0) {toRet[4].y = orgY + height;}
        else {toRet[4].y = orgY - height;}
        // up orientation
        toRet[0].y = orgY;
        toRet[1].y = orgY;
        toRet[2].y = orgY;
        toRet[3].y = orgY;
        toRet[0].x = orgX - width/2;
        toRet[0].z = orgZ + width/2;
        toRet[1].x = orgX + width/2;
        toRet[1].z = orgZ + width/2;
        toRet[2].x = orgX - width/2;
        toRet[2].z = orgZ - width/2;
        toRet[3].x = orgX + width/2;
        toRet[3].z = orgZ - width/2;
        toRet[4].x = orgX;
        toRet[4].z = orgZ;
        if (strcmp(orientation, "down") == 0) {flipSides(toRet);}
    // z coords stay on the same plane, no moving back or forward
    } else if (strcmp(orientation, "forward") == 0|| strcmp(orientation, "backward") == 0) {
        if (strcmp(orientation, "forward") == 0) {toRet[4].z = orgZ + height;}
        else {toRet[4].z = orgZ - height;}
        // forward orientation
        toRet[0].z = orgZ;
        toRet[1].z = orgZ;
        toRet[2].z = orgZ;
        toRet[3].z = orgZ;
        toRet[0].x = orgX - width/2;
        toRet[0].y = orgY - width/2;
        toRet[1].x = orgX + width/2;
        toRet[1].y = orgY - width/2;
        toRet[2].x = orgX - width/2;
        toRet[2].y = orgY + width/2;
        toRet[3].x = orgX + width/2;
        toRet[3].y = orgY + width/2;
        toRet[4].x = orgX;
        toRet[4].y = orgY;
        if (strcmp(orientation, "backward") == 0) {flipSides(toRet);}
    } else if (strcmp(orientation, "left") == 0|| strcmp(orientation, "right") == 0) {
        if (strcmp(orientation, "left") == 0) {toRet[4].x = orgX - height;}
        else {toRet[4].x = orgX + height;}
        // left orientation
        toRet[0].x = orgX;
        toRet[1].x = orgX;
        toRet[2].x = orgX;
        toRet[3].x = orgX;
        toRet[0].z = orgZ - width/2;
        toRet[0].y = orgY - width/2;
        toRet[1].z = orgZ + width/2;
        toRet[1].y = orgY - width/2;
        toRet[2].z = orgZ - width/2;
        toRet[2].y = orgY + width/2;
        toRet[3].z = orgZ + width/2;
        toRet[3].y = orgY + width/2;
        toRet[4].z = orgZ;
        toRet[4].y = orgY;
        if (strcmp(orientation, "right") == 0) {flipSides(toRet);}
    }
}

/**
 * @brief Create a Triangle3D object using the cords given
 * 
 * @param cord1 a coodrinate struct of x,y,z
 * @param cord2 a coodrinate struct of x,y,z
 * @param cord3 a coodrinate struct of x,y,z
 * @return Triangle3D a Triangle3D object using the coordinates given
 */
Triangle3D createTriangle(Coordinate3D cord1, Coordinate3D cord2, Coordinate3D cord3) {
    Triangle3D node;
    node.a= cord1;
    node.b= cord2;
    node.c= cord3;
    return node;
}

/**
 * @brief Creates a pyramid object by calculating the verticies of the pyramid based 
 * on the origin, height, width, and orientation. Then, using the verticies, it 
 * creates 6 triangle objects with each corner being a verticy. Returns the pointer 
 * to the object that we saved on the heap
 * 
 * @param origin The center of the base of the pyramid
 * @param width the width of the base of the pyramid
 * @param height the height of the pyramid
 * @param orientation the way the pyramid is facing 
 * ("up", "down", "left", "right", "forward", "backward")
 * @return Object3D* a pointer to the new object we have created
 */
Object3D* Object3D_create_pyramid(
Coordinate3D origin, double width, double height, char* orientation) {
    Object3D *object = calloc(1, sizeof(Object3D));
    // origin always stays the same verticies[4] is the top
    Coordinate3D verticies[5];
    getPyramidVertices(origin, orientation, verticies, width, height);
    object -> root = calloc(1, sizeof(Triangle3DNode));
    Triangle3DNode *root = object -> root;
    Triangle3DNode *temp = root;
    temp -> triangle = createTriangle(verticies[0], verticies[2], verticies[3]);
    temp -> next = calloc(1, sizeof(Triangle3DNode));
    temp = temp -> next;
    temp -> triangle = createTriangle(verticies[0], verticies[1], verticies[3]);
    temp -> next = calloc(1, sizeof(Triangle3DNode));
    temp = temp -> next;
    temp -> triangle = createTriangle(verticies[0], verticies[1], verticies[4]);
    temp -> next = calloc(1, sizeof(Triangle3DNode));
    temp = temp -> next;
    temp -> triangle = createTriangle(verticies[1], verticies[3], verticies[4]);
    temp -> next = calloc(1, sizeof(Triangle3DNode));
    temp = temp -> next;
    temp -> triangle = createTriangle(verticies[3], verticies[2], verticies[4]);
    temp -> next = calloc(1, sizeof(Triangle3DNode));
    temp = temp -> next;
    temp -> triangle = createTriangle(verticies[2], verticies[0], verticies[4]);
    object -> count = 6;
    return object;
}

/**
 * @brief Calculates the vertex coordinates of the cuboid based on the origin, width, height, and
 * depth. Puts the correct vertecies into the array of coordinates
 * 
 * @param origin the very center of the cuboid
 * @param width the width of the cuboid
 * @param height the height of the cuboid
 * @param depth the depth of the cuboid
 * @param toRet this is the pointer to an array of Coordinate3D objects that 
 * we are putting the correct coordinates into.
 */
void getCuboidVertices(
Coordinate3D origin, double width, double height, double depth, Coordinate3D *toRet) {
    double orgX = origin.x;
    double orgY = origin.y;
    double orgZ = origin.z;
    toRet[0].x = orgX - width/2; toRet[0].y = orgY - height/2; toRet[0].z = orgZ - depth/2;
    toRet[1].x = orgX - width/2; toRet[1].y = orgY - height/2; toRet[1].z = orgZ + depth/2;
    toRet[2].x = orgX - width/2; toRet[2].y = orgY + height/2; toRet[2].z = orgZ - depth/2;
    toRet[3].x = orgX - width/2; toRet[3].y = orgY + height/2; toRet[3].z = orgZ + depth/2;
    toRet[4].x = orgX + width/2; toRet[4].y = orgY - height/2; toRet[4].z = orgZ - depth/2;
    toRet[5].x = orgX + width/2; toRet[5].y = orgY - height/2; toRet[5].z = orgZ + depth/2;
    toRet[6].x = orgX + width/2; toRet[6].y = orgY + height/2; toRet[6].z = orgZ - depth/2;
    toRet[7].x = orgX + width/2; toRet[7].y = orgY + height/2; toRet[7].z = orgZ + depth/2;
}

/**
 * @brief Creates a new cuboid object through triangles using the verticies calculated 
 * based on the origin, width, height, and depth values given. Then, returns 
 * a pointer to our new cube object.
 * 
 * @param origin the very center of the cuboid
 * @param width the width of the cuboid
 * @param height the height of the cuboid
 * @param depth the depth of the cuboid 
 * @return Object3D* 
 */
Object3D* Object3D_create_cuboid(
Coordinate3D origin, double width, double height, double depth) {
    Object3D *object = calloc(1, sizeof(Object3D));
    Coordinate3D verticies[8];
    getCuboidVertices(origin, width, height, depth, verticies);
    object -> root = calloc(1, sizeof(Triangle3DNode));
    Triangle3DNode *root = object -> root;
    Triangle3DNode *temp = root;
    // bottom
    temp -> triangle = createTriangle(verticies[0], verticies[1], verticies[4]);
    temp -> next = calloc(1, sizeof(Triangle3DNode));
    temp = temp -> next;
    temp -> triangle = createTriangle(verticies[1], verticies[4], verticies[5]);
    temp -> next = calloc(1, sizeof(Triangle3DNode));
    temp = temp -> next;
    // left
    temp -> triangle = createTriangle(verticies[0], verticies[1], verticies[2]);
    temp -> next = calloc(1, sizeof(Triangle3DNode));
    temp = temp -> next;
    temp -> triangle = createTriangle(verticies[1], verticies[2], verticies[3]);
    temp -> next = calloc(1, sizeof(Triangle3DNode));
    temp = temp -> next;
    // back
    temp -> triangle = createTriangle(verticies[0], verticies[2], verticies[4]);
    temp -> next = calloc(1, sizeof(Triangle3DNode));
    temp = temp -> next;
    temp -> triangle = createTriangle(verticies[2], verticies[4], verticies[6]);
    temp -> next = calloc(1, sizeof(Triangle3DNode));
    temp = temp -> next;
    // ri
    temp -> triangle = createTriangle(verticies[4], verticies[5], verticies[6]);
    temp -> next = calloc(1, sizeof(Triangle3DNode));
    temp = temp -> next;
    temp -> triangle = createTriangle(verticies[5], verticies[6], verticies[7]);
    temp -> next = calloc(1, sizeof(Triangle3DNode));
    temp = temp -> next;
    // forward
    temp -> triangle = createTriangle(verticies[1], verticies[3], verticies[5]);
    temp -> next = calloc(1, sizeof(Triangle3DNode));
    temp = temp -> next;
    temp -> triangle = createTriangle(verticies[3], verticies[5], verticies[7]);
    temp -> next = calloc(1, sizeof(Triangle3DNode));
    temp = temp -> next;
    //TOP
    temp -> triangle = createTriangle(verticies[2], verticies[3], verticies[6]);
    temp -> next = calloc(1, sizeof(Triangle3DNode));
    temp = temp -> next;
    temp -> triangle = createTriangle(verticies[3], verticies[6], verticies[7]);
    object -> count = 12;
    return object;
}