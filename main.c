#include <SDL2/SDL.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>
#include <math.h>

typedef struct Circle{
    double x;
    double y;
    double radius;
}Circle;

double getDistance(double x1, double y1, double x2, double y2){
    return sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
}

void USR_FillCircle(struct Circle circle, SDL_Window *window){
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


int main(){

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window =  SDL_CreateWindow("trace-ray", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 500, SDL_WINDOW_MOUSE_GRABBED); 
    
    SDL_Surface* win_surface =  SDL_GetWindowSurface(window);
    
    SDL_Rect rect = (SDL_Rect) {500, 250, 50, 50}; // x, y, w, h
    SDL_FillRect(win_surface, &rect, 0x0de03b);
    SDL_UpdateWindowSurface(window);
    
    Circle circle = (Circle) {20, 20, 10};
    USR_FillCircle(circle, window);

    SDL_Delay(10000);

}
