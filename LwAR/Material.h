#pragma once

#include<opencv2\opencv.hpp>

namespace lwar
{
	enum ShaderType {
		Unlit,
		Standard
	};

	class Material
	{
	public:
		Material() : Material(ShaderType::Unlit) {}
		Material(ShaderType shaderType, cv::Mat texture = cv::Mat());
		~Material();
		static cv::Mat GetDefaultTexture();
		static cv::Mat ColorGradient();

		cv::Mat texture;
		ShaderType shaderType;
	};
}
