//https://en.wikipedia.org/wiki/3D_projection
//a_xyz = the 3d position of a point A to be projected
//c_xyz = the 3d position of point C representing the camera
//t_xyz = orientation of the camera in Tait-Bryan angles
//e_xyz = display surface relative to c_xyz
//b_xy  = the 2D projection of A
//d_xyz = position of a_xyz in camera coordinate system
//(TB angles: rotation about x axis, rotation about new y axis, rotation about new new z axis)
//
//just going to hack out my own math functions
//where x = a_x - c_x etc, and cos(t_a) = c_a etc:
//d_x = c_y*(s_z*y + c_z*x) - s_y*z
//d_y = s_x*(c_y*z + s_y*(s_z*y + c_z*x)) + c_x*(c_z*y - s_z*x)
//d_z = c_x*(c_y*z + s_y*(s_z*y + c_z*x)) - s_x*(c_z*y - s_z*x)
//
//Then this point can be projected onto the place (x, y) with:
//b_x = (e_z*d_x/d_z) + e_x
//b_y = (e_z*d_y/d_z) + e_y
//Then do whatever clipping is needed to fit the 2d points on the display plane

#include "renderHeader.h"

//Vec3
Vec3::Vec3(double theX = 0, double theY = 0, double theZ = 0){
    x = theX;
    y = theY;
    z = theZ;
}

Vec3 Vec3::operator+(Vec3 const& v) //define SpacePoint + SpacePoint
{ 
    double px = (x + v.x); 
    double py = (y + v.y);  
    double pz = (z + v.z);  
    return Vec3(px, py, pz); 
} 

Vec3 Vec3::operator-(Vec3 const& v) //define SpacePoint - SpacePoint
{ 
    double px = (x - v.x); 
    double py = (y - v.y);  
    double pz = (z - v.z);  
    return Vec3(px, py, pz); 
} 
        
Vec3 Vec3::operator*(double const& v) //define SpacePoint * double
{ 
    double px = (x * v); 
    double py = (y * v);  
    double pz = (z * v);  
    return Vec3(px, py, pz); 
} 

std::string Vec3::print(){
    std::string xStr = std::to_string(x);
    std::string yStr = std::to_string(y);
    std::string zStr = std::to_string(z);
    std::string theStr = "(" + xStr + ", " + yStr + ", " + zStr + ")";
    return theStr;
}

//Surface
Surface::Surface(Vec3 xVars, Vec3 yVars, Vec3 zVars, Bound sBound = {0, 1}, Bound tBound = {0, 1}, char dC = '@'){
    X = xVars;
    Y = yVars;
    Z = zVars;
    s = sBound;
    t = tBound;
    dispChar = dC;
}

FlatSurface Surface::cameraTransform(Camera c){ //convert to transform linear combination of s, t + constant point c
    Vec3 e = c.displaySurface;

    Vec3 mx = this->X;
    Vec3 my = this->Y;
    Vec3 mz = this->Z;
            
    mx.z -= c.camPoint.x;
    my.z -= c.camPoint.y;
    mz.z -= c.camPoint.z;
    double sx = sin(c.camAngle.x);
    double sy = sin(c.camAngle.y);
    double sz = sin(c.camAngle.z);
    double cx = cos(c.camAngle.x);
    double cy = cos(c.camAngle.y);
    double cz = cos(c.camAngle.z);

    Vec3 dx = (my*sz + mx*cz)*cy - mz*sy;
    Vec3 dy = (mz*cy + (my*sz + mx*cz)*sy)*sx + (my*cz - mx*sz)*cx;
    Vec3 dz = (mz*cy + (my*sz + mx*cz)*sy)*cx - (my*cz - mx*sz)*sx;

    return FlatSurface(dx, dy, dz, e, s, t, this->dispChar);
}

Vec3 Surface::eval(FlatPoint st){
    return Vec3(X.x*st.x + X.y*st.y + X.z, 
                Y.x*st.x + Y.y*st.y + Y.z,
                Z.x*st.x + Z.y*st.y + Z.z);
}

//FlatSurface
FlatSurface::FlatSurface(Vec3 dx, Vec3 dy, Vec3 dz, Vec3 E, Bound S, Bound T, char dC = '@'){
    Dx = dx;
    Dy = dy;
    Dz = dz;
    e = E;
    s = S;
    t = T;
    dispChar = dC;
}

