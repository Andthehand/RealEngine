#pragma once

#include <string>
#include <unordered_set>

namespace RealEngine {
	namespace FileExtenstion {
		template<typename T>
		static inline bool DoesExtensionExist(T& map, std::string& str) {
			return map.find(str) != map.end();
		}

		static const std::unordered_set<std::string> STBI_IMAGE_EXTENSTIONS {
			".jpg", ".jpeg", ".jfif", ".pjpeg", ".pjp",
			".png",								
			".tga",										//Raster graphics file format
			".bmp",										//Bitmap file
			".psd",										//Native file format of Adobe Photoshop
			".gif",
			".hdr",										//High dynamic range
			"pic"										//PICtor raster image
		};

		static const std::unordered_set<std::string> REALENGINE_SCENE_EXTENTIONS {
			".scene"
		};

		static const std::unordered_set<std::string> REALENGINE_PROJECT_EXTENTIONS{
			".realProj"
		};

		static const std::unordered_set<std::string> REALENGINE_SHADER_PANEL_EXTENTIONS{
			".shaderpanel"
		};
	}
}
