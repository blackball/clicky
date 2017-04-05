#include "x_window.hpp"

#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/XKBlib.h>
#include <X11/keysym.h>

#include <cstdio>
#include <cstring>

#include "line_buffer.hpp"

XWindow::XWindow()
	:
	m_width(1024),
	m_height(796),
	m_buffer(new LineBuffer())
{
}

XWindow::~XWindow()
{
	XftDrawDestroy(m_draw);
	XftColorFree(
		m_display,
		DefaultVisual(m_display, DefaultScreen(m_display)),
		DefaultColormap(m_display, DefaultScreen(m_display)),
		&m_fontColor
	);

	XFreeGC(m_display, m_context);
	XDestroyWindow(m_display, m_window);
	XCloseDisplay(m_display);

	delete m_buffer;
}

void XWindow::Init()
{
	m_display = XOpenDisplay(nullptr);

	if (!m_display)
		printf("Failed to open the X11 Display!\n");

	m_font = XftFontOpenName(
		m_display,
		DefaultScreen(m_display),
		"monospace"
	);

	int black = BlackPixel(m_display, DefaultScreen(m_display));
	int white = WhitePixel(m_display, DefaultScreen(m_display));

	// Create a window at the coordinates specified
	// 
	// The rogue zero is the border size, which is
	// normally determined by the DWM
	m_window = XCreateSimpleWindow(
		m_display,
		DefaultRootWindow(m_display),
		m_x,
		m_y,
		m_width,
		m_height,
		0,
		black,
		black
	);
	
	// Set the name and other metadata of the window
	XSetStandardProperties(
		m_display,
		m_window,
		"clicky",
		"clicky",
		None,
		nullptr,
		0,
		nullptr
	);

	XSelectInput(
		m_display,
		m_window,
		ExposureMask | KeyPressMask
	);

	// Display window
	XMapWindow(m_display, m_window);	

	// Handle window closing and destruction with X11
	Atom WM_DELETE_WINDOW = XInternAtom(
		m_display,
		"WM_DELETE_WINDOW",
		False
	);

	XSetWMProtocols(
		m_display,
		m_window,
		&WM_DELETE_WINDOW,
		1
	);

	m_context = XCreateGC(
		m_display,
		m_window,
		0,
		nullptr
	);

	m_draw = XftDrawCreate(
		m_display,
		m_window,
		DefaultVisual(m_display, DefaultScreen(m_display)),
		DefaultColormap(m_display, DefaultScreen(m_display))	
	);

	m_fontRenderColor.red   = 0xFFFF;
	m_fontRenderColor.green = 0xFFFF;
	m_fontRenderColor.blue  = 0xFFFF;
	m_fontRenderColor.alpha = 0xFFFF;

	XftColorAllocValue(
		m_display,
		DefaultVisual(m_display, DefaultScreen(m_display)),
		DefaultColormap(m_display, DefaultScreen(m_display)),
		&m_fontRenderColor,
		&m_fontColor
	);

	XSetForeground(
		m_display,
		m_context,
		white
	);		

	XFlush(m_display);
}

void XWindow::Loop()
{
	XEvent event;

	while (true)
	{
		XNextEvent(m_display, &event);

		if (event.type == Expose)
		{
			Line line = m_buffer->GrabLine(0);

			XftDrawString8(
				m_draw,
				&m_fontColor,
				m_font,
				20,
				20,
				reinterpret_cast<XftChar8*>(&line.text[0]),
				line.text.size()
			);	

			XFillRectangle(
				m_display,
				m_window,
				m_context,
				20 + (m_column * 2),
				20,
				4,
				2
			);
		}

		if (event.type == KeyPress)
		{
			size_t length         = 20;
			char   buffer[length] = {'\0'};
			KeySym sym;

			length = XLookupString(
				&event.xkey,
				buffer,
				length,
				&sym,
				nullptr
			);

			buffer[length] = '\0';

			if (sym == XK_BackSpace)
			{
				m_column = m_buffer->Delete(
					0,
					m_column,
					1
				);
			}
			else
			{
				m_buffer->Insert(
					0,
					m_column,
					buffer,
					length
				);

				m_column += length;
			}

			XExposeEvent expose_evt = {0};

			expose_evt.type    = Expose;
			expose_evt.display = m_display;
		   	expose_evt.window  = m_window;
			expose_evt.width   = m_width;
			expose_evt.height  = m_height;

			XEvent evt = {0};
		
			evt.type    = Expose;
			evt.xexpose = expose_evt;

			XClearWindow(m_display, m_window);

			XSendEvent(
				m_display,
				m_window,
				False,
				ExposureMask,
				&evt		
			);
		}

		if (event.type == ClientMessage)
		{
			break;
		}
	}
}
