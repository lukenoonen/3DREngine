#include <iostream>
#include <fstream>
#include <string>
#include <UTIL.h>
#include <SharedDefs.h>

void FlagCheck( std::fstream &fMaterial, std::string &sInput, unsigned int &uiDrawFlags )
{
	if (sInput == "depth")
	{
		bool bFlagIsOn;
		fMaterial >> bFlagIsOn;

		if (bFlagIsOn)
			uiDrawFlags |= FL_RENDERPASS_DEPTH;
	}
	else if (sInput == "unlit")
	{
		bool bFlagIsOn;
		fMaterial >> bFlagIsOn;

		if (bFlagIsOn)
			uiDrawFlags |= FL_RENDERPASS_UNLIT;
	}
	else if (sInput == "lit")
	{
		bool bFlagIsOn;
		fMaterial >> bFlagIsOn;

		if (bFlagIsOn)
			uiDrawFlags |= FL_RENDERPASS_LIT;
	}
	else if (sInput == "shadow")
	{
		bool bFlagIsOn;
		fMaterial >> bFlagIsOn;

		if (bFlagIsOn)
			uiDrawFlags |= FL_RENDERPASS_SHADOW;
	}
	else if (sInput == "hud")
	{
		bool bFlagIsOn;
		fMaterial >> bFlagIsOn;

		if (bFlagIsOn)
			uiDrawFlags |= FL_RENDERPASS_HUD;
	}
}

