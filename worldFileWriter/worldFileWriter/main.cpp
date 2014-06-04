#include <fstream>
#include <vector>
#include <math.h>


static std::string getSlicedCylinder()
{
    double h = 1.5;
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
    std::ofstream worldFile;
    worldFile.open("world.wbt");
    
    worldFile << "#VRML_SIM V7.4.3 utf8 \n WorldInfo { info [ \"Description\" \"Author: first name last name <e-mail>\" \"Date: DD MMM YYYY\" ] contactProperties [ ContactProperties { material2 \"slippery\" coulombFriction 0 bounce 0 } ] } Viewpoint { fieldOfView 0.78539 orientation -0.247418 -0.93182 -0.265511 1.96088 position -31.1922 20.4769 -11.3495 } Background { skyColor [ 0.4 0.7 1 ] } DirectionalLight { direction -10 -10 -10 intensity 0.45 } DirectionalLight { direction -10 -10 10 intensity 0.45 } DirectionalLight { direction 10 -10 10 intensity 0.45 } DirectionalLight { direction 10 -10 -10 intensity 0.45 } CircleArena { radius 15 floorTextureUrl [ \"textures/grid.png\" ] floorTileSize 1 1 wallThickness 0.3 wallHeight 1.5 subdivision 128 } ";
    
    worldFile << "DEF CLINIC_PLATFORM Solid { rotation 0.57735 0.57735 0.57735 0 children [ DEF CLINIC_PLATFORM_SHAPE Shape { appearance Appearance { material Material { diffuseColor 1 1 1 transparency 0.5 } } geometry ";
    
    worldFile << getSlicedCylinder();

    worldFile << " } ] contactMaterial \"slippery\" boundingObject USE CLINIC_PLATFORM_SHAPE } ";

    for (int i = 0; i < 50; i++)
    {
        std::string x = std::to_string((i % 10) - 5);
        std::string z = std::to_string((i / 10) - 5);
        std::string idx = std::to_string(i+1);
        
        worldFile << "DEF MODULE_" + idx + " Roombot { translation " + x + " 0.058 " + z + " name \"Roombot:" + idx + "\" } ";
    }
    
    worldFile << "DEF CLINIC Supervisor { translation 0 0.5 0 children [ Emitter { } Receiver { } ] controller \"BirthClinicController\" } DEF EVOLVER Supervisor { children [ Emitter { } Receiver { } ] controller \"EvolverController\" } DEF MODIFIER Supervisor { children [ Emitter { } Receiver { } ] controller \"EnvironmentModifierController\" } DEF SCREENSHOTS Supervisor { children [ Receiver { } ] controller \"ScreenshotController\" } DEF TIMING Supervisor {name \"TimingSupervisor\" controller \"TimingController\" }";
    
    worldFile.close();

    return 0;
}