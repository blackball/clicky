#include <X11/Xlib.h>
#include <cstdint>
#include <X11/Xft/Xft.h>

class LineBuffer;

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

	XftFont* m_font;
	XftDraw* m_draw;

	XRenderColor m_fontRenderColor;
	XftColor     m_fontColor;

	uint16_t m_x;
	uint16_t m_y;
	
	uint16_t m_width;
	uint16_t m_height;

	LineBuffer* m_buffer = nullptr;

	uint16_t m_column = 0;

};
