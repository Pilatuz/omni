//////////////////////////////////////////////////////////////////////////
//		This material is provided "as is", with absolutely no warranty
//	expressed or implied. Any use is at your own risk.
//
//		Permission to use or copy this software for any purpose is hereby
//	granted without fee, provided the above notices are retained on all
//	copies. Permission to modify the code and to distribute modified code
//	is granted, provided the above notices are retained, and a notice that
//	the code was modified is included with the above copyright notice.
//
//		http://omni.sourceforge.net
//////////////////////////////////////////////////////////////////////////
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <omni/GL.hpp>

#include <exception>
#include <WindowsX.h>
#include <tchar.h>

#define _USE_MATH_DEFINES
#include <math.h>

#pragma comment(lib, "opengl32.lib")

namespace omni
{
	namespace GL
	{

//////////////////////////////////////////////////////////////////////////
// create rendering context: draw to bitmap
Context::Context(HDC hDC)
	: m_hWnd(0), m_hDC(hDC), m_hRC(0)
{
	PIXELFORMATDESCRIPTOR pfd = { 0 };

	pfd.nSize      = sizeof(pfd);
	pfd.nVersion   = 1;
	pfd.dwFlags    = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_BITMAP;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 24;
	pfd.iLayerType = PFD_MAIN_PLANE;

	int pfd_id = ::ChoosePixelFormat(m_hDC, &pfd);
	if (0 == pfd_id)
		throw std::exception("can't choose GL pixel format!");

	if (!::SetPixelFormat(m_hDC, pfd_id, &pfd))
		throw std::exception("can't set GL pixel format!");

	m_hRC = ::wglCreateContext(m_hDC);
	if (0 == m_hRC)
		throw std::exception("can't create GL context!");
}


//////////////////////////////////////////////////////////////////////////
// create rendering context
Context::Context(HWND hWnd)
	: m_hWnd(hWnd), m_hDC(0), m_hRC(0)
{
	PIXELFORMATDESCRIPTOR pfd = { 0 };

	pfd.nSize      = sizeof(pfd);
	pfd.nVersion   = 1;
	pfd.dwFlags    = PFD_GENERIC_ACCELERATED
		| PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW
		| PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 24;
	pfd.iLayerType = PFD_MAIN_PLANE;

	m_hDC = ::GetWindowDC(hWnd);

	int pfd_id = ::ChoosePixelFormat(m_hDC, &pfd);
	if (0 == pfd_id)
		throw std::exception("can't choose GL pixel format!");

	if (!::SetPixelFormat(m_hDC, pfd_id, &pfd))
		throw std::exception("can't set GL pixel format!");

	m_hRC = ::wglCreateContext(m_hDC);
	if (0 == m_hRC)
		throw std::exception("can't create GL context!");
}


//////////////////////////////////////////////////////////////////////////
// destroy rendering context
Context::~Context()
{
	if (::wglGetCurrentContext() == m_hRC)
	{
		BOOL ret = ::wglMakeCurrent(0, 0);
		assert(ret!=FALSE && "Can't clear GL context");
	}

	{ // delete GL context
		BOOL ret = ::wglDeleteContext(m_hRC);
		assert(ret!=FALSE && "Can't delete GL context");
	}

	if (m_hWnd)
	{ // release DC
		int ret = ::ReleaseDC(m_hWnd, m_hDC);
		assert(ret!=FALSE && "Can't release DC context");
	}
}


//////////////////////////////////////////////////////////////////////////
// start drawing
void Context::start(HDC hDC) const
{
	//if (::wglGetCurrentContext() != m_hRC)
	{
		//::wglMakeCurrent(NULL, NULL); // deselect old context
		BOOL ret = ::wglMakeCurrent(hDC, m_hRC);
		assert(ret!=FALSE && "Can't select GL context");
	}
}


//////////////////////////////////////////////////////////////////////////
// start drawing
void Context::start() const
{
	start(m_hDC);
}


//////////////////////////////////////////////////////////////////////////
// finish drawing
void Context::flush() const
{
	::glFlush();
	assert(GL_NO_ERROR == ::glGetError()
		&& "GL flush with error");

	BOOL ret = ::SwapBuffers(m_hDC);
	assert(ret!=FALSE && "Can't swap GL buffers");
}


//////////////////////////////////////////////////////////////////////////
// GL Color default construction
Color::Color()
{
	set_R(0.0f);
	set_G(0.0f);
	set_B(0.0f);
	set_A(1.0f);
}


//////////////////////////////////////////////////////////////////////////
// GL Color construction
Color::Color(CCH_type r, CCH_type g, CCH_type b)
{
	set_R(r);
	set_G(g);
	set_B(b);
	set_A(1.0f);
}


//////////////////////////////////////////////////////////////////////////
// GL Color construction
Color::Color(CCH_type r, CCH_type g, CCH_type b, CCH_type a)
{
	set_R(r);
	set_G(g);
	set_B(b);
	set_A(a);
}


//////////////////////////////////////////////////////////////////////////
// construct GL Color object from GDI COLORREF value
Color Color::from_COLORREF(COLORREF color)
{
	return Color(
		GetRValue(color)/256.0f,
		GetGValue(color)/256.0f,
		GetBValue(color)/256.0f
	);
}


//////////////////////////////////////////////////////////////////////////
// construct GL Color object from GDI+ ARGB value
Color Color::from_ARGB(DWORD argb)
{
	return Color(
		((argb&0x00FF0000) >> 16) / 256.0f,
		((argb&0x0000FF00) >>  8) / 256.0f,
		((argb&0x000000FF) >>  0) / 256.0f,
		((argb&0xFF000000) >> 24) / 256.0f
	);
}


//////////////////////////////////////////////////////////////////////////
// common colors
const Color Color::ALICE_BLUE             = Color::from_ARGB(0xFFF0F8FF);
const Color Color::ANTIQUE_WHITE          = Color::from_ARGB(0xFFFAEBD7);
const Color Color::AQUA                   = Color::from_ARGB(0xFF00FFFF);
const Color Color::AQUAMARINE             = Color::from_ARGB(0xFF7FFFD4);
const Color Color::AZURE                  = Color::from_ARGB(0xFFF0FFFF);
const Color Color::BEIGE                  = Color::from_ARGB(0xFFF5F5DC);
const Color Color::BISQUE                 = Color::from_ARGB(0xFFFFE4C4);
const Color Color::BLACK                  = Color::from_ARGB(0xFF000000);
const Color Color::BLANCHED_ALMOND        = Color::from_ARGB(0xFFFFEBCD);
const Color Color::BLUE                   = Color::from_ARGB(0xFF0000FF);
const Color Color::BLUE_VIOLET            = Color::from_ARGB(0xFF8A2BE2);
const Color Color::BROWN                  = Color::from_ARGB(0xFFA52A2A);
const Color Color::BURLY_WOOD             = Color::from_ARGB(0xFFDEB887);
const Color Color::CADET_BLUE             = Color::from_ARGB(0xFF5F9EA0);
const Color Color::CHARTREUSE             = Color::from_ARGB(0xFF7FFF00);
const Color Color::CHOCOLATE              = Color::from_ARGB(0xFFD2691E);
const Color Color::CORAL                  = Color::from_ARGB(0xFFFF7F50);
const Color Color::CORNFLOWER_BLUE        = Color::from_ARGB(0xFF6495ED);
const Color Color::CORNSILK               = Color::from_ARGB(0xFFFFF8DC);
const Color Color::CRIMSON                = Color::from_ARGB(0xFFDC143C);
const Color Color::CYAN                   = Color::from_ARGB(0xFF00FFFF);
const Color Color::DARK_BLUE              = Color::from_ARGB(0xFF00008B);
const Color Color::DARK_CYAN              = Color::from_ARGB(0xFF008B8B);
const Color Color::DARK_GOLDENROD         = Color::from_ARGB(0xFFB8860B);
const Color Color::DARK_GRAY              = Color::from_ARGB(0xFFA9A9A9);
const Color Color::DARK_GREEN             = Color::from_ARGB(0xFF006400);
const Color Color::DARK_KHAKI             = Color::from_ARGB(0xFFBDB76B);
const Color Color::DARK_MAGENTA           = Color::from_ARGB(0xFF8B008B);
const Color Color::DARK_OLIVE_GREEN       = Color::from_ARGB(0xFF556B2F);
const Color Color::DARK_ORANGE            = Color::from_ARGB(0xFFFF8C00);
const Color Color::DARK_ORCHID            = Color::from_ARGB(0xFF9932CC);
const Color Color::DARK_RED               = Color::from_ARGB(0xFF8B0000);
const Color Color::DARK_SALMON            = Color::from_ARGB(0xFFE9967A);
const Color Color::DARK_SEA_GREEN         = Color::from_ARGB(0xFF8FBC8B);
const Color Color::DARK_SLATE_BLUE        = Color::from_ARGB(0xFF483D8B);
const Color Color::DARK_SLATE_GRAY        = Color::from_ARGB(0xFF2F4F4F);
const Color Color::DARK_TURQUOISE         = Color::from_ARGB(0xFF00CED1);
const Color Color::DARK_VIOLET            = Color::from_ARGB(0xFF9400D3);
const Color Color::DEEP_PINK              = Color::from_ARGB(0xFFFF1493);
const Color Color::DEEP_SKY_BLUE          = Color::from_ARGB(0xFF00BFFF);
const Color Color::DIM_GRAY               = Color::from_ARGB(0xFF696969);
const Color Color::DODGER_BLUE            = Color::from_ARGB(0xFF1E90FF);
const Color Color::FIREBRICK              = Color::from_ARGB(0xFFB22222);
const Color Color::FLORAL_WHITE           = Color::from_ARGB(0xFFFFFAF0);
const Color Color::FOREST_GREEN           = Color::from_ARGB(0xFF228B22);
const Color Color::FUCHSIA                = Color::from_ARGB(0xFFFF00FF);
const Color Color::GAINSBORO              = Color::from_ARGB(0xFFDCDCDC);
const Color Color::GHOST_WHITE            = Color::from_ARGB(0xFFF8F8FF);
const Color Color::GOLD                   = Color::from_ARGB(0xFFFFD700);
const Color Color::GOLDENROD              = Color::from_ARGB(0xFFDAA520);
const Color Color::GRAY                   = Color::from_ARGB(0xFF808080);
const Color Color::GREEN                  = Color::from_ARGB(0xFF008000);
const Color Color::GREEN_YELLOW           = Color::from_ARGB(0xFFADFF2F);
const Color Color::HONEYDEW               = Color::from_ARGB(0xFFF0FFF0);
const Color Color::HOT_PINK               = Color::from_ARGB(0xFFFF69B4);
const Color Color::INDIAN_RED             = Color::from_ARGB(0xFFCD5C5C);
const Color Color::INDIGO                 = Color::from_ARGB(0xFF4B0082);
const Color Color::IVORY                  = Color::from_ARGB(0xFFFFFFF0);
const Color Color::KHAKI                  = Color::from_ARGB(0xFFF0E68C);
const Color Color::LAVENDER               = Color::from_ARGB(0xFFE6E6FA);
const Color Color::LAVENDER_BLUSH         = Color::from_ARGB(0xFFFFF0F5);
const Color Color::LAWN_GREEN             = Color::from_ARGB(0xFF7CFC00);
const Color Color::LEMON_CHIFFON          = Color::from_ARGB(0xFFFFFACD);
const Color Color::LIGHT_BLUE             = Color::from_ARGB(0xFFADD8E6);
const Color Color::LIGHT_CORAL            = Color::from_ARGB(0xFFF08080);
const Color Color::LIGHT_CYAN             = Color::from_ARGB(0xFFE0FFFF);
const Color Color::LIGHT_GOLDENROD_YELLOW = Color::from_ARGB(0xFFFAFAD2);
const Color Color::LIGHT_GRAY             = Color::from_ARGB(0xFFD3D3D3);
const Color Color::LIGHT_GREEN            = Color::from_ARGB(0xFF90EE90);
const Color Color::LIGHT_PINK             = Color::from_ARGB(0xFFFFB6C1);
const Color Color::LIGHT_SALMON           = Color::from_ARGB(0xFFFFA07A);
const Color Color::LIGHT_SEA_GREEN        = Color::from_ARGB(0xFF20B2AA);
const Color Color::LIGHT_SKY_BLUE         = Color::from_ARGB(0xFF87CEFA);
const Color Color::LIGHT_SLATE_GRAY       = Color::from_ARGB(0xFF778899);
const Color Color::LIGHT_STEEL_BLUE       = Color::from_ARGB(0xFFB0C4DE);
const Color Color::LIGHT_YELLOW           = Color::from_ARGB(0xFFFFFFE0);
const Color Color::LIME                   = Color::from_ARGB(0xFF00FF00);
const Color Color::LIME_GREEN             = Color::from_ARGB(0xFF32CD32);
const Color Color::LINEN                  = Color::from_ARGB(0xFFFAF0E6);
const Color Color::MAGENTA                = Color::from_ARGB(0xFFFF00FF);
const Color Color::MAROON                 = Color::from_ARGB(0xFF800000);
const Color Color::MEDIUM_AQUAMARINE      = Color::from_ARGB(0xFF66CDAA);
const Color Color::MEDIUM_BLUE            = Color::from_ARGB(0xFF0000CD);
const Color Color::MEDIUM_ORCHID          = Color::from_ARGB(0xFFBA55D3);
const Color Color::MEDIUM_PURPLE          = Color::from_ARGB(0xFF9370DB);
const Color Color::MEDIUM_SEA_GREEN       = Color::from_ARGB(0xFF3CB371);
const Color Color::MEDIUM_SLATE_BLUE      = Color::from_ARGB(0xFF7B68EE);
const Color Color::MEDIUM_SPRING_GREEN    = Color::from_ARGB(0xFF00FA9A);
const Color Color::MEDIUM_TURQUOISE       = Color::from_ARGB(0xFF48D1CC);
const Color Color::MEDIUM_VIOLET_RED      = Color::from_ARGB(0xFFC71585);
const Color Color::MIDNIGHT_BLUE          = Color::from_ARGB(0xFF191970);
const Color Color::MINT_CREAM             = Color::from_ARGB(0xFFF5FFFA);
const Color Color::MISTY_ROSE             = Color::from_ARGB(0xFFFFE4E1);
const Color Color::MOCCASIN               = Color::from_ARGB(0xFFFFE4B5);
const Color Color::NAVAJO_WHITE           = Color::from_ARGB(0xFFFFDEAD);
const Color Color::NAVY                   = Color::from_ARGB(0xFF000080);
const Color Color::OLD_LACE               = Color::from_ARGB(0xFFFDF5E6);
const Color Color::OLIVE                  = Color::from_ARGB(0xFF808000);
const Color Color::OLIVE_DRAB             = Color::from_ARGB(0xFF6B8E23);
const Color Color::ORANGE                 = Color::from_ARGB(0xFFFFA500);
const Color Color::ORANGE_RED             = Color::from_ARGB(0xFFFF4500);
const Color Color::ORCHID                 = Color::from_ARGB(0xFFDA70D6);
const Color Color::PALE_GOLDENROD         = Color::from_ARGB(0xFFEEE8AA);
const Color Color::PALE_GREEN             = Color::from_ARGB(0xFF98FB98);
const Color Color::PALE_TURQUOISE         = Color::from_ARGB(0xFFAFEEEE);
const Color Color::PALE_VIOLET_RED        = Color::from_ARGB(0xFFDB7093);
const Color Color::PAPAYA_WHIP            = Color::from_ARGB(0xFFFFEFD5);
const Color Color::PEACH_PUFF             = Color::from_ARGB(0xFFFFDAB9);
const Color Color::PERU                   = Color::from_ARGB(0xFFCD853F);
const Color Color::PINK                   = Color::from_ARGB(0xFFFFC0CB);
const Color Color::PLUM                   = Color::from_ARGB(0xFFDDA0DD);
const Color Color::POWDER_BLUE            = Color::from_ARGB(0xFFB0E0E6);
const Color Color::PURPLE                 = Color::from_ARGB(0xFF800080);
const Color Color::RED                    = Color::from_ARGB(0xFFFF0000);
const Color Color::ROSY_BROWN             = Color::from_ARGB(0xFFBC8F8F);
const Color Color::ROYAL_BLUE             = Color::from_ARGB(0xFF4169E1);
const Color Color::SADDLE_BROWN           = Color::from_ARGB(0xFF8B4513);
const Color Color::SALMON                 = Color::from_ARGB(0xFFFA8072);
const Color Color::SANDY_BROWN            = Color::from_ARGB(0xFFF4A460);
const Color Color::SEA_GREEN              = Color::from_ARGB(0xFF2E8B57);
const Color Color::SEA_SHELL              = Color::from_ARGB(0xFFFFF5EE);
const Color Color::SIENNA                 = Color::from_ARGB(0xFFA0522D);
const Color Color::SILVER                 = Color::from_ARGB(0xFFC0C0C0);
const Color Color::SKY_BLUE               = Color::from_ARGB(0xFF87CEEB);
const Color Color::SLATE_BLUE             = Color::from_ARGB(0xFF6A5ACD);
const Color Color::SLATE_GRAY             = Color::from_ARGB(0xFF708090);
const Color Color::SNOW                   = Color::from_ARGB(0xFFFFFAFA);
const Color Color::SPRING_GREEN           = Color::from_ARGB(0xFF00FF7F);
const Color Color::STEEL_BLUE             = Color::from_ARGB(0xFF4682B4);
const Color Color::TAN                    = Color::from_ARGB(0xFFD2B48C);
const Color Color::TEAL                   = Color::from_ARGB(0xFF008080);
const Color Color::THISTLE                = Color::from_ARGB(0xFFD8BFD8);
const Color Color::TOMATO                 = Color::from_ARGB(0xFFFF6347);
const Color Color::TURQUOISE              = Color::from_ARGB(0xFF40E0D0);
const Color Color::VIOLET                 = Color::from_ARGB(0xFFEE82EE);
const Color Color::WHEAT                  = Color::from_ARGB(0xFFF5DEB3);
const Color Color::WHITE                  = Color::from_ARGB(0xFFFFFFFF);
const Color Color::WHITE_SMOKE            = Color::from_ARGB(0xFFF5F5F5);
const Color Color::YELLOW                 = Color::from_ARGB(0xFFFFFF00);
const Color Color::YELLOW_GREEN           = Color::from_ARGB(0xFF9ACD32);


//////////////////////////////////////////////////////////////////////////
// Pen construction
Pen::Pen()
	: m_color(Color::WHITE),
	  m_width(1.0f),
	  m_pattern(0xFFFF),
	  m_factor(1)
{}


//////////////////////////////////////////////////////////////////////////
// Pen construction
Pen::Pen(const Color c, float w)
	: m_color(c),
	  m_width(w),
	  m_pattern(0xFFFF),
	  m_factor(1)
{}


//////////////////////////////////////////////////////////////////////////
// Pen construction
Pen::Pen(const Color c, float w, Style style)
	: m_color(c),
	  m_width(w),
	  m_pattern(0xFFFF),
	  m_factor(1)
{
	switch (style)
	{
		case STYLE_DASH:
			m_pattern = 0xFF00;
			break;

		case STYLE_DOT:
			m_pattern = 0xF0F0;
			break;

		case STYLE_DASH_DOT:
			m_pattern = 0xFF2C;
			break;

		case STYLE_DASH_DOTDOT:
			m_pattern = 0xFF2C;
			break;
	}
}


//////////////////////////////////////////////////////////////////////////
// Pen construction
Pen::Pen(const Color c, float w, GLint sfactor, GLushort spattern)
	: m_color(c),
	  m_width(w),
	  m_pattern(spattern),
	  m_factor(sfactor)
{}


//////////////////////////////////////////////////////////////////////////
// select Pen styles in current GL rendering context
void Pen::select() const
{
	Canvas::color(m_color);
	::glLineWidth(m_width);

	::glLineStipple(m_factor, m_pattern);
	if (m_factor!=1 || m_pattern!=0xFFFF)
		::glEnable(GL_LINE_STIPPLE);
	else
		::glDisable(GL_LINE_STIPPLE);
}


//////////////////////////////////////////////////////////////////////////
// Font construction
Font::Font()
{
	m_base = ::glGenLists(N_CHARS);

	{ // create glyphs
		HDC hDC = ::wglGetCurrentDC();
		HFONT font = ::CreateFont(-14, 0, 0, 0, FW_NORMAL, 0, 0, 0,
			ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS,
				ANTIALIASED_QUALITY, FF_DONTCARE|DEFAULT_PITCH,
					_T("Tahoma"));
		HFONT old_font = SelectFont(hDC, font);

		BOOL ret = ::wglUseFontOutlines(hDC, 0, N_CHARS,
			m_base, 0.0, 0.0, WGL_FONT_POLYGONS, m_glyphs);
		if (ret != TRUE) throw std::exception("Can't create font outlines");

		SelectFont(hDC, old_font);
		::DeleteFont(font);
	}

	// calculate maximum height
	m_max_ascent = m_max_descent = 0.0f;
	for (size_t i = 0; i < N_CHARS; ++i)
	{
		const float a = m_glyphs[i].gmfptGlyphOrigin.y;
		const float d = m_glyphs[i].gmfBlackBoxY - a;

		if (m_max_ascent < a)
			m_max_ascent = a;
		if (m_max_descent < d)
			m_max_descent = d;
	}
}


//////////////////////////////////////////////////////////////////////////
// Font destruction
Font::~Font()
{
	// TODO: ::glDeleteLists(m_base, N_CHARS);
	// this line is commented, because in the WM_DESTROY
	// message handler, we can't call wglMakeCurrent()
}


//////////////////////////////////////////////////////////////////////////
// get Font base
GLuint Font::base() const
{
	return m_base;
}


//////////////////////////////////////////////////////////////////////////
// select font into the current GL rendering context
void Font::select() const
{
	::glListBase(base());
}


//////////////////////////////////////////////////////////////////////////
// get maximum font ascent
float Font::max_ascent() const
{
	return m_max_ascent;
}

//////////////////////////////////////////////////////////////////////////
// get maximum font descent
float Font::max_descent() const
{
	return m_max_descent;
}


//////////////////////////////////////////////////////////////////////////
// get maximum font height
float Font::max_height() const
{
	return m_max_ascent + m_max_descent;
}


//////////////////////////////////////////////////////////////////////////
// get metrics
const Font::Glyph& Font::glyph(size_t ch) const
{
	assert(ch < N_CHARS
		&& "Invalid char");
	return m_glyphs[ch];
}


//////////////////////////////////////////////////////////////////////////
// calculate text height
float Font::text_ascent(const char *text) const
{
	float A = 0.0f;

	for (const char *p = text; *p; ++p)
	{
		const Glyph &gm = glyph(*p);
		if (A < gm.gmfptGlyphOrigin.y)
			A = gm.gmfptGlyphOrigin.y;
	}

	return A;
}


//////////////////////////////////////////////////////////////////////////
// calculate text height
float Font::text_descent(const char *text) const
{
	float D = 0.0f;

	for (const char *p = text; *p; ++p)
	{
		const Glyph &gm = glyph(*p);
		if (D < gm.gmfptGlyphOrigin.y - gm.gmfBlackBoxY)
			D = gm.gmfptGlyphOrigin.y - gm.gmfBlackBoxY;
	}

	return D;
}


//////////////////////////////////////////////////////////////////////////
// calculate text height
float Font::text_height(const char *text) const
{
	float H = 0.0f;

	for (const char *p = text; *p; ++p)
	{
		const Glyph &gm = glyph(*p);
		if (H < gm.gmfBlackBoxY)
			H = gm.gmfBlackBoxY;
	}

	return H;
}


//////////////////////////////////////////////////////////////////////////
// calculate text width
float Font::text_width(const char *text) const
{
	float W = 0.0f;

	for (const char *p = text; *p; ++p)
	{
		const Font::Glyph &gm = glyph(*p);
		W += gm.gmfCellIncX;
	}

	return W;
}


//////////////////////////////////////////////////////////////////////////
// draw text
void drawText(const Font &font, const char *text,
	double x, double y, double x_scale, double y_scale,
		double angle, TextAlign::Enum align)
{
	const double W = font.text_width(text);
	const double D = font.max_descent();
	const double A = font.max_ascent();

	const double x_rot = cos(angle * M_PI/180.0);
	const double y_rot = sin(angle * M_PI/180.0);

	// horizontal
	if (align & TextAlign::HCENTER)
	{
		x -= W/2 * x_rot * x_scale;
		y -= W/2 * y_rot * y_scale;
	}
	else if (align & TextAlign::RIGHT)
	{
		x -= W * x_rot * x_scale;
		y -= W * y_rot * y_scale;
	}

	// vertical
	if (align & TextAlign::VCENTER)
	{
		x += (A-D)/2 * y_rot * x_scale;
		y -= (A-D)/2 * x_rot * y_scale;
	}
	else if (align & TextAlign::TOP)
	{
		x += A * y_rot * x_scale;
		y -= A * x_rot * y_scale;
	}
	else if (align & TextAlign::BOTTOM)
	{
		x -= D * y_rot * x_scale;
		y += D * x_rot * y_scale;
	}

	MatrixBackup backup;
	translate(x, y, 0.0);
	scale(x_scale, y_scale, 1.0);
	rotate(angle, 0.0, 0.0, 1.0);

	::glCallLists(strlen(text),
		GL_UNSIGNED_BYTE, text);

	assert(GL_NO_ERROR == ::glGetError()
		&& "Can't draw text");
}

	} // GL namespace
} // omni namespace
