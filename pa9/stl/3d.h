/**
 * File: 3d.h
 * Author: Benjamin Dicken
 * Course: CS 352 at the UofA
 * Description:
 * A C library for creating 3D shapes and scenes.
 * This header includes several structs needed for representing the 3D shapes.
 * It also contains the prototypes for several of the public-facing functions.
 */

#ifndef THREE_D_H
#define THREE_D_H

/**
 * Represents a Cartesian coordinate (x,y,z) in 3D space.
 */
typedef struct Coordinate3D {
  double x;
  double y;
  double z;
} Coordinate3D;

/**
 * Represents a triangle in 3D Space.
 * a, b, and c represent the locations of each corner of the triangle.
 */
typedef struct Triangle3D {
  Coordinate3D a;
  Coordinate3D b;
  Coordinate3D c;
} Triangle3D;

/** 
 * An Object3D is made up of zero or more triangles,  which can be combined to
 * create a shape such as a cuboid, a circle, a pyramid, etc. This 
 * Triangle3DNode allows us to chain together multiple triangles in a linked 
 * list type of structure. Object3D will store a pointer to the first node of
 * a Triangle3DNode linked list.
 */
typedef struct Triangle3DNode {
  Triangle3D triangle;
  struct Triangle3DNode * next;
} Triangle3DNode;

/** 
 * An Object3D is made up of zero or more triangles, which can be combined to
 * create a single 3D object such as a cube, a circle, a pyramid, etc.
 * The count field should be used to track how many triangles are being used
 * to represent this shape. Then, we have a pointer to the root (the first) 
 * Triangle3DNode, which stores a triangle, and a pointer to the next
 * Triangle3DNode.
 */
typedef struct Object3D {
  long count;
  Triangle3DNode* root;
} Object3D;

/**
 * This represents a scene in 3D space containing 0 or more objects within it.
 * The count field represents the number of Object3D's this scene contains.
 * The size field represents the size of the array to store the Object3Ds.
 * The objects field represents a pointer to an array of Object3D pointers.
 * This array should start off with a small value to save memory, and only grow
 * as needed as more Object3Ds are added.
 */
typedef struct Scene3D {
  long count;
  long size;
  Object3D** objects;
} Scene3D;

/**
 * This function allocate space for a new Scene3D object on the heap, 
 * initializes the values to defaults as necessary, and returns a pointer to
 * the object in memory.
 */
Scene3D* Scene3D_create();

/** 
 * Frees the memory on the heap for the Scene3D itself, as well as the Object3D
 * and Triangle3DNode's within it.
 *   Parameters:
 *     scene: The scene to destroy
 */
void Scene3D_destroy(Scene3D* scene);

/**
 * Appends a new object to the end of the Scene3D's array of objects.
 * This function is responsible for handling the dynamic growing of the array
 * of Object3D pointers when necessary.
 *   Parameters:
 *     scene: The scene to have an object appended to
 *     object: The object to append to this scene
 */
void Scene3D_append(Scene3D* scene, Object3D* object);

/**
 * Write every shape from the Scene3D to the file with file_name using the STL
 * text format. The function is responsible for opening, writing to, and 
 * closing the file.
 *   Parameters:
 *     scene: The scene to write to the file
 *     file_name: The name of the file to write the STL data to
 */
void Scene3D_write_stl_text(Scene3D* scene, char* file_name);

/**
 * This function should create a new Object3D on the heap and populate it with
 * a bunch of triangles to represent a pyramid in 3D space.
 * The caller is responsible for freeing the memory, or, if this shape gets 
 * added to a Scene3D, that is can be freed when the scene is.
 *   Parameters:
 *     origin: The origin point for the pyramid (center of base)
 *     width: The length of the sides of the base of the pyramid
 *     height: The height of the pyramid
 *     orientation: Determines the direction the pyramid points.
 *                  Should be either: 
 *                    "forward"
 *                    "backward" 
 *                    "up"
 *                    "down"
 *                    "left"
 *                    "right"
 */
Object3D* Object3D_create_pyramid(
    Coordinate3D origin, 
    double width, double height, char* orientation);

/**
 * This function should create a new Object3D on the heap and populate it with
 * a bunch of triangles to represent a cuboid in 3D space.
 * The caller is responsible for freeing the memory, or, if this shape gets 
 * added to a Scene3D, that is can be freed when the scene is.
 *   Parameters:
 *     origin: The origin point for the cuboid (center)
 *     width: The width of the cuboid (x)
 *     height: The height of the cuboid (y)
 *     depth: The depth of the cuboid (z)
 */
Object3D* Object3D_create_cuboid(
    Coordinate3D origin, 
    double width, double height, double depth);

#endif

