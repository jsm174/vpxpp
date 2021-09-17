#include "BgfxWindow.h"

#include <SDL2/SDL_syswm.h>

#include "imgui_impl_bgfx.h"
#include "imgui_impl_sdl.h"

struct SampleData
{
	static constexpr uint32_t kNumSamples = 100;

	SampleData()
	{
		reset();
	}

	void reset()
	{
		m_offset = 0;
		bx::memSet(m_values, 0, sizeof(m_values));

		m_min = 0.0f;
		m_max = 0.0f;
		m_avg = 0.0f;
	}

	void pushSample(float value)
	{
		m_values[m_offset] = value;
		m_offset = (m_offset + 1) % kNumSamples;

		float min = bx::kFloatMax;
		float max = -bx::kFloatMax;
		float avg = 0.0f;

		for (uint32_t ii = 0; ii < kNumSamples; ++ii)
		{
			const float val = m_values[ii];
			min = bx::min(min, val);
			max = bx::max(max, val);
			avg += val;
		}

		m_min = min;
		m_max = max;
		m_avg = avg / kNumSamples;
	}

	int32_t m_offset;
	float m_values[kNumSamples];

	float m_min;
	float m_max;
	float m_avg;
};

BgfxWindow::BgfxWindow()
{
}

BgfxWindow::~BgfxWindow()
{
}

bool BgfxWindow::Create(int width, int height)
{
	m_width = width;
	m_height = height;

	SDL_Init(SDL_INIT_VIDEO);

	m_pWindow = SDL_CreateWindow("bgfx",
								 SDL_WINDOWPOS_CENTERED,
								 SDL_WINDOWPOS_CENTERED,
								 m_width,
								 m_height,
								 SDL_WINDOW_SHOWN);

	if (m_pWindow == nullptr)
	{
		return false;
	}

	SDL_SysWMinfo wmi;
	SDL_VERSION(&wmi.version);

	if (!SDL_GetWindowWMInfo(m_pWindow, &wmi))
	{
		SDL_DestroyWindow(m_pWindow);
		m_pWindow = nullptr;

		return false;
	}

	bgfx::renderFrame();

	bgfx::PlatformData platformData{};

#if BX_PLATFORM_WINDOWS
	platformData.nwh = wmi.info.win.window;
#elif BX_PLATFORM_OSX
	platformData.nwh = wmi.info.cocoa.window;
#elif BX_PLATFORM_LINUX
	platformData.ndt = wmi.info.x11.display;
	platformData.nwh = (void*)(uintptr_t)wmi.info.x11.window;
#endif

	bgfx::Init init;
	init.type = bgfx::RendererType::Count;
	init.resolution.width = m_width;
	init.resolution.height = m_height;
	init.resolution.reset = BGFX_RESET_VSYNC;
	init.platformData = platformData;
	init.callback = this;

	if (!bgfx::init(init))
	{
		SDL_DestroyWindow(m_pWindow);
		m_pWindow = nullptr;

		return false;
	}

	bgfx::setDebug(BGFX_DEBUG_TEXT | BGFX_DEBUG_STATS);

	bgfx::setViewClear(
		0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x6495EDFF, 1.0f, 0);
	bgfx::setViewRect(0, 0, 0, width, height);

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	ImGui_Implbgfx_Init(0);

	switch (bgfx::getRendererType())
	{
	case bgfx::RendererType::Vulkan:
		ImGui_ImplSDL2_InitForVulkan(m_pWindow);
		break;
	case bgfx::RendererType::Metal:
		ImGui_ImplSDL2_InitForMetal(m_pWindow);
		break;
	default:
		SDL_DestroyWindow(m_pWindow);
		m_pWindow = nullptr;

		return false;
	}

	OnInitialUpdate();

	return true;
}

void BgfxWindow::Run()
{
	if (m_pWindow == nullptr)
	{
		return;
	}

	for (bool quit = false; !quit;)
	{
		SDL_Event currentEvent;
		while (SDL_PollEvent(&currentEvent) != 0)
		{
			ImGui_ImplSDL2_ProcessEvent(&currentEvent);
			if (currentEvent.type == SDL_QUIT)
			{
				quit = true;
				break;
			}
		}

		Render();
	}

	Destroy();
}

void BgfxWindow::OnInitialUpdate()
{
}

void BgfxWindow::Render()
{
	ImGui_Implbgfx_NewFrame();
	ImGui_ImplSDL2_NewFrame(m_pWindow);

	ImGui::NewFrame();
	ShowStatsWindow();
	ImGui::Render();

	ImGui_Implbgfx_RenderDrawLists(ImGui::GetDrawData());

	bgfx::touch(0);

	bgfx::frame();
}

