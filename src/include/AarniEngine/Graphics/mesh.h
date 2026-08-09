#ifndef _MESHH_
#define _MESHH_
#include <AarniEngine/Math/vector.h>

#include <iostream>
#include <fstream>
#include <string>

struct Face
{
    Vector3 *a,*b,*c; //Pointers to vertexes of the face
};

struct Mesh
{
    int vertexCount = 0;
    int faceCount = 0;
    Vector3 (*vertexes)[];
    Face (*faces)[];

    static Mesh LoadModel(std::string filename)
    {
        Mesh m = {};
        filename = "src/Game/Models/" + filename;
        // # ----- Quick counting ----- # //
        std::string rowText;
        int vertexMaxCount = 0;
        int faceMaxCount = 0;
        // Counts the amount of vertexes, faces, etc. in the file.
        std::ifstream MyReadFile(filename);
        if (!MyReadFile.is_open()) {
            std::cerr << "Failed to open file\n";
        }
        while (std::getline (MyReadFile, rowText))
        {
            char arr[rowText.length() + 1]; //char arr version of the string.
            strcpy(arr, rowText.c_str());
            
            char* rowType = strtok(arr, " ");
            if(strcmp(rowType,"v") == 0)
            {
                vertexMaxCount++;
            }
            else if (strcmp(rowType,"f") == 0)
            {
                faceMaxCount++;
            }
        }

        
        m.vertexCount = vertexMaxCount;
        m.faceCount = faceMaxCount;
        
        //Allocating memory.
        m.vertexes = (Vector3(*)[]) malloc(vertexMaxCount * sizeof(Vector3));
        m.faces = (Face(*)[]) malloc(faceMaxCount * sizeof(Face));
        
        int vertexIndex = 0;
        int faceIndex = 0;
        
        // Check if the end of file flag is set, and clear it
        if (MyReadFile.eof()) {
            MyReadFile.clear(); // Clear any error flags, including eof
        }
        MyReadFile.seekg(0);
        // Use a while loop together with the getline() function to read the file line by line
        while (std::getline (MyReadFile, rowText))
        {
            //Parses the data.
            char arr[rowText.length() + 1]; //char arr version of the string.
            strcpy(arr, rowText.c_str());
            char* rowType = strtok(arr, " "); 
            
            if(strcmp(rowType,"v") == 0)
            {
                (*m.vertexes)[vertexIndex] = {std::stof(strtok(NULL, " ")),std::stof(strtok(NULL, " ")),std::stof(strtok(NULL, " "))};
                vertexIndex++;
            }
            else if(strcmp(rowType,"f") == 0)
            {
                (*m.faces)[faceIndex] = {&(*m.vertexes)[std::stoi(strtok(NULL," ")) - 1], &(*m.vertexes)[std::stoi(strtok(NULL," ")) - 1], &(*m.vertexes)[std::stoi(strtok(NULL," ")) - 1]};
                faceIndex++;
            }
        }
        MyReadFile.close();
        return m;
    }
    
    void PrintMeshData()
    {
        std::cout << "Data of the mesh:" << std::endl;
        for(int i = 0; i < vertexCount; i++)
        {
            std::cout << (i + 1) << "." << " v " << (*vertexes)[i] << std::endl;
        }
        for(int i = 0; i < faceCount; i++)
        {
            std::cout << (i + 1) << "." << " f " << *(*faces)[i].a << " " << *(*faces)[i].b << " " << *(*faces)[i].c << std::endl;
        }
    }
};

#endif