//Cube
//ChatGPT generated below
// Function to perform rotation around the x-axis (roll)
Vec3 Cube::rotateX(const Vec3& v, double angle) {
    double cosA = cos(angle);
    double sinA = sin(angle);
    return Vec3(v.x, v.y * cosA - v.z * sinA, v.y * sinA + v.z * cosA);
}
// Function to perform rotation around the y-axis (pitch)
Vec3 Cube::rotateY(const Vec3& v, double angle) {
    double cosA = cos(angle);
    double sinA = sin(angle);
    return Vec3(v.x * cosA + v.z * sinA, v.y, -v.x * sinA + v.z * cosA);
}
// Function to perform rotation around the z-axis (yaw)
Vec3 Cube::rotateZ(const Vec3& v, double angle) {
    double cosA = cos(angle);
    double sinA = sin(angle);
    return Vec3(v.x * cosA - v.y * sinA, v.x * sinA + v.y * cosA, v.z);
}
// Function to rotate a vector using Tait-Bryan angles
Vec3 Cube::rotate(const Vec3& v, double roll, double pitch, double yaw) {
    Vec3 rotated = rotateX(v, roll);
    rotated = rotateY(rotated, pitch);
    rotated = rotateZ(rotated, yaw);
    return rotated;
}
// Function to compute the 8 vertices of the rotated cube
std::vector<Vec3> Cube::getCubeVertices(const Vec3& center, double lx, double ly, double lz, double roll, double pitch, double yaw) {
    // Half lengths
    double hx = lx / 2.0;
    double hy = ly / 2.0;
    double hz = lz / 2.0;

    // Vertices of the cube before rotation (relative to the center)
    std::vector<Vec3> vertices = {
        {-hx, -hy, -hz},
        { hx, -hy, -hz},
        { hx,  hy, -hz},
        {-hx,  hy, -hz},
        {-hx, -hy,  hz},
        { hx, -hy,  hz},
        { hx,  hy,  hz},
        {-hx,  hy,  hz}
    };

    // Apply rotation and translation to each vertex
    for (auto& vertex : vertices) {
        vertex = rotate(vertex, roll, pitch, yaw);
        vertex.x += center.x;
        vertex.y += center.y;
        vertex.z += center.z;
    }

    return vertices;
}

//should generate 3D surface objects
Cube::Cube(Vec3 center, double x, double y, double z, double ax, double ay, double az){
    
    //generate points
    vertices = getCubeVertices(center, x, y, z, ax, ay, az);

    //construct

    //Central A
    //A
    //B
    //A
    //A
    //B
    //Central B
    //B

    //12 vectors, 2 per surface, based on central points. The letters dont mean the central points labelled above
    //Central Point A
    Vec3 A1 = vertices.at(1) - vertices.at(0);
    Vec3 A2 = vertices.at(3) - vertices.at(0);
    Vec3 B1 = vertices.at(1) - vertices.at(0);
    Vec3 B2 = vertices.at(4) - vertices.at(0);
    Vec3 C1 = vertices.at(3) - vertices.at(0);
    Vec3 C2 = vertices.at(4) - vertices.at(0);
    //Central Point B
    Vec3 D1 = vertices.at(2) - vertices.at(6);
    Vec3 D2 = vertices.at(5) - vertices.at(6);
    Vec3 E1 = vertices.at(2) - vertices.at(6);
    Vec3 E2 = vertices.at(7) - vertices.at(6);
    Vec3 F1 = vertices.at(5) - vertices.at(6);
    Vec3 F2 = vertices.at(7) - vertices.at(6);

    Surface A(Vec3(A1.x, A2.x, vertices.at(0).x), Vec3(A1.y, A2.y, vertices.at(0).y), Vec3(A1.z, A2.z, vertices.at(0).z), {0, 1}, {0, 1}, '/');
    Surface B(Vec3(B1.x, B2.x, vertices.at(0).x), Vec3(B1.y, B2.y, vertices.at(0).y), Vec3(B1.z, B2.z, vertices.at(0).z), {0, 1}, {0, 1}, '*');
    Surface C(Vec3(C1.x, C2.x, vertices.at(0).x), Vec3(C1.y, C2.y, vertices.at(0).y), Vec3(C1.z, C2.z, vertices.at(0).z), {0, 1}, {0, 1}, 'o');
    Surface D(Vec3(D1.x, D2.x, vertices.at(6).x), Vec3(D1.y, D2.y, vertices.at(6).y), Vec3(D1.z, D2.z, vertices.at(6).z), {0, 1}, {0, 1}, '%');
    Surface E(Vec3(E1.x, E2.x, vertices.at(6).x), Vec3(E1.y, E2.y, vertices.at(6).y), Vec3(E1.z, E2.z, vertices.at(6).z), {0, 1}, {0, 1}, '@');
    Surface F(Vec3(F1.x, F2.x, vertices.at(6).x), Vec3(F1.y, F2.y, vertices.at(6).y), Vec3(F1.z, F2.z, vertices.at(6).z), {0, 1}, {0, 1}, 'Q');

    surfaces = {A, B, C, D, E, F};
}

