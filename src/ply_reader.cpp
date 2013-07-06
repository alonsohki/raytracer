// This code is "Public Domain", no rights reserved.

#define _CRT_SECURE_NO_WARNINGS

#include "ply_reader.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "Face.h"
#include "Vector.h"

using namespace std;

auto_ptr<PLY_Model> Read_PLY_Model(const char *filename)
{
	FILE *file = fopen(filename, "r");

	// Error checks omitted for simplicity.
	assert(file);

	// Parse header
	char header_field[1024] = "\0";
	int  vertex_count = 0;
	int  face_count = 0;

	while (strcmp(header_field, "end_header"))
	{
		fscanf(file, "%s", header_field);

		if (!strcmp(header_field, "vertex"))
			fscanf(file, "%d", &vertex_count);

		if (!strcmp(header_field, "face"))
			fscanf(file, "%d", &face_count);
	}

	// Construct the target buffers
	auto_ptr<PLY_Model> res(new PLY_Model());
	res->m_vertex_data.reserve(vertex_count);
	res->m_face_data.reserve(face_count);

	// Read vertex data
	for (int i = 0; i < vertex_count; ++i)
	{
        double values[3];
        fscanf(file, "%lf", &values[0]);
        fscanf(file, "%lf", &values[1]);
        fscanf(file, "%lf", &values[2]);
		res->m_vertex_data.push_back(vec3f((float)values[0], (float)values[1], (float)values[2]));
	}

	// Read face (triangles) data
	for (int i = 0; i < face_count; ++i)
	{
        int numVertices;
        fscanf(file, "%d", &numVertices);
        
        // Beginning of the face data row, assert that we have triangles
        assert(numVertices == 3);
        
        Face face;
        fscanf(file, "%d", &face.v1);
        fscanf(file, "%d", &face.v2);
        fscanf(file, "%d", &face.v3);
    	res->m_face_data.push_back(face);
	}

	fclose(file);

	return res;
}
