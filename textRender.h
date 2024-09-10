#ifndef RENDERHEADER_H
#define RENDERHEADER_H

#include <cmath>
#include <iostream>
#include <vector>

//the object formerly known as SpacePoint
class Vec3{ //doubles as any 3-part info: point, angles, vector, etc
    public:
        double x;
        double y;
        double z;

        Vec3(double, double, double);

        Vec3 operator+(Vec3 const&); //define SpacePoint + SpacePoint 

        Vec3 operator-(Vec3 const& v); //define SpacePoint + SpacePoint 
        
        Vec3 operator*(double const& v); //define SpacePoint * double

        std::string print();

        double length();
};

struct Bound{
    double min = 0;
    double max = 0;
};

struct FlatPoint{
    double x = 0;
    double y = 0;
};

struct Camera{
    Vec3 camPoint;
    Vec3 camAngle;
    Vec3 displaySurface;
};

class FlatSurface{
    public:
        //Defining with same vars as 3D version so transform can take place
        Vec3 Dx; // linear combination of s, t, constant for X
        Vec3 Dy;
        Vec3 Dz;
        Vec3 e;
        Bound s;
        Bound t;
        char dispChar;
        Camera c; 

        FlatSurface(Vec3 const& dx, Vec3 const& dy, Vec3 const& dz, Bound const& S, Bound const& T, Camera const&, char);

        double eval(char, double, double, int);

        //Evaulate returns Vec3(x, y, z) for some (s, t) input
        Vec3 eval(FlatPoint const&);

        std::string print();

        //double bx = (e.z*dx/dz) + e.x;
        //double by = (e.z*dy/dz) + e.y;
};

class Surface{
    public:
        Vec3 X; // linear combination of s, t, constant for X
        Vec3 Y;
        Vec3 Z;
        Bound s;
        Bound t;
        char dispChar;
    
        //how to define?
        //Plane can be represented parametrically as X = sA + tB + c, where s and t are parameters, A and B are vectors parallel to the plane, and c is a point
        //So theoretically some bounds on s and t should define edges?
        //Defines parallelogram!
        //should contain transform method, return FlatSurface
        Surface(Vec3 const&, Vec3 const&, Vec3 const&, Bound const&, Bound const&, char);

        FlatSurface cameraTransform(Camera const&); //convert to transform linear combination of s, t + constant point c
};

class Cube{
    //Lots of help from ChatGPT on this function
    public:
        // Vertices of the cube
        std::vector<Vec3> vertices;
        //6 3d surfaces
        std::vector<Surface> surfaces;

        //ChatGPT did these
        Vec3 rotateX(const Vec3& v, double angle);
        Vec3 rotateY(const Vec3& v, double angle);
        Vec3 rotateZ(const Vec3& v, double angle);
        Vec3 rotate(const Vec3& v, double roll, double pitch, double yaw);
        void getCubeVertices(const Vec3& center, double lx, double ly, double lz, double roll, double pitch, double yaw);

        //Generates 3D surface objects
        Cube(Vec3 const& center, double x, double y, double z, double ax, double ay, double az);
};

class Sphere{
    public:
        //Not vertices per se, but pts that define our simplified sphere
        std::vector<Vec3> vertices;
        //List of N surfaces. N will depend on the granularity parameter passed into the constructor
        std::vector<Surface> surfaces;

        Vec3 center;
        Vec3 poleN;
        Vec3 poleS;
        Vec3 angRef;
        int G;

        //Funtions:
        //Take sphere center and radius -> convert to list of pts
        //Constructor, takes info and calls... --> have it convert center and pole pts and an angle reference pt with Cube rotate functions
        //Take vertices, generate surfaces with the parallelograms (written in project notebook)
        //  Vertices should be in order top to bottom (top is one pole pt), wrapping around from some defined psi=0 (angle reference pt)
        //  For each (starting one after pole, ending one before pole) draw vector to one layer down, one over and to one over. Use to create surfaces
        //      Each layer has same # of pts, depending on granularity, so "one layer down" is always the same shift

        Sphere(Vec3 const&, double, Vec3 const&, int);
        void getSpherePts(double);
        //rotator functions
        void generateSurfaces();

};

//Display FlatSurfaces submitted given x and y display bounds
std::string Draw(std::vector<FlatSurface> const&, Bound const&, Bound const&);
//Does a solution exist for some point
bool doesPointExist(FlatSurface const& , FlatPoint const&, FlatPoint*);

#endif