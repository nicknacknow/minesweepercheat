#pragma once
#include <Windows.h>
#include <vector>

class ScreenshotAssist {
public:
	ScreenshotAssist() {
		hScreenDC = GetDC(NULL);
		hMemoryDC = CreateCompatibleDC(hScreenDC);
	}
	~ScreenshotAssist()
	{
		DeleteDC(hScreenDC);
		DeleteDC(hMemoryDC);
	}
	HBITMAP Screenshot(int x, int y, int width = 1, int height = 1) {
		HBITMAP hBitmap = CreateCompatibleBitmap(this->hScreenDC, width, height);
		HBITMAP hOldBitmap = static_cast<HBITMAP>(SelectObject(this->hMemoryDC, hBitmap));
		if (!BitBlt(this->hMemoryDC, 0, 0, width, height, this->hScreenDC, x, y, SRCCOPY)) exit(0);
		hBitmap = static_cast<HBITMAP>(SelectObject(this->hMemoryDC, hOldBitmap));

		return hBitmap;
	}
    std::vector<BYTE> ToPixels(HBITMAP BitmapHandle, int width = 1, int height = 1)
    {
        BITMAP Bmp = { 0 };
        BITMAPINFO Info = { 0 };
        std::vector<BYTE> Pixels = std::vector<BYTE>();

        HDC DC = CreateCompatibleDC(NULL);
        std::memset(&Info, 0, sizeof(BITMAPINFO)); //not necessary really..
        HBITMAP OldBitmap = (HBITMAP)SelectObject(DC, BitmapHandle);
        GetObject(BitmapHandle, sizeof(Bmp), &Bmp);

        Info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        Info.bmiHeader.biWidth = width = Bmp.bmWidth;
        Info.bmiHeader.biHeight = height = Bmp.bmHeight;
        Info.bmiHeader.biPlanes = 1;
        Info.bmiHeader.biBitCount = Bmp.bmBitsPixel;
        Info.bmiHeader.biCompression = BI_RGB;
        Info.bmiHeader.biSizeImage = ((width * Bmp.bmBitsPixel + 31) / 32) * 4 * height;

        Pixels.resize(Info.bmiHeader.biSizeImage);
        GetDIBits(DC, BitmapHandle, 0, height, &Pixels[0], &Info, DIB_RGB_COLORS);
        SelectObject(DC, OldBitmap);

        height = std::abs(height);
        DeleteDC(DC);
        return Pixels;
    }
private:
	HDC hScreenDC; 
	HDC hMemoryDC;
};