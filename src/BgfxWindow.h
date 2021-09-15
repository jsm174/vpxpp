#pragma once

#include <SDL2/SDL.h>

#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <bx/debug.h>
#include <bx/file.h>
#include <bx/string.h>
#include <bimg/bimg.h>

struct BgfxCallback : public bgfx::CallbackI
{
	virtual ~BgfxCallback()
	{
	}

	virtual void fatal(const char* _filePath, uint16_t _line, bgfx::Fatal::Enum _code, const char* _str) override
	{
		bx::debugPrintf("Callback - Fatal error: 0x%08x: %s", _code, _str);

		if (bgfx::Fatal::DebugCheck == _code)
		{
			bx::debugBreak();
		}
		else
		{
			abort();
		}
	}

	virtual void traceVargs(const char* _filePath, uint16_t _line, const char* _format, va_list _argList) override
	{
		char temp[2048];
		char* out = temp;
		va_list argListCopy;
		va_copy(argListCopy, _argList);
		int32_t len = bx::snprintf(out, sizeof(temp), "%s (%d): ", _filePath, _line);
		int32_t total = len + bx::vsnprintf(out + len, sizeof(temp) - len, _format, argListCopy);
		va_end(argListCopy);
		if ((int32_t)sizeof(temp) < total)
		{
			out = (char*)alloca(total + 1);
			bx::memCopy(out, temp, len);
			bx::vsnprintf(out + len, total - len, _format, _argList);
		}
		out[total] = '\0';
		bx::debugOutput(out);
	}

	virtual void profilerBegin(const char* /*_name*/, uint32_t /*_abgr*/, const char* /*_filePath*/, uint16_t /*_line*/) override
	{
	}

	virtual void profilerBeginLiteral(const char* /*_name*/, uint32_t /*_abgr*/, const char* /*_filePath*/, uint16_t /*_line*/) override
	{
	}

	virtual void profilerEnd() override
	{
	}

	virtual uint32_t cacheReadSize(uint64_t /*_id*/) override
	{
		return 0;
	}

	virtual bool cacheRead(uint64_t /*_id*/, void* /*_data*/, uint32_t /*_size*/) override
	{
		return false;
	}

	virtual void cacheWrite(uint64_t /*_id*/, const void* /*_data*/, uint32_t /*_size*/) override
	{
	}

	virtual void screenShot(const char* _filePath, uint32_t _width, uint32_t _height, uint32_t _pitch, const void* _data, uint32_t _size, bool _yflip) override
	{
		BX_UNUSED(_filePath, _width, _height, _pitch, _data, _size, _yflip);

		const int32_t len = bx::strLen(_filePath) + 5;
		char* filePath = (char*)alloca(len);
		bx::strCopy(filePath, len, _filePath);
		bx::strCat(filePath, len, ".tga");

		bx::FileWriter writer;
		if (bx::open(&writer, filePath))
		{
			bimg::imageWriteTga(&writer, _width, _height, _pitch, _data, false, _yflip);
			bx::close(&writer);
		}
	}

	virtual void captureBegin(uint32_t /*_width*/, uint32_t /*_height*/, uint32_t /*_pitch*/, bgfx::TextureFormat::Enum /*_format*/, bool /*_yflip*/) override
	{
		BX_TRACE("Warning: using capture without callback (a.k.a. pointless).");
	}

	virtual void captureEnd() override
	{
	}

	virtual void captureFrame(const void* /*_data*/, uint32_t /*_size*/) override
	{
	}
};

class BgfxWindow
{
public:
	BgfxWindow();
	virtual ~BgfxWindow();

	bool Create(int width, int height);
	void Run();

	virtual void Render();

private:
	void ShowStatsWindow();

	void Destroy();

	int m_width;
	int m_height;

	SDL_Window* m_pWindow;

	BgfxCallback m_callback;
};