#include "skybox.h"

// constuctor
SkyBox::SkyBox() {
	createVertices();
};

// constuctor
SkyBox::SkyBox(const char* fname) {
	createVertices();
	// load texture from file
	m_texture = new Texture(fname);
	if (m_texture)
		hasTex = true;
	else
		hasTex = false;
};

// destructor
SkyBox::~SkyBox() {
	Vertices.clear();
	Indices.clear();
}

// fill out the Verices and Indices Attributes
void SkyBox::createVertices() {
	Vertices = {
			{{1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
			{{1.0f, -1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
			{{-1.0f, -1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
			{{-1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
			{{1.0f, 1.0f, -1.0f}, {1.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
			{{1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
			{{-1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
			{{-1.0f, 1.0f, -1.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}}
	};

	Indices = {
			  1, 2, 3,
			  7, 6, 5,
			  0, 4, 5,
			  1, 5, 6,
			  6, 7, 3,
			  0, 3, 7,
			  0, 1, 3,
			  4, 7, 5,
			  1, 0, 5,
			  2, 1, 6,
			  2, 6, 3,
			  4, 0, 7
	};
};


void SkyBox::Render(GLint positionAttribLoc, GLint colorAttribLoc)
{

	glBindVertexArray(vao);

	// Enable vertex attibute arrays for each vertex attrib
	glEnableVertexAttribArray(positionAttribLoc);
	glEnableVertexAttribArray(colorAttribLoc);

	// Bind your VBO
	glBindBuffer(GL_ARRAY_BUFFER, VB);

	// Set vertex attribute pointers to the load correct data
	glVertexAttribPointer(positionAttribLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(colorAttribLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));


	// Bind your index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

	// Render
	glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);

	// Disable vertex arrays
	glDisableVertexAttribArray(positionAttribLoc);
	glDisableVertexAttribArray(colorAttribLoc);
}

void SkyBox::Render(GLint posAttribLoc, GLint colAttribLoc, GLint tcAttribLoc, GLint hasTextureLoc)
{

	glBindVertexArray(vao);
	// Enable vertex attibute arrays for each vertex attrib
	glEnableVertexAttribArray(posAttribLoc);
	glEnableVertexAttribArray(colAttribLoc);
	glEnableVertexAttribArray(tcAttribLoc);

	// Bind your VBO
	glBindBuffer(GL_ARRAY_BUFFER, VB);

	// Set vertex attribute pointers to the load correct data. Update here to load the correct attributes.
	glVertexAttribPointer(posAttribLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(colAttribLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glVertexAttribPointer(tcAttribLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texcoord));


	// If has texture, set up texture unit(s): update here for texture rendering
	// If has texture, set up texture unit(s) Update here to activate and assign texture unit
	if (m_texture != NULL) {
		glUniform1i(hasTextureLoc, true);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_texture->getTextureID());
	}
	else
		glUniform1i(hasTextureLoc, false);



	// Bind your Element Array
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

	// Render
	glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);

	// Disable vertex arrays
	glDisableVertexAttribArray(posAttribLoc);
	glDisableVertexAttribArray(colAttribLoc);
	glDisableVertexAttribArray(tcAttribLoc);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

//// prepare vertex buffer, prepare vertex array object, and transform object in preparation for rendering
//void SkyBox::Initialize(GLint posAttribLoc, GLint colAttribLoc) {
//
//	// Set up your VOA
//	glGenVertexArrays(1, &vao);
//	glBindVertexArray(vao);
//
//	// setting the Vertex VBO
//	glGenBuffers(1, &VB);
//	glBindBuffer(GL_ARRAY_BUFFER, VB);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);
//	glVertexAttribPointer(posAttribLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
//	glVertexAttribPointer(colAttribLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
//
//	// Setting the Index VBO
//	glGenBuffers(1, &IB);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Indices.size(), &Indices[0], GL_STATIC_DRAW);
//
//	// Computing the model matrix
//	// Model orientation
//	angle = 1 * 3.1415f;
//
//	float tvec1 = glm::linearRand(-5.f, 5.f);
//	float tvec2 = glm::linearRand(-5.f, 5.f);
//	float tvec3 = glm::linearRand(-2.f, 2.f);
//
//	model = glm::translate(glm::mat4(1.0f), glm::vec3(tvec1, tvec2, tvec3));
//	model *= glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 1.0f, .0f));
//
//}

// move the object around the scene
void SkyBox::Update(unsigned int dt) {
	//model = glm::translate(model, m_speed);

	model *= glm::rotate(glm::mat4(1.0f), m_rotate, glm::vec3(.0f, .0f, 1.f));

}

// return our 3d model
glm::mat4 SkyBox::GetModel() {
	return model;
}


void SkyBox::setupModelMatrix(glm::vec3 pivot, float angle, float scale) {
	pivotLocation = pivot;
	model = glm::translate(glm::mat4(1.0f), pivotLocation);
	model *= glm::rotate(glm::mat4(1.f), angle, glm::vec3(0., 1., 0));
	model *= glm::scale(glm::vec3(scale, scale, scale));
}

//// bind our buffers, send the data to the GPU, and draw our object on screen
//void SkyBox::Render(GLint posAttribLoc, GLint colAttribLoc) {
//	// Bind VAO
//	glBindVertexArray(vao);
//
//	// Bind VBO(s)
//	glBindBuffer(GL_ARRAY_BUFFER, VB);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
//
//	// enable the vertex attribute arrays 
//	// this is the poistion attrib in the vertex shader
//	glEnableVertexAttribArray(posAttribLoc);
//	// this is the color attribe in the vertex shader
//	glEnableVertexAttribArray(colAttribLoc);
//
//	// Draw call to OpenGL
//	glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);
//
//	// disable the vertex attributes
//	glDisableVertexAttribArray(posAttribLoc);
//	glDisableVertexAttribArray(colAttribLoc);
//
//	// unbind VBO(s) and ElementBuffer(s)
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//
//}
void SkyBox::setupBuffers() {
	// For OpenGL 3
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &VB);
	glBindBuffer(GL_ARRAY_BUFFER, VB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &IB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Indices.size(), &Indices[0], GL_STATIC_DRAW);
}