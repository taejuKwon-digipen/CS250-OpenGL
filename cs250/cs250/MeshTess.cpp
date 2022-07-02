#include "MeshTess.h"
#include <array>

MeshTess MeshTess::CreateIcosahedron()
{
	const std::array Faces = { 2, 1,  0, 3, 2,  0, 4, 3, 0, 5, 4, 0, 1, 5, 0, 11, 6, 7, 11, 7,
							   8, 11, 8, 9, 11, 9, 10, 11, 10, 6, 1, 2, 6, 2, 3, 7, 3, 4, 8, 4,
							   5, 9, 5, 1, 10, 2, 7, 6, 3, 8, 7, 4, 9, 8, 5, 10, 9, 1, 6, 10 };
	
	const std::array Verts = { Vertex{0.000f, 0.000f, 1.000f},	 Vertex{0.894f, 0.000f, 0.447f},
							   Vertex{0.276f, 0.851f, 0.447f},	 Vertex{-0.724f, 0.526f, 0.447f},
							   Vertex{-0.724f, -0.526f, 0.447f}, Vertex{0.276f, -0.851f, 0.447f},
							   Vertex{0.724f, 0.526f, -0.447f},  Vertex{-0.276f, 0.851f, -0.447f},
							   Vertex{-0.894f, 0.000f, -0.447f}, Vertex{-0.276f, -0.851f, -0.447f},
							   Vertex{0.724f, -0.526f, -0.447f}, Vertex{0.000f, 0.000f, -1.000f} };

	MeshTess meshTess;
	meshTess.indexBuffer = std::vector<int>{ std::begin(Faces), std::end(Faces) };
	meshTess.vertexBuffer = std::vector<Vertex>{ std::begin(Verts), std::end(Verts) };
	meshTess.numIndices = static_cast<int>(Faces.size());
	meshTess.numVertices = static_cast<int>(Verts.size());
	return meshTess;
}

void MeshTess::SendVertexData(MeshTess& mesh)
{
	const int vertexSize = sizeof(Vertex);
	const int indexSize = sizeof(int);

	struct VertexLayout
	{
		int location;
		int size;
		int type;
		bool normalized;
		int offset;
	};
	const VertexLayout vLayout[] = { {0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, pos)} };

	const int layoutSize = sizeof(VertexLayout);
	const int numAttribs = sizeof(vLayout) / layoutSize;

	glGenVertexArrays(1, &mesh.VAO);
	glBindVertexArray(mesh.VAO);

	glGenBuffers(1, &mesh.VBO);
	glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);

	glBufferData(GL_ARRAY_BUFFER, mesh.numVertices* vertexSize, &mesh.vertexBuffer[0], GL_STATIC_DRAW);

	glGenBuffers(1, &mesh.EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.numIndices* indexSize, &mesh.indexBuffer[0], GL_STATIC_DRAW);

	for (int i = 0; i < numAttribs; ++i)
	{
		glEnableVertexAttribArray(vLayout[i].location);
		glVertexAttribPointer(vLayout[i].location, vLayout[i].size, vLayout[i].type, vLayout[i].normalized, vertexSize, reinterpret_cast<void*>(vLayout[i].offset + 0LL));
	}
}