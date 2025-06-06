#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>
#include <math.h>

#define PI 3.141592654
#define SUN_COLOR 0xf549f3
#define RAY_COLOR 0xf549f3
#define WIN_WIDTH 1000
#define WIN_HEIGHT 500

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

int detectObstacle(double x, double y, Room room){
    if(x < (room.x + room.width) && x > room.x){
        /*
        if(y > (room.y + (int)(0.25 * (room.height - 2*room.thickness))) && y < (room.thickness + (int)(0.75 * (room.height - 2*room.thickness)))){
            return 0;
        }
        */
        int gap_top = room.y + room.thickness + (int)(0.25 * (room.height - 2 * room.thickness));
        int gap_bottom = room.y + room.thickness + (int)(0.75 * (room.height - 2 * room.thickness));

        if (y > gap_top && y < gap_bottom) {
            return 0;
        }
        
        if(y > room.y && y < (room.y + room.height)){
            return 1;
        }
    }
    return 0;
}


void USR_BuildCircle(struct Circle circle, SDL_Window *window, Uint32 color){
    SDL_Surface* win_surface =  SDL_GetWindowSurface(window);

    for(int x = circle.x - circle.radius ; x <= circle.x + circle.radius ; x++){
        for(int y = circle.y - circle.radius ; y <= circle.y + circle.radius ; y++){
            if( getDistance(x, y, circle.x, circle.y) <= circle.radius){
                SDL_Rect rect = (SDL_Rect) {x, y, 1, 1}; // x, y, w, h
                SDL_FillRect(win_surface, &rect, color);
            } 
        }
    }
    
    SDL_UpdateWindowSurface(window);
}

void USR_BuildRoom(Room room, SDL_Window *window){
    SDL_Surface* win_surface =  SDL_GetWindowSurface(window);
    
    int x = room.x;
    int y = room.y;
    int height = room.height;
    int width = room.width;
    int thickness = room.thickness;

    SDL_Rect top = (SDL_Rect) {x, y, width, thickness};
    SDL_FillRect(win_surface, &top, 0x0de03b);
    SDL_Rect bottom = (SDL_Rect) {x, y + height - thickness, width, thickness};
    SDL_FillRect(win_surface, &bottom, 0x0de03b);
    SDL_Rect left_up = (SDL_Rect) {x, y + thickness, thickness, (int)(0.25 * (height - 2*thickness)) };
    SDL_FillRect(win_surface, &left_up, 0x0de03b);
    SDL_Rect left_bottom = (SDL_Rect) {x, y + thickness + (int)(0.75 * (height - 2*thickness)), thickness, (int)(0.25 * (height - 2*thickness)) };
    SDL_FillRect(win_surface, &left_bottom, 0x0de03b);
    SDL_Rect right = (SDL_Rect) {x + width - thickness, y + thickness, thickness, height - 2 * thickness};
    SDL_FillRect(win_surface, &right, 0x0de03b);

    SDL_UpdateWindowSurface(window);
}

void USR_RenderRays(RayCluster cluster, SDL_Window *window, Uint32 color, Room room){
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

            if(x < 0 || y < 0 || x > 1000 || y > 500 || detectObstacle(x, y, room)){
                break;
            }else{
                SDL_Rect right = (SDL_Rect) {x, y, 1, 1};
                SDL_FillRect(win_surface, &right, color);
            }
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
    
    Circle sun = (Circle) {200, 200, 50};
    RayCluster sunRays = (RayCluster) {200, 200, 150};
    Room room = {750, 250, 100, 200, 10};

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
            
            USR_BuildCircle(sun, window, SUN_COLOR);
            USR_RenderRays(sunRays, window, RAY_COLOR, room);
            USR_BuildRoom(room, window);
            // clear canvas before each frame
            SDL_FillRect(win_surface, &win_rect, 0x000000);
        }
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
