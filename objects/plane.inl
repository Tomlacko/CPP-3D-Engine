#pragma once

#include <cstdint>

const int plane_vertices_count = 4;
const float plane_vertices[plane_vertices_count * 8] =
{
	//       Position                 Normal             Tex Coord
	-0.5f, 0.0f,-0.5f,		 0.0f, 1.0f, 0.0f,		0.0f, 1.0f,
	-0.5f, 0.0f, 0.5f,		 0.0f, 1.0f, 0.0f,		0.0f, 0.0f,
	0.5f, 0.0f, 0.5f,		 0.0f, 1.0f, 0.0f,		1.0f, 0.0f,
	0.5f, 0.0f,-0.5f,		 0.0f, 1.0f, 0.0f,		1.0f, 1.0f,
};

const int plane_indices_count = 6;
const uint32_t plane_indices[plane_indices_count] =
{
	0, 2, 1,		0, 3, 2		// Top face
};
