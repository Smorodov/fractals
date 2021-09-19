// NeuroTruck.cpp : Defines the entry point for the console application.
//
#include <time.h>
#include <iostream>
#include <vector>
#include <SDL.h> 
#include <SDL_timer.h>
#include <SDL_image.h> 
#include <iostream>
#undef main
using namespace std;
// ----------------------------------------
// floating point point
// ----------------------------------------
struct FPoint
{
    float x;
    float y;
    float angle;
    float scale;
    float r, g, b;
    FPoint()
    {
        r=255, g = 255, b = 255;
        scale = 1;
        angle = 0;
    }
};
// ----------------------------------------
// the main fractal function
// ----------------------------------------
void computerecursive(int level,int max_level, FPoint center, std::vector<FPoint>& local_pts, std::vector<  FPoint >& pts)
{
    // desired fractal depth
    if(level >= max_level)
    {
        return;
    }
    std::vector<  FPoint > newPts;
    // scale change for each level of depth
    
    for (int j = 0; j < local_pts.size(); ++j)
    {
        FPoint p;
        p.x = local_pts[j].x * local_pts[j].scale * cos(local_pts[j].angle) -local_pts[j].y * local_pts[j].scale * sin(local_pts[j].angle);
        p.y = local_pts[j].x * local_pts[j].scale * sin(local_pts[j].angle) +local_pts[j].y * local_pts[j].scale * cos(local_pts[j].angle);                
        p.scale = local_pts[j].scale;
        p.angle = local_pts[j].angle;
        p.r = local_pts[j].r;
        p.g = local_pts[j].g;
        p.b = local_pts[j].b;

        //p.r = center.r * (1.0 - local_pts[j].scale) + local_pts[j].r * local_pts[j].scale;
        //p.g = center.g * (1.0 - local_pts[j].scale) + local_pts[j].g * local_pts[j].scale;
        //p.b = center.b * (1.0 - local_pts[j].scale) + local_pts[j].b * local_pts[j].scale;
        newPts.push_back(p);
    }

  //  if (level == max_level - 1)
    {
        for (int i = 0; i < local_pts.size(); ++i)
        {
            FPoint p;
            p.x = local_pts[i].x + center.x;
            p.y = local_pts[i].y + center.y;
            p.angle = local_pts[i].angle;
            p.scale = local_pts[i].scale;
            p.r = center.r;
            p.g = center.g;
            p.b = center.b;

            pts.push_back(p);
        }
    }

    for (int j = 0; j < local_pts.size(); ++j)
    {
        FPoint p;
        p.x = local_pts[j].x + center.x;
        p.y = local_pts[j].y + center.y;
        p.angle = local_pts[j].angle;
        p.scale = local_pts[j].scale;
        p.r = center.r * (( 1.0 - local_pts[j].scale) * (level + 1) ) + local_pts[j].r * (local_pts[j].scale);
        p.g = center.g * (( 1.0 - local_pts[j].scale) * (level + 1) ) + local_pts[j].g * (local_pts[j].scale);
        p.b = center.b * (( 1.0 - local_pts[j].scale) * (level + 1) ) + local_pts[j].b * (local_pts[j].scale);
        float n = max(max(p.r,p.g),p.b);
        if (n > 0)
        {
            p.r /= n;
            p.g /= n;
            p.b /= n;
            p.r *= 255;
            p.g *= 255;
            p.b *= 255;
        }
        
        computerecursive(level + 1, max_level, p, newPts, pts);
    }    
}
// ----------------------------------------
//
// ----------------------------------------
void main()
{
    int W = 800;
    int H = 800;

    if (SDL_Init(SDL_INIT_VIDEO) == 0)
    {
        SDL_Window* window = NULL;
        SDL_Renderer* renderer = NULL;
        SDL_ShowCursor(SDL_DISABLE);

        if (SDL_CreateWindowAndRenderer(W, H, 0, &window, &renderer) == 0)
        {
            SDL_bool done = SDL_FALSE;
            float angle = -60.0*M_PI/180.0;
            while (!done)
            {
                SDL_Event event;

                SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
                SDL_RenderClear(renderer);

                // ----------------------------------------
                // draw mouse
                // ----------------------------------------
                SDL_Point mouse;
                Uint32 state = SDL_GetMouseState(&mouse.x, &mouse.y);
                SDL_SetRenderDrawColor(renderer, 255, 255, 0, SDL_ALPHA_OPAQUE);
                SDL_RenderDrawLine(renderer, mouse.x - 15, mouse.y, mouse.x + 15, mouse.y);
                SDL_RenderDrawLine(renderer, mouse.x, mouse.y - 15, mouse.x, mouse.y + 15);
  
                // snimation speed
                //angle += 0.001;
                // number of initial polygon verticas
                int NP = 3;
                int max_level = 8;
                std::vector<  FPoint > local_pts(NP);
                // initial palygon radius
                float r = min(W,H)/5;
                // coordinates of center
                float cx = W / 2;
                float cy = H / 2;
                float step = 360.0f / (float)(NP);
                std::vector<FPoint> pts;
                // fill seed points set
                float red[3] =   {0,    0,   255};
                float green[3] = {0,    255, 0 };
                float blue[3] =  {255 , 0,   0 };

                for (int n = 0; n < NP; ++n)
                {
                    local_pts[n].x = r * sin((float)n * step * M_PI / 180.0+angle);
                    local_pts[n].y = r * cos((float)n * step * M_PI / 180.0+angle);
                    local_pts[n].scale = 0.5;
                    local_pts[n].angle = angle;
                    local_pts[n].r = red[n % 3];
                    local_pts[n].g = green[n % 3];
                    local_pts[n].b = blue[n % 3];
                }
                               
                FPoint p;
                p.x = cx;
                p.y = cy;
                p.scale = 0.5;
                p.angle = angle;
                p.r = 255;
                p.g = 255;
                p.b = 255;                
                   
                computerecursive(0, max_level, p, local_pts, pts);

                for (int i=0;i<pts.size()-NP;i+=NP)
                {
                    for (int j = 1; j <= NP; j += 1)
                    {
                        int ind1 = i + (j) % NP;
                        int ind2 = i + (j+1) % NP;
                        SDL_SetRenderDrawColor(renderer, pts[ind1].r, pts[ind1].g, pts[ind1].b, SDL_ALPHA_OPAQUE);
                        SDL_RenderDrawLine(renderer, pts[ind1].x, pts[ind1].y, pts[ind2].x, pts[ind2].y);
                        SDL_RenderDrawPoint(renderer, round(pts[ind1].x), round(pts[ind1].y) );
                        SDL_SetRenderDrawColor(renderer, pts[ind2].r, pts[ind2].g, pts[ind2].b, SDL_ALPHA_OPAQUE);
                        SDL_RenderDrawPoint(renderer, round(pts[ind2].x), round(pts[ind2].y) );
                    }                  
                }
                // ----------------------------------------
                // show 
                // ----------------------------------------
                SDL_RenderPresent(renderer);
                // ----------------------------------------
                // process events
                // ----------------------------------------
                while (SDL_PollEvent(&event))
                {
                    if (event.type == SDL_QUIT)
                    {
                        done = SDL_TRUE;
                    }
                    /* Look for a keypress */
                    if (event.type == SDL_KEYDOWN)
                    {
                        /* Check the SDLKey values and move change the coords */
                        switch (event.key.keysym.sym)
                        {
                        case SDLK_LEFT:
                            
                            break;
                        case SDLK_RIGHT:
                            
                            break;
                        case SDLK_UP:
                            
                            break;
                        case SDLK_DOWN:
                            
                            break;
                        default:
                            break;

                        }
                    }
                }
            }

            if (renderer)
            {
                SDL_DestroyRenderer(renderer);
            }
            if (window)
            {
                SDL_DestroyWindow(window);
            }
        }
        SDL_ShowCursor(SDL_ENABLE);
        SDL_Quit();
    }
}
