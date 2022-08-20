#pragma once

#ifdef __cplusplus

#include <string>
#include <memory>
#include <optional>

#include <seal/window.h>

namespace seal {

class window {
public:
	using instance = std::unique_ptr<Seal_Window, decltype(&Seal_DestroyWindow)>;

	using options = Seal_WindowOptions;

	struct stats {
		Seal_Size width;
		Seal_Size height;
		Seal_Float ratio;
	};

	window(size_t width, size_t height, const std::string& title, std::optional<options> options);

	window::stats get_stats() const;

	bool is_fullscreen() const;

	/**
	 * @brief Toggles the fullscreen on this window
	 * 
	 * @param active True to activate fullscreen false otherwise
	 */
	void toggle_fullscreen(bool active);

	void swap_buffers();

	void resume();

private:
	instance m_window;
};

}

#endif