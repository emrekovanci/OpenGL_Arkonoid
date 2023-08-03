#include "Effects/PostProcessor.hpp"

#include <iostream>

PostProcessor::PostProcessor(const Shader& shader, unsigned int width, unsigned int height) :
	_shader { shader },
	_width { width },
	_height { height },
	_fboTexture { }
{
	glGenFramebuffers(1, &_msfbo);
	glGenFramebuffers(1, &_fbo);
	glGenRenderbuffers(1, &_rbo);

	glBindFramebuffer(GL_FRAMEBUFFER, _msfbo);
	glBindRenderbuffer(GL_RENDERBUFFER, _rbo);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_RGB, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, _rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::POSTPROCESSOR: Failed to initialize MSFBO!\n";
	}

	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
	_fboTexture.Generate(width, height, nullptr);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _fboTexture.Id, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::POSTPROCESSOR: Failed to initialize FBO!\n";
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	initRenderData();

	_shader.Use();
	_shader.SetInt("scene", 0);

	const float offset = 1.0f / 300.0f;
	const float offsets[9][2] =
	{
	   { -offset,  offset  },  // top-left
	   {  0.0f,    offset  },  // top-center
	   {  offset,  offset  },  // top-right
	   { -offset,  0.0f    },  // center-left
	   {  0.0f,    0.0f    },  // center-center
	   {  offset,  0.0f    },  // center - right
	   { -offset, -offset  },  // bottom-left
	   {  0.0f,   -offset  },  // bottom-center
	   {  offset, -offset  }   // bottom-right
	};
	glUniform2fv(glGetUniformLocation(_shader.GetProgramId(), "offsets"), 9, reinterpret_cast<const float*>(offsets));

	const int edge_kernel[9] =
	{
		-1, -1, -1,
		-1,  8, -1,
		-1, -1, -1
	};
	glUniform1iv(glGetUniformLocation(_shader.GetProgramId(), "edge_kernel"), 9, edge_kernel);

	const float blur_kernel[9] =
	{
		1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f,
		2.0f / 16.0f, 4.0f / 16.0f, 2.0f / 16.0f,
		1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f
	};
	glUniform1fv(glGetUniformLocation(_shader.GetProgramId(), "blur_kernel"), 9, blur_kernel);
}

void PostProcessor::BeginRender()
{
	glBindFramebuffer(GL_FRAMEBUFFER, _msfbo);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void PostProcessor::EndRender()
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, _msfbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _fbo);
	glBlitFramebuffer(0, 0, _width, _height, 0, 0, _width, _height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PostProcessor::Render(float time)
{
	_shader.Use();
	_shader.SetFloat("time", time);
	_shader.SetBool("confuse", Confuse);
	_shader.SetBool("chaos", Chaos);
	_shader.SetBool("shake", Shake);

	glActiveTexture(GL_TEXTURE0);
	_fboTexture.Bind();
	glBindVertexArray(_vao);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void PostProcessor::initRenderData()
{
	unsigned int vbo;

	float vertices[] =
	{
		// pos        // tex
		-1.0f, -1.0f, 0.0f, 0.0f,
		 1.0f,  1.0f, 1.0f, 1.0f,
		-1.0f,  1.0f, 0.0f, 1.0f,

		-1.0f, -1.0f, 0.0f, 0.0f,
		 1.0f, -1.0f, 1.0f, 0.0f,
		 1.0f,  1.0f, 1.0f, 1.0f
	};

	glGenVertexArrays(1, &_vao);
	glGenBuffers(1, &vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(_vao);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
