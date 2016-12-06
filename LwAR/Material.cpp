#include "Material.h"



Material::Material(ShaderType shaderType, cv::Mat texture)
{
	this->shaderType = shaderType;

	if (texture.empty())
		texture = GetDefaultTexture();

	this->texture = texture;
}


Material::~Material()
{
}


cv::Mat Material::GetDefaultTexture()
{
	cv::Mat image(512, 512, CV_8UC3);
	image.setTo(cv::Scalar(255, 255, 255));
	return image;
}
