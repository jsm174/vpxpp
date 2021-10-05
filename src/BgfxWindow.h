#pragma once

#include <SDL2/SDL.h>

#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <bimg/bimg.h>
#include <bx/debug.h>
#include <bx/file.h>
#include <bx/string.h>

class BgfxWindow : public bgfx::CallbackI
{
public:
	BgfxWindow();
	virtual ~BgfxWindow();

	virtual void PreCreate();

	bool Create(int width, int height);
	void Run();

	virtual void PreRender();
	virtual void RenderImgGui();
	virtual void Render() = 0;
	virtual void PostRender();

	virtual void fatal(const char* _filePath, uint16_t _line, bgfx::Fatal::Enum _code, const char* _str) override;
	virtual void traceVargs(const char* _filePath, uint16_t _line, const char* _format, va_list _argList) override;
	virtual void profilerBegin(const char* /*_name*/, uint32_t /*_abgr*/, const char* /*_filePath*/, uint16_t /*_line*/) override;
	virtual void profilerBeginLiteral(const char* /*_name*/, uint32_t /*_abgr*/, const char* /*_filePath*/, uint16_t /*_line*/) override;
	virtual void profilerEnd() override;
	virtual uint32_t cacheReadSize(uint64_t /*_id*/) override;
	virtual bool cacheRead(uint64_t /*_id*/, void* /*_data*/, uint32_t /*_size*/) override;
	virtual void cacheWrite(uint64_t /*_id*/, const void* /*_data*/, uint32_t /*_size*/) override;
	virtual void screenShot(const char* _filePath, uint32_t _width, uint32_t _height, uint32_t _pitch, const void* _data, uint32_t _size, bool _yflip) override;
	virtual void captureBegin(uint32_t /*_width*/, uint32_t /*_height*/, uint32_t /*_pitch*/, bgfx::TextureFormat::Enum /*_format*/, bool /*_yflip*/) override;
	virtual void captureEnd() override;
	virtual void captureFrame(const void* /*_data*/, uint32_t /*_size*/) override;

private:
	virtual void OnInitialUpdate() = 0;

	void ShowStatsWindow();
	void Destroy();

	int m_width;
	int m_height;

	SDL_Window* m_pWindow;
};