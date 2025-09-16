#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>

#define PI 3.141592654
#define SUN_COLOR 0xf549f3
#define RAY_COLOR 0xf549f3
#define ROOM_COLOR 0x0de03b
#define WIN_WIDTH 1000
#define WIN_HEIGHT 500

int numObjects = 0;

typedef struct Object{
    void *shape;
    int (*detect)(double x, double y, void *shape);
    void (*draw)(void *shape, SDL_Window *window, Uint32 color);
    Uint32 objectColor;
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

double getDistance(double x1, double y1, double x2, double y2){
    return sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
}

int detectRoom(double x, double y, void* roomObject){
    Room *room = (Room*)roomObject;
    if(x >= room->x && x < room->x + room->width &&
        y >= room->y && y < room->y + room->thickness){
        return 1;
    }

    if(x >= room->x && x < room->x + room->width &&
        y >= room->y + room->height - room->thickness &&
        y < room->y + room->height){
        return 1;
    }

    if(x >= room->x && x < room->x + room->thickness &&
        y >= room->y + room->thickness &&
        y < room->y + room->thickness + (int)(0.25 * (room->height - 2 * room->thickness))){
        return 1;
    }

    if(x >= room->x && x < room->x + room->thickness &&
        y >= room->y + room->thickness + (int)(0.75 * (room->height - 2 * room->thickness)) &&
        y < room->y + room->height - room->thickness){
        return 1;
    }

    if(x >= room->x + room->width - room->thickness && x < room->x + room->width &&
        y >= room->y + room->thickness &&
        y < room->y + room->height - room->thickness){
        return 1;
    }

    return 0;
}

void drawRoom(void *roomObject, SDL_Window *window, Uint32 color){
    Room *room = (Room*)roomObject;
    SDL_Surface* win_surface =  SDL_GetWindowSurface(window);
    
    int x = room->x;
    int y = room->y;
    int height = room->height;
    int width = room->width;
    int thickness = room->thickness;

    SDL_Rect top = (SDL_Rect) {x, y, width, thickness};
    SDL_FillRect(win_surface, &top, color);
    SDL_Rect bottom = (SDL_Rect) {x, y + height - thickness, width, thickness};
    SDL_FillRect(win_surface, &bottom, color);
    SDL_Rect left_up = (SDL_Rect) {x, y + thickness, thickness, (int)(0.25 * (height - 2*thickness)) };
    SDL_FillRect(win_surface, &left_up, color);
    SDL_Rect left_bottom = (SDL_Rect) {x, y + thickness + (int)(0.75 * (height - 2*thickness)), thickness, (int)(0.25 * (height - 2*thickness)) };
    SDL_FillRect(win_surface, &left_bottom, color);
    SDL_Rect right = (SDL_Rect) {x + width - thickness, y + thickness, thickness, height - 2 * thickness};
    SDL_FillRect(win_surface, &right, color);

    SDL_UpdateWindowSurface(window);
}

void drawCircle(void *circleObject, SDL_Window *window, Uint32 color){
    Circle *circle = (Circle*)circleObject;
    SDL_Surface* win_surface =  SDL_GetWindowSurface(window);

    for(int x = circle->x - circle->radius ; x <= circle->x + circle->radius ; x++){
        for(int y = circle->y - circle->radius ; y <= circle->y + circle->radius ; y++){
            if( getDistance(x, y, circle->x, circle->y) <= circle->radius){
                SDL_Rect rect = (SDL_Rect) {x, y, 1, 1}; // x, y, w, h
                SDL_FillRect(win_surface, &rect, color);
            } 
        }
    }
    
    SDL_UpdateWindowSurface(window);
}

int detectCircle(double x, double y, void* circleObject){
    Circle *circle = (Circle *)circleObject;
    if(getDistance(x, y, circle->x, circle->y) <= circle->radius){
        return 1;
    }
    return 0;
}

void USR_RenderRays(RayCluster cluster, SDL_Window *window, Uint32 color, Object *objects){
    double xOrigin = cluster.x;
    double yOrigin = cluster.y;
    int numRays = cluster.numRays;
    
    SDL_Surface* win_surface =  SDL_GetWindowSurface(window);
    double angleResolution = 360.0 / numRays;
    double rayResolution = 1;

    for(double angle = 0 ; angle <= 360.0; angle += angleResolution){
        for(double d = 0; ; d += rayResolution){
            double x = xOrigin + d * cos(PI * angle / 180.0);
            double y = yOrigin + d * sin(PI * angle / 180.0);

            if(x < 0 || y < 0 || x > 1000 || y > 500){
                break;
            }
            int hit = 0;
            for(int i = 1 ; i < numObjects ; i++){
                if(objects[i].detect(x, y, objects[i].shape)){
                    hit = 1;
                    break;
                }
            }
            if(hit) break;
 
            SDL_Rect right = (SDL_Rect) {x, y, 1, 1};
            SDL_FillRect(win_surface, &right, color);
        }
    }
    SDL_UpdateWindowSurface(window);
}

int main(){

    SDL_Init(SDL_INIT_VIDEO);
    
    SDL_Window* window =  SDL_CreateWindow("trace-ray", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 500, SDL_WINDOW_MOUSE_GRABBED); 
    SDL_Rect win_rect = (SDL_Rect) {0, 0, WIN_WIDTH, WIN_HEIGHT}; // x, y, w, h
    SDL_Surface* win_surface =  SDL_GetWindowSurface(window);
    SDL_Rect rect = (SDL_Rect) {500, 250, 50, 50};
    SDL_FillRect(win_surface, &rect, 0x0de03b);
    SDL_UpdateWindowSurface(window);
    
    RayCluster sunRays = (RayCluster) {200, 200, 200};
    
    Object objects[10];
    Circle sun = {200, 200, 50};
    Circle ball = {100, 100, 25};
    Room room1 = {750, 250, 100, 200, 10};
    Room room2 = {250, 250, 100, 200, 15};

    Object circle1Object = {&sun, &detectCircle, &drawCircle, SUN_COLOR};
    objects[numObjects++] = circle1Object;
    Object room1Object = {&room1, &detectRoom, &drawRoom, ROOM_COLOR};
    objects[numObjects++] = room1Object;
    Object room2Object = {&room2, &detectRoom, &drawRoom, ROOM_COLOR};
    objects[numObjects++] = room2Object;
    Object circle2Object = {&ball, &detectCircle, &drawCircle, SUN_COLOR};
    objects[numObjects++] = circle2Object;

    SDL_Event event;

    int run = 1;
    while(run){
        while(SDL_PollEvent(&event)){
            switch(event.type){
                case SDL_QUIT:
                    run = 0;
                    SDL_DestroyWindow(window);
                    SDL_Quit();
                    break;
                case SDL_MOUSEMOTION:
                    if(event.motion.state == SDL_PRESSED){
                        sun.x = event.motion.x;
                        sun.y = event.motion.y;
                        sunRays.x = event.motion.x;
                        sunRays.y = event.motion.y;
                    } 
            }
            
            USR_RenderRays(sunRays, window, RAY_COLOR, objects);
            for(int i = 0 ; i < numObjects ; i++){
                objects[i].draw(objects[i].shape, window, objects[i].objectColor);
            }
            // clear canvas before each frame
            SDL_FillRect(win_surface, &win_rect, 0x000000);
        }
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
