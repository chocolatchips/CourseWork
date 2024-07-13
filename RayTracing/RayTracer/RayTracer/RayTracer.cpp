// RayTracer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
//#include <string>
//#include <vector>
#include <cmath>
#include "RayTracer.h"
#include "Sphere.h"
#include "Light.h"

using namespace std;

#define SMALL_NUMBER    1.e-8
#define MAX_REFLECT     4
#define OFFSET          0.0001f

const string NEAR = "NEAR";
const string LEFT = "LEFT";
const string RIGHT = "RIGHT";
const string TOP = "TOP";
const string BOTTOM = "BOTTOM";
const string SPHERE_ID = "SPHERE";
const string AMBIENT_ID = "AMBIENT";
const string BACK_ID = "BACK";
const string LIGHT_ID = "LIGHT";
const string RES_ID = "RES";
const string OUTPUT = "OUTPUT";

Plane plane;
Res res;
Ambient ambient;
Back back;
vector<light> lights;
vector<sphere> spheres;
vector<vec3> colours;
string output;
ofstream newFile;
vec4 origin(0.0, 0.0, 0.0, 1.0);
vec4 near_plane(0.0, 0.0, 0.0, 1.0);

bool intersect(vec4, vec4, float&, float&);
vec3 ray_color(ray, int);
void write_colour();
void parseValues(vector<vector<string>>);
vector<string> removeWhiteSpace(string);
void setAmbient(vector<string>);
void setBack(vector<string>);
void setRes(vector<string>);
void createSphere(vector<string>);
void createLight(vector<string>);

int main(int argc, char* argv[]) {
    fstream fs;
    string text;
    // Check that input file is provided
    if (argc < 2) {
        cout << "No file selected";
        return 0;
    }
    cout << argv[1] << "\n";
    ifstream open_file(argv[1]);
    // Check that input file exists and can be accessed
    if (!open_file.is_open()) {
        cout << "Could not open file: " << argv[1] << "\n";
        return 1;
    }
    // Get each line of text from file
    vector<vector<string>> fileLines;
    while (getline(open_file, text)) {

        fileLines.push_back(removeWhiteSpace(text));
    }
    parseValues(fileLines);
    open_file.close();
    
    float W = float(abs(plane.left) + abs(plane.right))/2.f;
    float H = float(abs(plane.bottom) + abs(plane.top))/2.f;
    vec4 u(W, 0.f, 0.f, 0.f);
    vec4 v(0.f, H, 0.f, 0.f);

    for (int h = res.y - 1; h >= 0; h--) {
        for (int c = 0; c < res.x; c++) {
            float col = W *(2.f * c / res.x - 1.f);
            float row = H * (2.f * h / res.y - 1.f);
            ray r(origin, vec4(0.f, 0.f, -float(plane.near), 0.f) + u * col + v * row);
            colours.push_back(ray_color(r, 0));
        }
    }
    write_colour();
    return 0;
}


/// <summary>
/// This function calculates each light's impact on the
/// diffuse and specular intensity of the input point's colour
/// </summary>
/// <param name="P">The current point in world coordinates</param>
/// <param name="hs">The sphere the current point lies on</param>
/// <param name="dir">Direction of ray impact on current point</param>
/// <param name="N">The normal of the point on the hit sphere</param>
/// <returns>Diffuse and specular colour intensity</returns>
vec3 light_colour(vec4 P, sphere hs, vec4 dir, vec4 N) {
    // Default clour returned is black
    vec3 colour(0.f, 0.f, 0.f);

    //Check each light's impact on colour of point
    for (light light : lights) {
        vec4 L = light.centre() - P;
        float light_dis = dot_prod(L, L);
        L = normalize(L);
        float LN = max(0.f, dot_prod(L, N));
        
        float tn = 0.f;
        float tf = 0.f;
        bool shadow = false;
        // Check each sphere for intersection along path to light
        for (sphere sp : spheres) {
            vec4 S = sp.inverse(P);
            vec4 c = normalize(sp.inverse(L));
            bool inter = intersect(c, S, tn, tf);
            // Check if intersetion distance is less than light distance
            shadow = inter && dot_prod(c * tn, c * tn) < light_dis;
            if (shadow)
                break;
        }
        // If point in shadow, light does not affect colour
        if (shadow)
            continue;
        // Calculate diffuse
        vec3 dif = hs.kd() * hs.colour() * light.colour() * LN;
        
        // Use negative incoming direction vector as viewing vector
        vec4 V = normalize(-dir);
        // Calculate reflection vector
        vec4 R = normalize(2.f * dot_prod(L, N) * N - L);
        // Calculate Specular
        vec3 spec = hs.ks() * pow(max(0.f,dot_prod(R, V)), hs.N()) * light.colour();
        
        colour += dif + spec;
    }
    return colour;
}


