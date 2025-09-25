#include <SDL2/SDL.h>

typedef struct Object{
    void *shape;
    void *(*detect)(double x, double y, void *shape);
    void (*draw)(void *shape, SDL_Window *window, Uint32 color);
    Uint32 objectColor;
    int objectType;
    void (*moveObject)(void *shape, double x, double y);
}Object;

typedef struct Circle{
    double x;
    double y;
    double radius;
}Circle;

typedef struct Room{
    int x;
    int y;
    int height;
    int width;
    int thickness;
}Room;

typedef struct RayCluster{
    double x;
    double y;
    int numRays;
}RayCluster;
