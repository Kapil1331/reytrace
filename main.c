#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>
#include <math.h>

typedef struct Circle{
    double x;
    double y;
    double radius;
}Circle;

/*
typedef struct RayOrigin{
    double x;
    double y;
}RayOrigin;
*/

double getDistance(double x1, double y1, double x2, double y2){
    return sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
}

void USR_BuildCircle(struct Circle circle, SDL_Window *window){
    SDL_Surface* win_surface =  SDL_GetWindowSurface(window);

    for(int x = circle.x - circle.radius ; x <= circle.x + circle.radius ; x++){
        for(int y = circle.y - circle.radius ; y <= circle.y + circle.radius ; y++){
            if( getDistance(x, y, circle.x, circle.y) <= circle.radius){
                SDL_Rect rect = (SDL_Rect) {x, y, 1, 1}; // x, y, w, h
                SDL_FillRect(win_surface, &rect, 0x0de03b);
            }    
        }
    }
    
    SDL_UpdateWindowSurface(window);
}

void USR_BuildRoom(int x, int y, int height, int width, int thickness, SDL_Window *window){
    SDL_Surface* win_surface =  SDL_GetWindowSurface(window);
    
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



void USR_RenderRays(int xOrigin, int yOrigin, int numRays){
    double angleResolution = 360.0 / numRays;
    for(int angle = 0 ; angle <= 360.0; angle += angleResolution){
        for(int x = xOrigin; x <= 1000 - xOrigin && x >= 0 ; x++){
            for(int x = xOrigin; x <= 1000 - xOrigin && x >= 0 ; x++){
                //todo : render a line by rendering points individually    
            }
        }
    }


}

int main(){

    SDL_Init(SDL_INIT_VIDEO);
    
    SDL_Window* window =  SDL_CreateWindow("trace-ray", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 500, SDL_WINDOW_MOUSE_GRABBED); 
    SDL_Rect win_rect = (SDL_Rect) {0, 0, 1000, 500}; // x, y, w, h
    SDL_Surface* win_surface =  SDL_GetWindowSurface(window);
    SDL_Rect rect = (SDL_Rect) {500, 250, 50, 50};
    SDL_FillRect(win_surface, &rect, 0x0de03b);
    SDL_UpdateWindowSurface(window);
    
    SDL_Event event;
    Circle circle = (Circle) {200, 200, 100};
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
                        printf("%d, %d\n", event.motion.x, event.motion.y);
                        circle.x = event.motion.x;
                        circle.y = event.motion.y;
                    } 
            }
            
            USR_BuildCircle(circle, window);
            USR_BuildRoom(750, 250, 100, 200, 10, window);
            // clear canvas before each frame
            SDL_FillRect(win_surface, &win_rect, 0x000000);
        }
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
