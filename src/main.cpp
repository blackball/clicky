#include "x_window.hpp"

int main()
{
	XWindow window;

	window.Init();

	window.Loop();

	return 0;
}