/// <summary>
/// This function recursively finds the colour of a fired ray.
/// The function finds the closest intersection of all spheres
/// and uses that intersection point as the start of the new ray.
/// </summary>
/// <param name="r">The incoming ray</param>
/// <param name="ref">The current number of reflections after original ray</param>
/// <returns>Colour intensity of current ray</returns>
vec3 ray_color(ray r, int ref) {
    vec3 colour(0.f, 0.f, 0.f);
    if (ref == MAX_REFLECT)
        return colour;

    auto sp = spheres.begin();
    float t = FLT_MAX;
    sphere hit_sphere;
    for (sphere sp : spheres)
    {
        vec4 S = sp.inverse(r.origin() + OFFSET * r.direct());
        vec4 c = sp.inverse(r.direct());
        float tn = 0.f;
        float tf = 0.f;
        if (intersect(c, S, tn, tf)) {
            // Check if near intersection is before near plane
            if (tn < plane.near && ref == 0) {
                tn = tf;
                // Check if far intersection is before near plane
                if (tf < plane.near)
                    continue;
            }
            t = min(t, tn);
            hit_sphere = sp;
        }
    }
    // If no intersection, return background if current ray not reflection
    if (t == FLT_MAX) {
        if (ref == 0)
            return back.colour();
        return colour;
    }

    vec4 P = r.pos(t);
    vec4 N = normalize(hit_sphere.normal(P));
    vec4 v = -2.f * dot_prod(N, r.direct()) * N + r.direct();

    //Calculate reflected ray colour effect
    auto ref_colour = hit_sphere.kr() * ray_color(ray(P, v), ++ref);
    // Calculate ambient light colour effect
    auto ambient_colour = vec3(ambient.Ir, ambient.Ig, ambient.Ib) * hit_sphere.ka() * hit_sphere.colour();
    // Calculate light source colour effect
    auto l_colour = light_colour(P, hit_sphere, t * r.direct(), N);
    // Add all colour effects together
    colour = ref_colour + ambient_colour + l_colour;

    return colour;
}

/// <summary>
/// Function finds intersections of the ray using the quadratic formula
/// Updates tn value with closest intersection point if one is found
/// If B^2 - AC < 0 -> no solution
/// If B^2 - AC == 0 -> no solution
/// If B^2 - AC > 0 -> closest intersection point (min value)
/// </summary>
/// <param name="v">Ray direction converted for canonical sphere</param>
/// <param name="S">Ray origin point converted for canonical sphere</param>
/// <param name="tn">Value to update to closest intersection</param>
/// <returns>True->intersection found. False->no intersection</returns>
bool intersect(vec4 v, vec4 S, float &tn, float &tf) {
    float a = dot_prod(v, v);
    float b = dot_prod(S, v);
    float c = dot_prod(S, S) - 1.f;
    float abc = pow(b, 2.f) - a * c;
    if (abc < 0.f)
        return false;
    float sq = sqrt(abc) / a;

    float ta = -b / a + sq;
    float tb = -b / a - sq;
    float close = tb;
    tf = ta;
    if (close < 0.f || FLT_MAX < close) {
        close = ta;
        tf = tb;
        if (close < 0.f || FLT_MAX < close)
            return false;
    }
    tn = close;
    return true;
}


/// <summary>
/// Sets each component of the input colour vector within
/// bounds [0.f, 1.f]
/// </summary>
/// <param name="col">Input colour vector</param>
/// <returns>Colour vector with all values within [0.f, 1.f]</returns>
vec3 scale_colour(vec3 col) {
    float x = max(0.f, min(1.f, col.x()));
    float y = max(0.f, min(1.f, col.y()));
    float z = max(0.f, min(1.f, col.z()));
    return vec3(x, y, z);
}


/// <summary>
/// Function writes the colours of each pixel(ray) to the output file
/// in P3 format
/// </summary>
void write_colour() {
    ofstream newFile;
    newFile.open(output);
    newFile << "P3\n" << res.x << " " << res.y << "\n255\n";
    for (vec3 colour : colours) {
        colour = scale_colour(colour);
        int ir = static_cast<int>(255.999f * colour[0]);
        int ig = static_cast<int>(255.999f * colour[1]);
        int ib = static_cast<int>(255.999f * colour[2]);
        newFile << ir << " " << ig << " " << ib << "\n";
    }
    newFile.close();
}


/// <summary>
/// Function parses input string to remove whitespace the and return vector
/// containing split strings.
/// Based on https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c
/// </summary>
/// <param name="input">String to be parsed</param>
/// <returns>Vector of strings split around delimiter</returns>
vector<string> removeWhiteSpace(string input) {
    vector<string> values{};
    size_t pos = 0;
    string token;
    string delimiter = "\t";
    while ((pos = input.find(delimiter)) != string::npos) {
        token = input.substr(0, pos);
        if (token.length() != 0)
            values.push_back(token);
        input.erase(0, pos + delimiter.length());
    }
    if (input.length() != 0)
        values.push_back(input);
    vector<string> result;
    delimiter = " ";
    for (string i : values) {
        while ((pos = i.find(delimiter)) != string::npos) {
            token = i.substr(0, pos);
            if (token.length() != 0)
                result.push_back(token);
            i.erase(0, pos + delimiter.length());
        }
        if (i.length() != 0)
            result.push_back(i);
    }
    return result;
}


