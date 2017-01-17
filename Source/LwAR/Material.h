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

		// Returns the default Texture which is just a white Image.
		//
		// @return Default white Texture as cv::Mat
		static cv::Mat GetDefaultTexture();

		// Returns the default Texture which is just a white Image.
		//
		// @return Color-Gradiant Image from green to red as cv::Mat
		static cv::Mat ColorGradient();

		cv::Mat texture;
		ShaderType shaderType;
		DrawMode drawMode = DrawMode::Triangles;
	};
}
