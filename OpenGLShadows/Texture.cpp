/*
File Name: texture.cpp
Copyright ? 2016, 2019
Author: David Erbelding, Niko Procopi
Written under the supervision of David I. Schwartz, Ph.D., and
supported by a professional development seed grant from the B. Thomas
Golisano College of Computing & Information Sciences
(https://www.rit.edu/gccis) at the Rochester Institute of Technology.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at
your option) any later version.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "Texture.h"

// use the same sampler for every texture
GLuint sampler;

Texture::Texture(char* filePath)
{
	// Load the file.
	FIBITMAP* bitmap = FreeImage_Load(FreeImage_GetFileType(filePath), filePath);
	// Convert the file to 32 bits so we can use it.
	FIBITMAP* bitmap32 = FreeImage_ConvertTo32Bits(bitmap);

	// Create an OpenGL texture.
	glGenTextures(1, &m_texture);

	// build the sampler if it does not exist
	if (sampler == 0)
		glGenSamplers(1, &sampler);

	// Bind our texture.
	glBindTexture(GL_TEXTURE_2D, m_texture);

	// Bind our sampler.
	glBindSampler(m_texture, sampler);

	// Fill our openGL side texture object.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, FreeImage_GetWidth(bitmap32), FreeImage_GetHeight(bitmap32),
		0, GL_BGRA, GL_UNSIGNED_BYTE, static_cast<void*>(FreeImage_GetBits(bitmap32)));

	// Set texture sampling parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	// Get the maximum anisotropy that our GPU supports
	GLfloat maxAniso = 0.0f;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAniso);

	// set the sampler parameters
	glSamplerParameteri(sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glSamplerParameterf(sampler, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAniso);

	// This creates the mipmap layers
	glGenerateMipmap(GL_TEXTURE_2D);

	// Unbind the texture.
	glBindTexture(GL_TEXTURE_2D, 0);

	// We can unload the images now that the texture data has been buffered with opengl
	FreeImage_Unload(bitmap);
	FreeImage_Unload(bitmap32);
}


Texture::~Texture()
{
	glDeleteTextures(1, &m_texture);
}

void Texture::IncRefCount()
{
	m_refCount++;
}

void Texture::DecRefCount()
{
	m_refCount--;
	if (m_refCount == 0)
	{
		delete this;
	}
}

GLuint Texture::GetGLTexture()
{
	return m_texture;
}
