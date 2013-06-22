// This code is "Public Domain", no rights reserved.

#ifndef PLY_READER_H
#define PLY_READER_H

#include <vector>
#include <memory>
#include <assert.h>
#include <float.h>

struct PLY_Model
{
	// Helper accessors
	float Get_Coordinate(size_t face, int triangle_vtx_idx, int axis) const
	{
		assert(face >= 0 && face < Get_Face_Count());
		assert(triangle_vtx_idx >= 0 && triangle_vtx_idx < 3);
		assert(axis >= 0 && axis < 3);

		size_t vertex_idx = m_face_data[face*3 + triangle_vtx_idx];
		assert(vertex_idx >= 0 && vertex_idx < Get_Vertex_Count());

		return m_vertex_data[vertex_idx*3 + axis];
	}

	// Note: Slow O(n)
#define BOUND_FUN(type, begin, compare)\
	float Get_Bound_##type(int axis) const			\
	{												\
		size_t	vtx_cnt = m_vertex_data.size();		\
		float	bound	= begin;					\
		for (size_t i = 0; i < vtx_cnt; i += 3)		\
			if (m_vertex_data[i+axis] compare bound)\
				bound = m_vertex_data[i+axis];		\
		return bound;								\
	}

	BOUND_FUN(Min,  FLT_MAX, <)
	BOUND_FUN(Max, -FLT_MAX, >)

#undef BOUND_FUN

	size_t Get_Vertex_Count() const
	{
		return m_vertex_data.size() / 3;
	}

	size_t Get_Face_Count() const
	{
		return m_face_data.size() / 3;
	}

	// Public raw model data
	std::vector<float>	m_vertex_data;
	std::vector<int>	m_face_data;
};

// Note: This is not a general PLY model reader, it works only with the
// data from Stanford 3D Scanning Repository.  For example, the happy
// buddha model from http://graphics.stanford.edu/data/3Dscanrep/ .

std::auto_ptr<PLY_Model> Read_PLY_Model(const char *filename);

#endif
