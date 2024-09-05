//Temporary driver code to test out the "textRender.cpp" file
//Camera math assumes facing XY plane with Y-axis horizontal!

//--------------------------------------------------------------------
//TODO: 
//Split cube and sphere into their own files?
//Change from "renderHeader.h" to "textRender.h"
//Make stuff private/public
//Check each angle adjuster to make sense from driver
//Tweak z-preference? -- desmos graph
//Try implementing sphere
//Start with animated changing frames
//Lower-quality render for far-away objects?
//Add documentation to textRender stuff
//--------------------------------------------------------------------

#include "renderHeader.h"

int main(){
    Sphere testSphere(Vec3(0, 0, 0), 4, Vec3(0, 0, 0), 9);

    std::cout << "\nThe points:" << testSphere.vertices.size();
    std::cout << "\nThe surfaces:" << testSphere.surfaces.size();

    Camera c{Vec3(0, 0, 10), Vec3(0, 0, 0), Vec3(0, 0, 30)};

    std::vector<FlatSurface> flatSurfaces;
    for(auto& s : testSphere.surfaces){
        flatSurfaces.push_back(s.cameraTransform(c));
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