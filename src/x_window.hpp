#include <X11/Xlib.h>
#include <cstdint>

class XWindow
{
public:
	XWindow();

	~XWindow();

	void Init();

	void Loop();

private:
	Display* m_display;

	Window m_window;

	GC m_context;

	uint16_t m_x;
	uint16_t m_y;
	
	uint16_t m_width;
	uint16_t m_height;

};
