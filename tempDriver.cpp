//Temporary driver code to test out the "textRender.cpp" file
//Camera math assumes facing XY plane with Y-axis horizontal!

//--------------------------------------------------------------------
//TODO: 
//Check each angle adjuster to make sense from driver
//Tweak z-preference?
//Try implementing sphere
//Start with animated changing frames
//Change Draw() s t stepping based on camera distance from object?
//--------------------------------------------------------------------

#include "renderHeader.h"

int main(){
    Cube theCube(Vec3(0, 0, 0), 2, 2, 2, 0, M_PI/4, M_PI/4);
    Camera c{Vec3(0, 0, 4), Vec3(0, 0, 0), Vec3(0, 0, 40)};

    std::vector<FlatSurface> flatSurfaces;
    for(auto& s : theCube.surfaces){
        flatSurfaces.push_back(s.cameraTransform(c));
    }

    std::cout << Draw(flatSurfaces, {-40, 40}, {-20, 20});

    return 0;
}

/*Possible process to figure out z-preference:
Current procedure: Check if any flat surface has input (s, t) that satisfies (x, y) of interest (Super inefficient!!)

Proposed procedure: (Somehow) Obtain s, t that renders a point and save it and the flat surface that generated it to 
a temp list. If there is more than one flat surface that generates that point, take the saved s, t and plug it into 
an eval for the original 3D version of that surface (do this with ptr in flat surface that goes back to generator surface). 
Calculate distance between plugged output and camera point, use the character of the surface with the lowest distance.

*/

/*Possible process to generate sphere from parallelograms:

Divide degrees of sphere (theta and psi) into number of parallelo faces to generate per pi-radian. 
For each pi-radian chunk, cast a line from center to surface and draw paralellogram tangent to surface -- size? 
Maybe at each cast line draw a vector, and then assemble vectors after?

Sketch on paper

*/