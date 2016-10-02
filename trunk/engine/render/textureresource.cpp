#include "config.h"
#include "textureresource.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "rasterizer.h"

namespace Render
{

TextureResource::TextureResource()
{

}

TextureResource::~TextureResource()
{
	glDeleteTextures(1, &texture);
}

void TextureResource::setShader(shared_ptr<ShaderObject> inShader)
{
	this->shader = inShader;
}

void TextureResource::setRasterizer(shared_ptr<Rasterizer> inRaster)
{
	this->raster = inRaster;
}

void TextureResource::loadFromFile(const char * filename)
{

	int w, h, n; //Width, Height, components per pixel (ex. RGB = 3, RGBA = 4)
	unsigned char *image = stbi_load(filename, &w, &h, &n, 0);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// If there's no alpha channel, use RGB colors. else: use RGBA.
	if (n == 3){
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	}
	else if (n == 4){
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	}

	glGenerateMipmap(GL_TEXTURE_2D);
	

	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(image);

}

void TextureResource::loadFromRasterizer()
{
	int w = raster->screenwidth;
	int h = raster->screenheight;

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, &raster->draw()[0]);
	
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void TextureResource::bindTexture(GLuint slot)
{
	this->shader->applyProgram();

	this->shader->setUniVector3fv(this->matAmbientReflectance, "u_matAmbientReflectance");
	this->shader->setUniVector3fv(this->matDiffuseReflectance, "u_matDiffuseReflectance");
	this->shader->setUniVector3fv(this->matSpecularReflectance, "u_matSpecularReflectance");

	this->shader->setUni1f(this->matShininess, "u_matShininess");

	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, this->texture);


}

void TextureResource::setAmbientReflectance(GLfloat r, GLfloat g, GLfloat b)
{
	matAmbientReflectance.set(r, g, b);
}

void TextureResource::setDiffuseReflectance(GLfloat r, GLfloat g, GLfloat b)
{
	matDiffuseReflectance.set(r, g, b);
}

void TextureResource::setSpecularReflectance(GLfloat r, GLfloat g, GLfloat b)
{
	matSpecularReflectance.set(r, g, b);
}

void TextureResource::setShininess(GLfloat value)
{
	matShininess = value;
}

}