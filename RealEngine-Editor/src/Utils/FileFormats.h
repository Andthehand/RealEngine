#pragma once

#include <string>
#include <unordered_set>

namespace RealEngine {
	namespace FileExtenstion {
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
	}
}
