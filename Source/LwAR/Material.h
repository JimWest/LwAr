#pragma once

#include<opencv2\opencv.hpp>

namespace lwar
{
	enum ShaderType {
		Unlit,
		Standard
	};

	enum DrawMode
	{
		Points, Lines, Triangles, Quads
	};

	class Material
	{
	public:
		Material() : Material(ShaderType::Standard) {}
		Material(ShaderType shaderType, cv::Mat texture = cv::Mat());
		~Material();
		static cv::Mat GetDefaultTexture();
		static cv::Mat ColorGradient();

		cv::Mat texture;
		ShaderType shaderType;
		DrawMode drawMode = DrawMode::Triangles;
	};
}
