#include "Star.h"

#define BOUNDING_RADIUS_SCALE 4.4

unsigned int Star::instanceCount = 0;
GLuint Star::texture = 0;

GLuint Star::GenerateStarTexture(const unsigned int size) {
	// Gaussian texture
	GLubyte pixels[size][size];
	for (unsigned int i = 0; i < size; i++) {
		for (unsigned int j = 0; j < size; j++) {
			const double x = (2.0 * i - size) / size;
			const double y = (2.0 * j - size) / size;
//      pixels[j][i] = (GLubyte)(255.0 * exp(-6.0 * (x * x + y * y)));
			pixels[j][i] = (GLubyte) (255.0 * exp(-6.0 * (x * x + y * y)));
		}
	}

	// Generate texture
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE8, size, size, 0, GL_LUMINANCE,
			GL_UNSIGNED_BYTE, pixels);
	//glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	return texture;
}

Star::Star(const double radius) :
		radius(radius) {
	// If first instance to be created
	if (!instanceCount++)
		texture = GenerateStarTexture(1024);
}

Star::~Star() {
	// If last instance to be deleted
	if (!--instanceCount) {
		glDeleteTextures(1, &texture);
		texture = 0;
	}
}

double Star::GetBoundingRadius() const {
	// Greater than the actual radius, to cover the glow
	return radius * BOUNDING_RADIUS_SCALE;
}

double Star::GetHoverBoundingRadius() const {
	return radius;
}

Vector3<double> Star::GetClosestSurfacePoint(const Vector3<double> &pos) const {
	return TransformToWorld(TransformToLocal(pos, true).GetNormalized(), false)
			* radius;
}

double Star::GetClosestSurfaceDistance(const Vector3<double> &pos) const {
	return TransformToLocal(pos, true).GetLength() - radius;
}

void Star::Render() const {
	// Remove orientation part from modelview matrix to achieve billboarding
	glPushMatrix();
	if (translateWhenRendering)
		glTranslatef(position.x, position.y, position.z);
	GLdouble modelview[16];
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	modelview[1] = modelview[2] = modelview[4] = modelview[6] = modelview[8] =
			modelview[9] = 0.0;
	modelview[0] = modelview[5] = modelview[10] = 1.0;
	glLoadMatrixd(modelview);
	const double renderScale = GetBoundingRadius();
	glScaled(renderScale, renderScale, renderScale);

	// Prepare to render
	glPushAttrib(GL_ENABLE_BIT | GL_DEPTH_BUFFER_BIT);
	glDepthMask(GL_FALSE);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	// Render
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	ShaderManager::UseShaderProgram(SUN);
	static const GLint uniformStarTex = glGetUniformLocation(
			ShaderManager::GetShaderProgramHandle(SUN), "starTex");
	glUniform1i(uniformStarTex, 0);
	glBegin(GL_TRIANGLE_STRIP);
	glTexCoord2f(0, 0);
	glVertex2i(-1, -1);
	glTexCoord2f(1, 0);
	glVertex2i(1, -1);
	glTexCoord2f(0, 1);
	glVertex2i(-1, 1);
	glTexCoord2f(1, 1);
	glVertex2i(1, 1);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);

	// Restore render state
	glPopAttrib();
	glPopMatrix();
}
