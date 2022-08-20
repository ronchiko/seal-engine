#include <seal/cpp/window.hpp>

#include <seal/cpp/error.hpp>

using namespace seal;

window::instance _create_window_instance(
	size_t w, 
	size_t h, 
	const std::string& title, 
	std::optional<window::options> options
) {
	auto *window = Seal_CreateWindow(
		w, 
		h, 
		title.c_str(), 
		options.has_value() ? &options.value() : nullptr);

	if(!window) {
		throw seal::error("Failed to create window");
	}

	return {window, Seal_DestroyWindow};
}

window::window(size_t w, size_t h, const std::string& title, std::optional<options> options) 
	: m_window(_create_window_instance(w, h, title, options))
{
}

bool window::is_fullscreen() const {
	return Seal_GetWindowFullscreen(m_window.get());
}

void window::toggle_fullscreen(bool active) {
	Seal_SetWindowFullscreen(m_window.get(), static_cast<Seal_Bool>(active));
}

void window::resume() {
	Seal_WindowContinue(m_window.get());
}

window::stats window::get_stats() const {
	stats stats{};

	Seal_GetWindowStats(m_window.get(), &stats.width, &stats.height, &stats.ratio);

	return stats;
}