void BgfxWindow::ShowStatsWindow()
{
	ImGui::ShowMetricsWindow();

	static SampleData s_frameTime;
	ImGui::Begin("Stats Window window");

	const bgfx::Stats* stats = bgfx::getStats();
	const double toMsCpu = 1000.0 / stats->cpuTimerFreq;
	const double toMsGpu = 1000.0 / stats->gpuTimerFreq;
	const double frameMs = double(stats->cpuTimeFrame) * toMsCpu;

	s_frameTime.pushSample(float(frameMs));

	char frameTextOverlay[256];
	bx::snprintf(frameTextOverlay, BX_COUNTOF(frameTextOverlay), "%s%.3fms, %s%.3fms\nAvg: %.3fms, %.1f FPS", "Up ", s_frameTime.m_min, "Dn ", s_frameTime.m_max, s_frameTime.m_avg, 1000.0f / s_frameTime.m_avg);

	ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImColor(0.0f, 0.5f, 0.15f, 1.0f).Value);
	ImGui::PlotHistogram("Frame", s_frameTime.m_values, SampleData::kNumSamples, s_frameTime.m_offset, frameTextOverlay, 0.0f, 60.0f, ImVec2(0.0f, 45.0f));
	ImGui::PopStyleColor();

	ImGui::Text("Submit CPU %0.3f, GPU %0.3f (L: %d)", double(stats->cpuTimeEnd - stats->cpuTimeBegin) * toMsCpu, double(stats->gpuTimeEnd - stats->gpuTimeBegin) * toMsGpu, stats->maxGpuLatency);

	if (-INT64_MAX != stats->gpuMemoryUsed)
	{
		char tmp0[64];
		bx::prettify(tmp0, BX_COUNTOF(tmp0), stats->gpuMemoryUsed);

		char tmp1[64];
		bx::prettify(tmp1, BX_COUNTOF(tmp1), stats->gpuMemoryMax);

		ImGui::Text("GPU mem: %s / %s", tmp0, tmp1);
	}

	ImGui::End();
}

void BgfxWindow::Destroy()
{
	if (m_pWindow == nullptr)
	{
		return;
	}

	ImGui_ImplSDL2_Shutdown();
	ImGui_Implbgfx_Shutdown();

	ImGui::DestroyContext();

	bgfx::shutdown();

	SDL_DestroyWindow(m_pWindow);
	m_pWindow = nullptr;

	SDL_Quit();
}

void BgfxWindow::fatal(const char* _filePath, uint16_t _line, bgfx::Fatal::Enum _code, const char* _str)
{
	bx::debugPrintf("BgfxWindow::fatal - Fatal error: 0x%08x: %s", _code, _str);

	if (bgfx::Fatal::DebugCheck == _code)
	{
		bx::debugBreak();
	}
	else
	{
		if (m_pWindow) {
			SDL_DestroyWindow(m_pWindow);
			m_pWindow = nullptr;
		}
	}
}

void BgfxWindow::traceVargs(const char* _filePath, uint16_t _line, const char* _format, va_list _argList)
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

void BgfxWindow::profilerBegin(const char* /*_name*/, uint32_t /*_abgr*/, const char* /*_filePath*/, uint16_t /*_line*/)
{
}

void BgfxWindow::profilerBeginLiteral(const char* /*_name*/, uint32_t /*_abgr*/, const char* /*_filePath*/, uint16_t /*_line*/)
{
}

void BgfxWindow::profilerEnd()
{
}

uint32_t BgfxWindow::cacheReadSize(uint64_t /*_id*/)
{
	return 0;
}

bool BgfxWindow::cacheRead(uint64_t /*_id*/, void* /*_data*/, uint32_t /*_size*/)
{
	return false;
}

void BgfxWindow::cacheWrite(uint64_t /*_id*/, const void* /*_data*/, uint32_t /*_size*/)
{
}

void BgfxWindow::screenShot(const char* _filePath, uint32_t _width, uint32_t _height, uint32_t _pitch, const void* _data, uint32_t _size, bool _yflip)
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

void BgfxWindow::captureBegin(uint32_t /*_width*/, uint32_t /*_height*/, uint32_t /*_pitch*/, bgfx::TextureFormat::Enum /*_format*/, bool /*_yflip*/)
{
	BX_TRACE("Warning: using capture without callback (a.k.a. pointless).");
}

void BgfxWindow::captureEnd()
{
}

void BgfxWindow::captureFrame(const void* /*_data*/, uint32_t /*_size*/)
{
}