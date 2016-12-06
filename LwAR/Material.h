#pragma once

#include<opencv2\opencv.hpp>

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
	cv::Mat GetDefaultTexture();

	cv::Mat texture;
	ShaderType shaderType;
};

