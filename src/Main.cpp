#include <stdio.h>
#include "Core.h"
#include "Test0.h"
#include "Test1.h"
#include "Test2.h"
#include "Test3.h"
#include "Test4.h"
#include "Test5.h"
#include "Test6.h"
//#include "Test7.h"
#include "Test8.h"
#include "Test9.h"

#include "Test_UgliGame.h"


BaseGameState* game = NULL;

class Main : public entry::AppI
{
	uint32_t m_debug;
	uint32_t m_reset;

public:
	Main(const char* _name, const char* _description) : entry::AppI(_name, _description) { }
	void init(int32_t _argc, const char* const* _argv, uint32_t _width, uint32_t _height) override
	{
		Args args(_argc, _argv);
		Log("QEng v" + VERSION + "\n");

		screenWidth = _width;
		screenHeight = _height;
		m_debug = BGFX_DEBUG_TEXT; // BGFX_DEBUG_NONE  BGFX_DEBUG_*
		m_reset = BGFX_RESET_VSYNC;

		bgfx::Init init;
		//init.type = bgfx::RendererType::OpenGL;
		init.type = args.m_type;
		init.vendorId = args.m_pciId;
		init.resolution.width = _width;
		init.resolution.height = _height;
		init.resolution.reset = m_reset;
		bgfx::init(init);

		// Enable debug text.
		//bgfx::setDebug(m_debug);

		// Set view 0 clear state.
		bgfx::setViewClear(RENDER3D_VIEW_ID, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);

#if 0
		game = new Test1();

#else
		// Test chooser:
		int test;
		Log("Choose test (1-xx) :: ");
		fscanf(stdin, "%d", &test);
		switch (test)
		{
		case 1: game = new Test1(); break; // models to scene tree (untextured, textured, no material)
		case 2: game = new Test2(); break; // render loops in Update() (models not in scene tree)
		case 3: game = new Test3(); break; // keyboard test
		case 4: game = new Test4(); break; // instancing (one way, check Test9 for other way)
		case 5: game = new Test5(); break; // scenegraph
		case 6: game = new Test6(); break; // multilight, normalmapping
		case 7: game = new Test::Test(); break; //
		case 8: game = new Test8(); break; // statemanager, changing scene
		case 9: game = new Test9(); break; // all shaders test: tex, tex_lit, bump, and those with and without instancing
		
		case 10: game = new UgliGame::Test(); break;
	}
#endif

		game->Init();
}

	int shutdown() override
	{
		bgfx::shutdown();
		return 0;
	}

	bool update() override
	{
		if (!entry::processEvents(screenWidth, screenHeight, m_debug, m_reset, &mouseState))
		{
			if (inputGetKeyState(entry::Key::Esc))
			{
				Log("ESC pressed. Exiting...");
				game->Dispose();
				delete game;

				DisposeAll();
				return false;
			}

			// Update frame timer
			int64_t now = bx::getHPCounter();
			static int64_t last = now;
			const int64_t frameTime = now - last;
			last = now;
			const double freq = double(bx::getHPFrequency());
			const float deltatime = float(frameTime / freq);
			deltaTime = deltatime;

			bgfx::touch(RENDER3D_VIEW_ID); // This dummy draw call is here to make sure that view 0 is cleared if no other draw calls are submitted to view 0.

			game->Update(deltatime);

			//printf("%d %d\n", screenWidth, screenHeight); //debug
			//void bgfx::reset(uint32_t _width, uint32_t _height, uint32_t _flags = BGFX_RESET_NONE, TextureFormat::Enum_format = TextureFormat::Count)
			bgfx::setViewRect(RENDER3D_VIEW_ID, 0, 0, screenWidth, screenHeight);

			bgfx::frame(); // Advance to next frame. Rendering thread will be kicked to process submitted rendering primitives.
			return true;
		}
		return false;
	}
};

ENTRY_IMPLEMENT_MAIN(Main, "Test", "");
