#include "Mesh.h"

namespace tf
{
	void SOAMesh::loadObjFile(const char* path)
	{
		// @TODO
	}

	void SOAMesh::loadRawData(SOAVertex* vertices)
	{
		numIndices = vertices->numIndices;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glGenBuffers(NumAttr, attributes);
		u32 numVerts = vertices->numVertices;
		_addAttr(vertices->positions, numVerts * sizeof(vertices->positions[0]), 3, GL_FLOAT, PositionAttr);
		_addAttr(vertices->texCoords, numVerts * sizeof(vertices->texCoords[0]), 2, GL_FLOAT, TextureAttr);
		_addAttr(vertices->normals, numVerts * sizeof(vertices->normals[0]), 3, GL_FLOAT, NormalAttr);
		_addAttr(vertices->indices, numVerts * sizeof(vertices->indices[0]), 1, GL_UNSIGNED_INT, IndexAttr, GL_ELEMENT_ARRAY_BUFFER);
	}

	void SOAMesh::_addAttr(void* buff, u32 size,
		u32 components_per_attr,
		GLenum type, AttributeType attr,
		GLenum buffer_target_type)
	{
		glBindBuffer(buffer_target_type, attributes[attr]);
		glBufferData(buffer_target_type, size, buff, GL_STATIC_DRAW);
		glEnableVertexAttribArray(attr);
		glVertexAttribPointer(attr, components_per_attr, type, GL_FALSE, 0, 0);
	}

	void SOAMesh::draw()
	{
		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, NULL);
	}

	void SOAMesh::unload()
	{
		glDeleteBuffers(NumAttr, attributes);
		glDeleteVertexArrays(1, &vao);
	}
}

namespace tf
{
	void AOSMesh::loadObjFile(const char* path)
	{
		// @TODO
	}

	void AOSMesh::loadRawData(AOSVertex* vertices, u32 numVertices, u32* indices, u32 _numIndices)
	{
		numIndices = _numIndices;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glGenBuffers(NumBuffers, buffers);

		glBindBuffer(GL_ARRAY_BUFFER, buffers[VertexBuffer]);
		glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(vertices[0]), vertices, GL_STATIC_DRAW);

		u32 stride = sizeof(vertices[0]);

		_addAttr(PositionAttr, 3, stride, offsetof(AOSVertex, position));
		_addAttr(TextureAttr, 2, stride, offsetof(AOSVertex, texCoord));
		_addAttr(NormalAttr, 3, stride, offsetof(AOSVertex, normal));

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[IndexBuffer]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(indices[0]), indices, GL_STATIC_DRAW);
	}

	inline void AOSMesh::_addAttr(AttributeType attr, u32 components_per_attr, u32 stride, u32 offset)
	{
		glEnableVertexAttribArray(attr);
		glVertexAttribPointer(attr, components_per_attr, GL_FLOAT, GL_FALSE, stride, (void*)offset);
	}

	void AOSMesh::draw()
	{
		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, NULL);
	}

	void AOSMesh::unload()
	{
		glDeleteBuffers(NumBuffers, buffers);
		glDeleteVertexArrays(1, &vao);
	}
}