/// <summary>
/// Function iterates through the input values and populates the fields for the scene
/// </summary>
/// <param name="values">Values for each field from input file</param>
void parseValues(vector<vector<string>> values) {
    for (vector<string> vec : values) {
        if (vec.size() == 0)
            continue;
        string start = *vec.begin();

        if (start.compare(NEAR) == 0) {
            plane.near = stoi(*--vec.end());
        }
        else if (start.compare(LEFT) == 0) {
            plane.left = stoi(*--vec.end());
        }
        else if (start.compare(RIGHT) == 0) {
            plane.right = stoi(*--vec.end());
        }
        else if (start.compare(TOP) == 0) {
            plane.top = stoi(*--vec.end());
        }
        else if (start.compare(BOTTOM) == 0) {
            plane.bottom = stoi(*--vec.end());
        }
        else if (start.compare(SPHERE_ID) == 0) {
            createSphere(vec);
        }
        else if (start.compare(AMBIENT_ID) == 0) {
            setAmbient(vec);
        }
        else if (start.compare(BACK_ID) == 0) {
            setBack(vec);
        }
        else if (start.compare(LIGHT_ID) == 0) {
            createLight(vec);
        }
        else if (start.compare(RES_ID) == 0) {
            setRes(vec);
        }
        else if (start.compare(OUTPUT) == 0) {
            output = *--vec.end();
        }
    }
}


/// <summary>
/// Populates the values for the ambient
/// </summary>
/// <param name="values">Strings from input file containing ambient values</param>
void setAmbient(vector<string> values) {
    if (values.size() != 4)
        cout << "AMBIENT does not contain expected number of values";
    ambient.Ir = stof(*(1 + values.begin()));
    ambient.Ig = stof(*(2 + values.begin()));
    ambient.Ib = stof(*(3 + values.begin()));
}


/// <summary>
/// Populates the values for the back
/// </summary>
/// <param name="values">Strings from input file containing back values</param>
void setBack(vector<string> values) {
    if (values.size() != 4)
        cout << "BACK does not contain expected number of values";
    back.r = stof(*(1 + values.begin()));
    back.g = stof(*(2 + values.begin()));
    back.b = stof(*(3 + values.begin()));
}


/// <summary>
/// Populates the values for the resolution
/// </summary>
/// <param name="values">Strings from input file containing resolution values</param>
void setRes(vector<string> values) {
    if (values.size() != 3)
        cout << "RES does not contain expected number of values";
    res.x = stoi(*(1 + values.begin()));
    res.y = stoi(*(2 + values.begin()));
}


/// <summary>
/// Creates a new sphere object that is then added to the sphere vector
/// </summary>
/// <param name="values">Strings from input file containing sphere values</param>
void createSphere(vector<string> values) {
    if (values.size() != 16)
        cout << "SPHERE does not contain expected number of values";
    
    string name = *(1 + values.begin());
    float xPos = stof(*(2 + values.begin()));
    float yPos = stof(*(3 + values.begin()));
    float zPos = stof(*(4 + values.begin()));
    float xScl = stof(*(5 + values.begin()));
    float yScl = stof(*(6 + values.begin()));
    float zScl = stof(*(7 + values.begin()));
    float r = stof(*(8 + values.begin()));
    float g = stof(*(9 + values.begin()));
    float b = stof(*(10 + values.begin()));
    float Ka = stof(*(11 + values.begin()));
    float Kd = stof(*(12 + values.begin()));
    float Ks = stof(*(13 + values.begin()));
    float Kr = stof(*(14 + values.begin()));
    int n = stoi(*(15 + values.begin()));
    sphere sphere(name, xPos, yPos, zPos, xScl, yScl, zScl, r, g, b, Ka, Kd, Ks, Kr, n);
    spheres.push_back(sphere);
}


/// <summary>
/// Creates a new light object that is then added to the lights vector
/// </summary>
/// <param name="values">Strings from input file containing light values</param>
void createLight(vector<string> values) {
    if (values.size() != 8)
        cout << "LIGHT does not contain expected number of values";
    
    string name = *(1 + values.begin());
    float xPos = stof(*(2 + values.begin()));
    float yPos = stof(*(3 + values.begin()));
    float zPos = stof(*(4 + values.begin()));
    float Ir = stof(*(5 + values.begin()));
    float Ig = stof(*(6 + values.begin()));
    float Ib = stof(*(7 + values.begin()));
    lights.push_back(light(name, xPos, yPos, zPos, Ir, Ig, Ib));
}

