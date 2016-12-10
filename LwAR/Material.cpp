#include "Material.h"


namespace lwar
{
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


	cv::Mat Material::ColorGradient()
	{
		int taille = 512;
		cv::Mat image(taille, taille, CV_8UC3);
		for (int y = 0; y < taille; y++) {
			cv::Vec3b val;
			val[0] = 0; val[1] = (y * 255) / taille; val[2] = (taille - y) * 255 / taille;
			for (int x = 0; x < taille; x++)
				image.at<cv::Vec3b>(y, x) = val;
		}
		return image;
	}
}