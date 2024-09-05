//Temporary driver code to test out the "textRender.cpp" file
//Camera math assumes facing XY plane with Y-axis horizontal!

//--------------------------------------------------------------------
//TODO: 
//Check each angle adjuster to make sense from driver
//Tweak z-preference?
//Try implementing sphere
//Start with animated changing frames
//Change Draw() s t stepping based on camera distance from object?
//Add documentation to textRender stuff
//--------------------------------------------------------------------

#include "renderHeader.h"

int main(){
    Cube theCube(Vec3(0, 0, 0), 2, 2, 2, 0, M_PI/4, M_PI/4);
    Camera c{Vec3(0, 0, 4), Vec3(0, 0, 0), Vec3(0, 0, 40)};

    std::vector<FlatSurface> flatSurfaces;
    for(auto& s : theCube.surfaces){
        flatSurfaces.push_back(s.cameraTransform(c));
    }

    for(auto& f : flatSurfaces){
        std::cout << "\n\n" << f.print();
    }

    std::cout << Draw(flatSurfaces, {-40, 40}, {-20, 20});

    return 0;
}

//-----------------------------------------------------------------
//Future big steps
//Project can read in info from text file instead of relying on driver code -->
//  separate file/header
//Separate animations file/header
//Some system for game procedure
//  If turn-based, modify old Vampire code?
//-----------------------------------------------------------------

/*Possible process to generate sphere from parallelograms:

Divide degrees of sphere (theta and psi) into number of parallelo faces to generate per pi-radian. 
For each pi-radian chunk, cast a line from center to surface and draw paralellogram tangent to surface -- size? 
Maybe at each cast line draw a vector, and then assemble vectors after?

Sketch on paper

*/