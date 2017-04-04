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

			XDrawString(
					m_display,
					m_window,
					m_context,
					20,
					20,
					&line.text[0],
					line.text.size()
				);	
		}

		if (event.type == KeyPress)
		{
			KeySym sym = XkbKeycodeToKeysym(
					m_display,
					event.xkey.keycode,
					0,
					event.xkey.state & ShiftMask ? 1 : 0
				);
			
			if (sym == XK_space)
			{
				m_buffer->Insert(0, m_column, ' ');
			}
			else
			{
				char* key = XKeysymToString(sym);

				assert(key && "Could not get string from KeySym!");

				m_buffer->Insert(0, m_column, key[0]);
			}

			m_column++;

			XExposeEvent expose_evt = {0};

			expose_evt.type    = Expose;
			expose_evt.display = m_display;
		   	expose_evt.window  = m_window;
			expose_evt.width   = m_width;
			expose_evt.height  = m_height;

			XEvent evt = {0};

			evt.type    = Expose;
			evt.xexpose = expose_evt;

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