//World functions

//Display FlatSurfaces submitted given x and y display bounds(inclusive)
std::string Draw(std::vector<FlatSurface> flatSurfaces, Bound x, Bound y){
    //Iterate through x and y (i, j)
    //For each, use DPE function:

    //Manupulate projected equations to be s(stuff) + t(stuff) = (stuff)
    //As+Bt+C, Ds+Et+F if we can get (true, true) point exists
    //From alegebraic: 
    //  Plug in f(0) and f(1)
    //  If at least one is 0 <= f <= 1, success
    //  If not, plug in 0 and 1 to inverse (x = ...)
    //  If at least one is 0 <= f <= 1, success
    //  If not, failure
    //Same success for both?

    //Not only do we need to know if some part of x(0, 1) returns a range that intersects with (0, 1), but it has to be the same range...
    //Find intersection of lines!!! If within bounds, problem solved!!!

    std::string retStr = "";

    for(int i = y.min; i <= y.max; i++){
        std::string thisLine = "\n";
        for(int j = x.min; j <= x.max; j++){
            int h = flatSurfaces.size();
            bool gotIt = false;
            while(!gotIt && h != 0){
                //iterate through other flat surfaces
                if(doesPointExist(flatSurfaces.at(h-1), FlatPoint{double(i), double(j)})){
                    thisLine += flatSurfaces.at(h-1).dispChar;
                    gotIt = true;
                }
                h--;
            }if(!gotIt){
                thisLine += " ";
            }
        }
        retStr += thisLine;
    }

    return retStr;
}

bool doesPointExist(FlatSurface f, FlatPoint p, FlatPoint* st){
    
    //REWRITE REWRITE REWRITE
    
    //Manupulate projected equations to be s(stuff) + t(stuff) = (stuff)
    //As+Bt+C, Ds+Et+F if we can get (true, true) point exists
    //Check if point of intersection is within bounds

    //Horizontal scaling factor -- to make shapes appear square in Windows console
    //Default character aspect ratio is 3:2
    double hF = 2;
    //Multiplied by scaling in X

    //define coeffs to linearized equations
    double A = (f.e.z*f.Dx.x - f.Dz.x*(p.x-f.e.x));
    double B = (f.e.z*f.Dx.y - f.Dz.y*(p.x-f.e.x));
    double C = f.Dz.z*(p.x-f.e.x) - f.e.z*f.Dx.z;

    double D = (f.e.z*hF*f.Dy.x - f.Dz.x*(p.y-f.e.y));
    double E = (f.e.z*hF*f.Dy.y - f.Dz.y*(p.y-f.e.y));
    double F = f.Dz.z*(p.y-f.e.y) - f.e.z*hF*f.Dy.z;

    //As+Bt-C=Ds+Et-F?
    //Cramer's rule: As + Bt = C
    //               Ds + Et = F

    double detA = A*E-B*D;
    double detA1 = C*E-B*F;
    double detA2 = A*F-C*D;

    double sSol = detA1/detA;
    double tSol = detA2/detA;

    return ((sSol >=0 && sSol <= 1) && (tSol >=0 && tSol <= 1));

    return false;
}