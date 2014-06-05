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
    
    worldFile << "#VRML_SIM V7.4.3 utf8 \n WorldInfo { info [ \"Description\" \"Author: first name last name <e-mail>\" \"Date: DD MMM YYYY\" ] basicTimeStep 16 contactProperties [ ContactProperties { material2 \"slippery\" coulombFriction 0 bounce 0 } ] } Viewpoint { fieldOfView 0.78539 orientation -0.297522 -0.909481 -0.290388 1.86939 position -37.2068 27.9927 -6.17963 } Background { skyColor [ 0.4 0.7 1 ] } DirectionalLight { direction -10 -10 -10 intensity 0.45 } DirectionalLight { direction -10 -10 10 intensity 0.45 } DirectionalLight { direction 10 -10 10 intensity 0.45 } DirectionalLight { direction 10 -10 -10 intensity 0.45 } CircleArena { radius 15 floorTextureUrl [ \"textures/grid.png\" ] floorTileSize 1 1 wallThickness 0.3 wallHeight 1.5 subdivision 128 } ";
    
    worldFile << "DEF CLINIC_PLATFORM Solid { rotation 0.57735 0.57735 0.57735 0 children [ DEF CLINIC_PLATFORM_SHAPE Shape { appearance Appearance { material Material { diffuseColor 1 1 1 transparency 0.5 } } geometry ";
    
    worldFile << getSlicedCylinder();

    worldFile << " } ] contactMaterial \"slippery\" boundingObject USE CLINIC_PLATFORM_SHAPE } ";

    if (type.compare("D") == 0)
    {
        worldFile << "DEF FERTILITY_CIRCLE Solid { children [ Shape { appearance Appearance { material Material { diffuseColor 0.42295 0.8 0.131441 transparency 0.8 } } geometry Cylinder { height 0.1 radius 5 subdivision 128 } } ] } ";
    }
    
    for (int i = 0; i < numberOfModules; i++)
    {
        std::string x = std::to_string((i % 10) - 5);
        std::string z = std::to_string((i / 10) - 5);
        std::string idx = std::to_string(i+1);
        
        worldFile << "DEF MODULE_" + idx + " Roombot { translation " + x + " 0.058 " + z + " name \"Roombot:" + idx + "\" } ";
    }
    
    worldFile << "DEF CLINIC Supervisor { translation 0 0.5 0 children [ Emitter { } Receiver { } ] controller \"BirthClinicController\" } DEF EVOLVER Supervisor { children [ Emitter { } Receiver { } ] controller \"EvolverController\" } DEF MODIFIER Supervisor { children [ Emitter { } Receiver { } ] controller \"EnvironmentModifierController\" } DEF SCREENSHOTS Supervisor { children [ Camera { translation 0 6.5 0 rotation 0 0.707107 0.707107 3.1415 width 256 height 256 maxRange 15 windowPosition 1 1 antiAliasing TRUE } Receiver { } ] controller \"ScreenshotController\" } DEF TIMING Supervisor {name \"TimingSupervisor\" controller \"TimingController\" }";
    
    worldFile.close();

    return 0;
}