//-----------------------------------------------
//
//	This file is part of the Siv3D Engine.
//
//	Copyright (c) 2008-2026 Ryo Suzuki
//	Copyright (c) 2016-2026 OpenSiv3D Project
//
//	Licensed under the MIT License.
//
//-----------------------------------------------

# include <GLFW/internal.h>
# include <cassert>

extern "C"
{
	GLFWAPI const char* glfwGetKeysSiv3D(GLFWwindow* handle)
	{
		_GLFWwindow* window = (_GLFWwindow*)handle;
		assert(window != NULL);

		return window->keys;
	}
}
