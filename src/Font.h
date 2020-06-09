/*
// some settings
m_textBufferManager->setBackgroundColor(m_staticText, 0x551111ff);
m_textBufferManager->setUnderlineColor(m_staticText, 0xff2222ff);
m_textBufferManager->setOverlineColor(m_staticText, 0x2222ffff);
m_textBufferManager->setStrikeThroughColor(m_staticText, 0x22ff22ff);
m_textBufferManager->setStyle(m_staticText, [  STYLE_NORMAL	STYLE_STRIKE_THROUGH  STYLE_OVERLINE    STYLE_UNDERLINE  STYLE_BACKGROUND   STYLE_STRIKE_THROUGH  ] );
m_textBufferManager->appendText(m_staticText, m_font, L"Testing öä");

*/
#pragma once
#include "Core.h"

class Font
{
	FontManager* m_fontManager = NULL;
	TextBufferManager* m_textBufferManager = NULL;

	FontHandle m_font;
	TrueTypeHandle m_ttf;

	TextBufferHandle m_transientText;
	TextBufferHandle m_staticText;

public:
	void Init()
	{
		// Init the text rendering system.
		m_fontManager = new FontManager(512);
		m_textBufferManager = new TextBufferManager(m_fontManager);
	}

	void LoadTtf(std::string filePath, int fontSize, bool preloadGlyph = false)
	{
		if (m_fontManager == NULL)
			Init();

		std::string dir = ASSETSDIR + filePath;

		uint32_t size;
		void* data = load(dir.c_str(), &size);

		if (data != NULL)
		{
			m_ttf = m_fontManager->createTtf((uint8_t*)data, size);
			BX_FREE(entry::getAllocator(), data);
		}

		if (data == NULL || !isValid(m_ttf))
		{
			Log("  ERROR: Cannot load " + filePath);
			throw "Cannot load font!";
			return;
		}

		m_font = m_fontManager->createFontByPixelSize(m_ttf, 0, fontSize);

		if (preloadGlyph) // Preload glyphs and blit them to atlas.
			m_fontManager->preloadGlyph(m_font, L"abcdefghijklmnopqrstuvwxyzåäöABCDEFGHIJKLMNOPQRSTUVWXYZÅÄÖ.,!?/*-+= \n");

		//m_fontManager->destroyTtf(m_ttf);


		// Create a transient buffer for real-time data.
		m_transientText = m_textBufferManager->createTextBuffer(FONT_TYPE_ALPHA, BufferType::Transient);

		//create a static text buffer compatible with alpha font
		//a static text buffer content cannot be modified after its first submit.
		m_staticText = m_textBufferManager->createTextBuffer(FONT_TYPE_ALPHA, BufferType::Static);
	}

	void CreateStaticText(float x, float y, wchar_t* txt)
	{
		// The pen position represent the top left of the box of the first line of text.
		m_textBufferManager->setPenPosition(m_staticText, x, y);

		// Add some text to the buffer.
		// The position of the pen is adjusted when there is an endline.
		m_textBufferManager->appendText(m_staticText, m_font, txt);
	}
	void AppendToStaticText(wchar_t* txt)
	{
		m_textBufferManager->appendText(m_staticText, m_font, txt);
	}

	void CreateText(float x, float y, wchar_t* txt)
	{
		m_textBufferManager->clearTextBuffer(m_transientText);
		m_textBufferManager->setPenPosition(m_transientText, x, y);
		m_textBufferManager->appendText(m_transientText, m_font, txt);
	}
	void AppendToText(wchar_t* txt)
	{
		m_textBufferManager->appendText(m_transientText, m_font, txt);
	}

	void Dispose()
	{
		m_fontManager->destroyTtf(m_ttf);
		m_fontManager->destroyFont(m_font);
		m_textBufferManager->destroyTextBuffer(m_staticText);
		m_textBufferManager->destroyTextBuffer(m_transientText);
		delete m_textBufferManager;
		delete m_fontManager;
	}

	// Submit text to ORTHO_VIEW_ID (viewId==1)
	void Submit()
	{
		// Submit the static text.
		m_textBufferManager->submitTextBuffer(m_staticText, ORTHO_VIEW_ID);

		// Submit the debug text.
		m_textBufferManager->submitTextBuffer(m_transientText, ORTHO_VIEW_ID);
	}

	TextRectangle GetTextRectangle(bool staticText)
	{
		if (staticText) return m_textBufferManager->getRectangle(m_staticText);
		return m_textBufferManager->getRectangle(m_transientText);
	}

	void SetPenPos(int x, int y, bool staticText)
	{
		if (staticText) m_textBufferManager->setPenPosition(m_staticText, x, y);
		else m_textBufferManager->setPenPosition(m_transientText, x, y);
	}

	void Center(int x, int y, bool staticText)
	{
		TextRectangle tr;
		if (staticText) tr = m_textBufferManager->getRectangle(m_staticText);
		else tr = m_textBufferManager->getRectangle(m_transientText);
		if (x == -1) x = screenWidth / 2 - tr.width / 2;
		if (y == -1) y = screenHeight / 2 - tr.height / 2;
		SetPenPos(x, y, staticText);
	}

};