int main()
{
	std::cout << "Material (0) or material controller (1)? ";
	unsigned int uiType;
	std::cin >> uiType;

	if (uiType == 0)
	{
		std::cout << "Material path: ";
		std::string sPath;
		std::cin >> sPath;

		std::fstream fMaterial( sPath, std::ios::in );

		std::string sOutput( sPath );
		sOutput = sOutput.substr( 0, sOutput.find_last_of( '.' ) ) + ".3mt";
		std::fstream fOutput( sOutput, std::ios::out | std::ios::binary );

		std::string sShader;
		fMaterial >> sShader;
		ShaderType_t tShaderType = UTIL_ShaderNameToType( sShader.c_str() );

		UTIL_Write( fOutput, &tShaderType, 1, ShaderType_t );

		switch (tShaderType)
		{
		case SHADERTYPE_LIT:
		{
			std::string sInput;

			std::string sDiffuse = "";
			std::string sSpecular = "";
			std::string sNormal = "";
			float flShininess = 16.0f;
			float flTextureScaleX = 1.0f, flTextureScaleY = 1.0f;
			unsigned int uiDrawFlags = 0;

			while (fMaterial >> sInput)
			{
				if (sInput == "diffuse")
					fMaterial >> sDiffuse;
				else if (sInput == "specular")
					fMaterial >> sSpecular;
				else if (sInput == "normal")
					fMaterial >> sNormal;
				else if (sInput == "shininess")
					fMaterial >> flShininess;
				else if (sInput == "textureScale")
				{
					fMaterial >> flTextureScaleX;
					fMaterial >> flTextureScaleY;
				}
				else
					FlagCheck( fMaterial, sInput, uiDrawFlags );
			}

			unsigned int uiSize;

			uiSize = (unsigned int)sDiffuse.size();
			UTIL_Write( fOutput, &uiSize, 1, unsigned int );
			UTIL_Write( fOutput, sDiffuse.c_str(), sDiffuse.size(), char );
			uiSize = (unsigned int)sSpecular.size();
			UTIL_Write( fOutput, &uiSize, 1, unsigned int );
			UTIL_Write( fOutput, sSpecular.c_str(), sSpecular.size(), char );
			uiSize = (unsigned int)sNormal.size();
			UTIL_Write( fOutput, &uiSize, 1, unsigned int );
			UTIL_Write( fOutput, sNormal.c_str(), sNormal.size(), char );
			UTIL_Write( fOutput, &flShininess, 1, float );
			UTIL_Write( fOutput, &flTextureScaleX, 1, float );
			UTIL_Write( fOutput, &flTextureScaleY, 1, float );
			UTIL_Write( fOutput, &uiDrawFlags, 1, unsigned int );
			break;
		}
		case SHADERTYPE_UNLIT:
		{
			std::string sInput;

			std::string sDiffuse = "";
			float flTextureScaleX = 1.0f, flTextureScaleY = 1.0f;
			unsigned int uiDrawFlags = 0;

			while (fMaterial >> sInput)
			{
				if (sInput == "diffuse")
					fMaterial >> sDiffuse;
				else if (sInput == "textureScale")
				{
					fMaterial >> flTextureScaleX;
					fMaterial >> flTextureScaleY;
				}
				else
					FlagCheck( fMaterial, sInput, uiDrawFlags );
			}

			unsigned int uiSize;

			uiSize = (unsigned int)sDiffuse.size();
			UTIL_Write( fOutput, &uiSize, 1, unsigned int );
			UTIL_Write( fOutput, sDiffuse.c_str(), sDiffuse.size(), char );
			UTIL_Write( fOutput, &flTextureScaleX, 1, float );
			UTIL_Write( fOutput, &flTextureScaleY, 1, float );
			UTIL_Write( fOutput, &uiDrawFlags, 1, unsigned int );
			break;
		}
		case SHADERTYPE_SKYBOX:
		{
			std::string sInput;

			std::string sTexture_Right = "";
			std::string sTexture_Left = "";
			std::string sTexture_Top = "";
			std::string sTexture_Bottom = "";
			std::string sTexture_Back = "";
			std::string sTexture_Front = "";
			unsigned int uiDrawFlags = 0;

			while (fMaterial >> sInput)
			{
				if (sInput == "texture_right")
					fMaterial >> sTexture_Right;
				else if (sInput == "texture_left")
					fMaterial >> sTexture_Left;
				else if (sInput == "texture_top")
					fMaterial >> sTexture_Top;
				else if (sInput == "texture_bottom")
					fMaterial >> sTexture_Bottom;
				else if (sInput == "texture_front")
					fMaterial >> sTexture_Front;
				else if (sInput == "texture_back")
					fMaterial >> sTexture_Back;
				else
					FlagCheck( fMaterial, sInput, uiDrawFlags );
			}

			unsigned int uiSize;

			uiSize = (unsigned int)sTexture_Right.size();
			UTIL_Write( fOutput, &uiSize, 1, unsigned int );
			UTIL_Write( fOutput, sTexture_Right.c_str(), sTexture_Right.size(), char );
			uiSize = (unsigned int)sTexture_Left.size();
			UTIL_Write( fOutput, &uiSize, 1, unsigned int );
			UTIL_Write( fOutput, sTexture_Left.c_str(), sTexture_Left.size(), char );
			uiSize = (unsigned int)sTexture_Bottom.size();
			UTIL_Write( fOutput, &uiSize, 1, unsigned int );
			UTIL_Write( fOutput, sTexture_Bottom.c_str(), sTexture_Bottom.size(), char );
			uiSize = (unsigned int)sTexture_Top.size();
			UTIL_Write( fOutput, &uiSize, 1, unsigned int );
			UTIL_Write( fOutput, sTexture_Top.c_str(), sTexture_Top.size(), char );
			uiSize = (unsigned int)sTexture_Front.size();
			UTIL_Write( fOutput, &uiSize, 1, unsigned int );
			UTIL_Write( fOutput, sTexture_Front.c_str(), sTexture_Front.size(), char );
			uiSize = (unsigned int)sTexture_Back.size();
			UTIL_Write( fOutput, &uiSize, 1, unsigned int );
			UTIL_Write( fOutput, sTexture_Back.c_str(), sTexture_Back.size(), char );
			UTIL_Write( fOutput, &uiDrawFlags, 1, unsigned int );
			break;
		}
		case SHADERTYPE_COLORHUD:
		{
			std::string sInput;

			float flColorR = 0.0f, flColorG = 0.0f, flColorB = 0.0f;
			unsigned int uiDrawFlags = 0;

			while (fMaterial >> sInput)
			{
				if (sInput == "color")
				{
					fMaterial >> flColorR;
					fMaterial >> flColorG;
					fMaterial >> flColorB;
				}
				else
					FlagCheck( fMaterial, sInput, uiDrawFlags );
			}

			UTIL_Write( fOutput, &flColorR, 1, float );
			UTIL_Write( fOutput, &flColorG, 1, float );
			UTIL_Write( fOutput, &flColorB, 1, float );
			UTIL_Write( fOutput, &uiDrawFlags, 1, unsigned int );
			break;
		}
		}

		fOutput.close();
	}
	else if (uiType == 1)
	{
		std::cout << "Material controller path: ";
		std::string sPath;
		std::cin >> sPath;

		std::fstream fMaterialController( sPath, std::ios::in );

		std::string sOutput( sPath );
		sOutput = sOutput.substr( 0, sOutput.find_last_of( '.' ) ) + ".3mc";
		std::fstream fOutput( sOutput, std::ios::out | std::ios::binary );

		std::string sInput;
		while (fMaterialController >> sInput)
		{
			unsigned int uiSize = sInput.size();
			UTIL_Write( fOutput, &uiSize, 1, unsigned int );
			UTIL_Write( fOutput, sInput.c_str(), uiSize, char );
		}

		fMaterialController.close();
	}

	return 0;
}