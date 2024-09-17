//Temporary driver code to test out the "textRender.cpp" file
//Camera math assumes facing XY plane with Y-axis horizontal!

//Anim logic:
//Will need to have one part of program rendering frames, while other takes user input
//Anim function can take reference to an object attribute as a parameter + another parameter for a 
//  change in value + another parameter for how many frames to do it in
//  So if I want to change the x position of a Cube from 0 to 8 over 20 frames, the call would be animChange(&theCube.center.x, 8, 20)
//  May need to use a template for this?
//Within the loop generating frames, there will need to be a queue of object changes

//--------------------------------------------------------------------
//TODO: 
//Fully implement drawFrame in Anim
//Clean up textRender stuff
//  Make stuff private/public
//  Add documentation to textRender stuff
//  Make Cube take Vec3 for sides, angles
//Check each angle adjuster to make sense from driver
//Tweak z-preference? -- desmos graph
//Start with animated changing frames
//Lower-quality render for far-away objects?
//Ability to read in from file
//--------------------------------------------------------------------

#include "textRender.h"
#include "anim.h"

int main(){
    Sphere testSphere(Vec3(0, 0, 0), 4, Vec3(0, 0, 0), 10);
    Cube testCube(Vec3(0, 0, 60), 6, 6, 6, 0, 0, 0);

    Camera c{Vec3(0, 0, 10), Vec3(0, 0, 0), Vec3(0, 0, 30)};

    std::vector<FlatSurface> flatSurfaces;

    //testing animation
    for(int i = 0; i < 20; i++){
        flatSurfaces.clear();
        clear_screen(' ');

        c.camPoint.z+=1;

        for(auto& s : testSphere.surfaces){
            flatSurfaces.push_back(s.cameraTransform(c));
        }

        std::cout << Draw(flatSurfaces, {-40, 40}, {-20, 20});

        wait(50);

    }

    for(int i = 0; i < 20; i++){
        flatSurfaces.clear();
        clear_screen(' ');

        c.camAngle.x += (M_PI/20.0);
        for(auto& s : testSphere.surfaces){
            if(s.isInView(c)){
                flatSurfaces.push_back(s.cameraTransform(c));
            }
        }
        for(auto& s : testCube.surfaces){
            if(s.isInView(c)){
                flatSurfaces.push_back(s.cameraTransform(c));
            }
        }

        std::cout << Draw(flatSurfaces, {-40, 40}, {-20, 20});

        wait(50);

    }
    
    for(int i = 0; i < 20; i++){
        flatSurfaces.clear();
        clear_screen(' ');

        c.camPoint.z+=1;

        for(auto& s : testCube.surfaces){
            flatSurfaces.push_back(s.cameraTransform(c));
        }

        std::cout << Draw(flatSurfaces, {-40, 40}, {-20, 20});

        wait(50);

    }

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