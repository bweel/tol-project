#include <fstream>
#include <vector>
#include <math.h>
#include <iostream>


static std::string getSlicedCylinder()
{
    double h = 1.6;
    double r = 2;
    double slope_factor = 0.7;
    double pi = 3.14159;
    int subdivisions = 128;
    
    std::vector<double> x = std::vector<double>();
    std::vector<double> z = std::vector<double>();
    std::vector<double> angle = std::vector<double>();
    
    double a = 0;
    while (a < (2*pi))
    {
        angle.push_back(a);
        a = a + ((2*pi)/subdivisions);
    }
    
    for (int i = 0; i < angle.size(); i++)
    {
        x.push_back(sin(angle[i]) * r);
        z.push_back(cos(angle[i]) * r);
    }
    
    std::string text = "IndexedFaceSet { coord Coordinate { point [ ";
    for (int i = 0; i < x.size(); i++)
    {
        std::string point = std::to_string(x[i]) + " 0 " + std::to_string(z[i]);
        text = text + point + " ";
    }
    
    for (int i = 0; i < x.size(); i++)
    {
        std::string point = std::to_string(x[i]) + " " + std::to_string(h + x[i] * slope_factor) + " " + std::to_string(z[i]);
        if (i == x.size()-1)
        {
            text = text + point + " ] ";
        }
        else{
            text = text + point + " ";
        }
    }
    
    text = text + " } coordIndex [ ";
    
    for (int i = 0; i < x.size(); i++)
    {
        text = text + std::to_string(i) + ", ";
    }
    text = text + "-1 ";
    
    for (int i = 0; i < x.size()-1; i++)
    {
        text = text + std::to_string(i) + ", " + std::to_string(i + 1) + ", " + std::to_string(i + x.size() + 1) + ", " + std::to_string(i + x.size()) + ", -1 ";
    }
    text = text + std::to_string(x.size()-1) + ", 0, " + std::to_string(x.size()) + ", " + std::to_string(x.size()*2-1) + ", -1 ";
    
    for (int i = 0; i < x.size(); i++)
    {
        text = text + std::to_string(i + x.size()) + ", ";
    }
    text = text + "-1 ";
    text = text + "] }";
    
    return text;
}

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        std::cout << "not enough arguments" << std::endl;
        return 0;
    }
    
    std::string type = argv[1];
    int numberOfModules = std::atoi(argv[2]);
    
    if (type.compare("D") != 0 && type.compare("C") != 0)
    {
        std::cout << "specify world type: C for centralized and D for distributed" << std::endl;
        return 0;
    }
    
    std::ofstream worldFile;
    worldFile.open("world.wbt");
    
    worldFile << "\
#VRML_SIM V7.4.3 utf8 \n\
WorldInfo {\n\
    info [\n\
        \"Description\"\n\
        \"Author: first name last name <e-mail>\"\n\
        \"Date: DD MMM YYYY\"\n\
    ]\n\
    basicTimeStep 16\n\
    contactProperties [\n\
        ContactProperties {\n\
            material2 \"slippery\"\n\
            coulombFriction 0\n\
            bounce 0\n\
        }\n\
    ]\n\
}\n\
Viewpoint {\n\
    fieldOfView 0.78539\n\
    orientation -0.297522 -0.909481 -0.290388 1.86939\n\
    position -37.2068 27.9927 -6.17963\n\
}\n\
Background {\n\
    skyColor [ 0.4 0.7 1 ]\n\
}\n\
DirectionalLight {\n\
        direction -10 -10 -10\n\
        intensity 0.45 \n\
}\n\
DirectionalLight {\n\
        direction -10 -10 10\n\
        intensity 0.45\n\
}\n\
DirectionalLight {\n\
    direction 10 -10 10\n\
    intensity 0.45\n\
}\n\
DirectionalLight {\n\
        direction 10 -10 -10\n\
        intensity 0.45\n\
}\n\
CircleArena {\n\
    radius 15\n\
    floorTextureUrl [ \"textures/grid.png\" ]\n\
    floorTileSize 1 1\n\
    wallThickness 0.3\n\
    wallHeight 1.5\n\
    subdivision 128\n\
}\n";
    
    worldFile << "\
DEF CLINIC_PLATFORM Solid {\n\
    rotation 0.57735 0.57735 0.57735 0\n\
    children [\n\
        DEF CLINIC_PLATFORM_SHAPE Shape {\n\
            appearance Appearance {\n\
            material Material {\n\
                diffuseColor 1 1 1\n\
                transparency 0.5\n\
        }\n\
    }\n\
    geometry\n";
    
    worldFile << getSlicedCylinder();

    worldFile << "\n\
        }\n\
    ]\n\
    contactMaterial \"slippery\"\n\
    boundingObject USE CLINIC_PLATFORM_SHAPE\n\
}\n";

    if (type.compare("D") == 0)
    {
        worldFile << "\
DEF FERTILITY_CIRCLE Solid {\n\
    children [\n\
        Shape {\n\
            appearance Appearance {\n\
                material Material {\n\
                    diffuseColor 0.42295 0.8 0.131441\n\
                    transparency 0.8\n\
                }\n\
            }\n\
            geometry Cylinder {\n\
                height 0.1\n\
                radius 5\n\
                subdivision 128\n\
            }\n\
        }\n\
    ]\n\
}\n";
    }
    
    worldFile << "\
DEF CLINIC Supervisor {\n\
    translation 0 0.5 0\n\
    children [\n\
        Emitter { }\n\
        Receiver { }\n\
    ]\n\
    controller \"BirthClinicController\"\n\
}\n\
DEF EVOLVER Supervisor {\n\
    children [\n\
        Emitter { }\n\
        Receiver { }\n\
    ]\n\
    controller \"EvolverController\"\n\
}\n\
DEF MODIFIER Supervisor {\n\
    children [\n\
        Emitter { }\n\
        Receiver { }\n\
    ]\n\
    controller \"EnvironmentModifierController\"\n\
}\n\
DEF SCREENSHOTS Supervisor {\n\
    children [\n\
        Transform {\n\
            translation 0 4.5 0\n\
            children [\n\
                  Shape {\n\
                      appearance Appearance {\n\
                          material Material {\n\
                              diffuseColor 0.471931 0.476204 0.489006\n\
                              shininess 0\n\
                          }\n\
                      }\n\
                      geometry Box {\n\
                          size 2 0.1 2\n\
                      }\n\
                  }\n\
              ]\n\
        }\n\
        Camera {\n\
            translation 0 6.5 0\n\
            rotation 0 0.707107 0.707107 3.1415\n\
            width 256\n\
            height 256\n\
            maxRange 15\n\
            windowPosition 1 1\n\
            antiAliasing TRUE\n\
        }\n\
        Receiver { }\n\
    ]\n\
controller \"ScreenshotController\"\n\
}\n\
DEF TIMING Supervisor {\n\
    name \"TimingSupervisor\"\n\
    controller \"TimingController\"\n\
}\n";

    for (int i = 0; i < numberOfModules; i++)
    {
        std::string x = std::to_string((i % 10) - 5);
        std::string z = std::to_string((i / 10) - 5);
        std::string idx = std::to_string(i+1);
        
        worldFile << "\
DEF MODULE_" + idx + " Roombot {\n\
    translation " + x + " 0.058 " + z + "\n\
    name \"Roombot:" + idx + "\"\n\
}\n";
    }
    
    worldFile.close();

    return 0;
}