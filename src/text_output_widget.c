#include <assert.h>

#include <Windows.h>

int register_class()
{
    HINSTANCE hInstance;
    HBRUSH transparent_brush;
    HBRUSH black_brush;
    WNDCLASSA win_class;
    ATOM black_class;
    ATOM transparent_class;

    hInstance = GetModuleHandle(NULL);
    if (hInstance == NULL)
    {
        return -1;

    }

    transparent_brush = GetStockObject(NULL_BRUSH);
    if (transparent_brush == NULL)
    {
        return -1;
    }

    black_brush = CreateSolidBrush(RGB(0, 0, 0));
    if (black_brush == NULL)
    {
        assert(DeleteObject(transparent_brush) != 0);
        return -1;
    }

    win_class = (WNDCLASSA) {
            .style = CS_GLOBALCLASS | CS_HREDRAW | CS_SAVEBITS | CS_VREDRAW,
            .lpfnWndProc = NULL,
            .cbClsExtra = 0,
            .cbWndExtra = 0,
            .hInstance = hInstance,
            .hIcon = NULL,
            .hCursor = NULL,
            .hbrBackground = black_brush,
            .lpszMenuName = NULL,
            .lpszClassName = "COMiC::TerminalWindowBlack"
    };

    black_class = RegisterClassA(&win_class);
    if (black_class == 0)
    {
        assert(DeleteObject(transparent_brush) != 0);
        assert(DeleteObject(black_brush) != 0);
        return -1;
    }

    win_class.hbrBackground = transparent_brush;
    win_class.lpszClassName = "COMiC::TerminalWindowTransparent";

    transparent_class = RegisterClassA(&win_class);
    if (transparent_class == 0)
    {
        assert(DeleteObject(transparent_brush) != 0);
        assert(DeleteObject(black_brush) != 0);
        UnregisterClassA(black_class, hInstance);
        return -1;
    }

    return 0;
}