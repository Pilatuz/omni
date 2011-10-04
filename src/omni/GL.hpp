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
#ifndef __OMNI_GL_PLUS_H_
#define __OMNI_GL_PLUS_H_

#if !defined(_WINDOWS_)
#	error windows.h must be included first
#endif // _WINDOWS_

#include <omni/defs.hpp>

#include <gl/GL.h>

#include <assert.h>

namespace omni
{
	namespace GL
	{

//////////////////////////////////////////////////////////////////////////
// GL Rendering Context
class Context: private NonCopyable {
public:
	explicit Context(HDC hDC);
	explicit Context(HWND hWnd);
	~Context();

public:
	void start(HDC hDC) const;
	void start() const;
	void flush() const;

private:
	HWND  m_hWnd;
	HGLRC m_hRC;
	HDC   m_hDC;
};


//////////////////////////////////////////////////////////////////////////
// Array class - fixed size array
template<typename T, GLsizei N>
	class Array {
	public:
		typedef T value_type;
		enum { size = N };
		value_type v[N];

	public:
		const T& operator[](GLsizei i) const { return v[i]; }
		      T& operator[](GLsizei i)       { return v[i]; }
	};


//////////////////////////////////////////////////////////////////////////
// Color class
class Color {
public:
	typedef GLfloat CCH_type; // color channel
	typedef Array<CCH_type, 4> RAW_Format;

public:
	Color();
	Color(CCH_type r, CCH_type g, CCH_type b);
	Color(CCH_type r, CCH_type g, CCH_type b, CCH_type a);

public:
	static Color from_COLORREF(COLORREF color);
	static Color from_ARGB(DWORD argb);

public:
	CCH_type R() const { return m_RAW[0]; }
	CCH_type G() const { return m_RAW[1]; }
	CCH_type B() const { return m_RAW[2]; }
	CCH_type A() const { return m_RAW[3]; }

	void set_R(CCH_type r) { m_RAW[0] = r; }
	void set_G(CCH_type g) { m_RAW[1] = g; }
	void set_B(CCH_type b) { m_RAW[2] = b; }
	void set_A(CCH_type a) { m_RAW[3] = a; }

	const RAW_Format& RAW() const { return m_RAW; }
	      RAW_Format& RAW()       { return m_RAW; }


public:
	static const Color ALICE_BLUE;
	static const Color ANTIQUE_WHITE;
	static const Color AQUA;
	static const Color AQUAMARINE;
	static const Color AZURE;
	static const Color BEIGE;
	static const Color BISQUE;
	static const Color BLACK;
	static const Color BLANCHED_ALMOND;
	static const Color BLUE;
	static const Color BLUE_VIOLET;
	static const Color BROWN;
	static const Color BURLY_WOOD;
	static const Color CADET_BLUE;
	static const Color CHARTREUSE;
	static const Color CHOCOLATE;
	static const Color CORAL;
	static const Color CORNFLOWER_BLUE;
	static const Color CORNSILK;
	static const Color CRIMSON;
	static const Color CYAN;
	static const Color DARK_BLUE;
	static const Color DARK_CYAN;
	static const Color DARK_GOLDENROD;
	static const Color DARK_GRAY;
	static const Color DARK_GREEN;
	static const Color DARK_KHAKI;
	static const Color DARK_MAGENTA;
	static const Color DARK_OLIVE_GREEN;
	static const Color DARK_ORANGE;
	static const Color DARK_ORCHID;
	static const Color DARK_RED;
	static const Color DARK_SALMON;
	static const Color DARK_SEA_GREEN;
	static const Color DARK_SLATE_BLUE;
	static const Color DARK_SLATE_GRAY;
	static const Color DARK_TURQUOISE;
	static const Color DARK_VIOLET;
	static const Color DEEP_PINK;
	static const Color DEEP_SKY_BLUE;
	static const Color DIM_GRAY;
	static const Color DODGER_BLUE;
	static const Color FIREBRICK;
	static const Color FLORAL_WHITE;
	static const Color FOREST_GREEN;
	static const Color FUCHSIA;
	static const Color GAINSBORO;
	static const Color GHOST_WHITE;
	static const Color GOLD;
	static const Color GOLDENROD;
	static const Color GRAY;
	static const Color GREEN;
	static const Color GREEN_YELLOW;
	static const Color HONEYDEW;
	static const Color HOT_PINK;
	static const Color INDIAN_RED;
	static const Color INDIGO;
	static const Color IVORY;
	static const Color KHAKI;
	static const Color LAVENDER;
	static const Color LAVENDER_BLUSH;
	static const Color LAWN_GREEN;
	static const Color LEMON_CHIFFON;
	static const Color LIGHT_BLUE;
	static const Color LIGHT_CORAL;
	static const Color LIGHT_CYAN;
	static const Color LIGHT_GOLDENROD_YELLOW;
	static const Color LIGHT_GRAY;
	static const Color LIGHT_GREEN;
	static const Color LIGHT_PINK;
	static const Color LIGHT_SALMON;
	static const Color LIGHT_SEA_GREEN;
	static const Color LIGHT_SKY_BLUE;
	static const Color LIGHT_SLATE_GRAY;
	static const Color LIGHT_STEEL_BLUE;
	static const Color LIGHT_YELLOW;
	static const Color LIME;
	static const Color LIME_GREEN;
	static const Color LINEN;
	static const Color MAGENTA;
	static const Color MAROON;
	static const Color MEDIUM_AQUAMARINE;
	static const Color MEDIUM_BLUE;
	static const Color MEDIUM_ORCHID;
	static const Color MEDIUM_PURPLE;
	static const Color MEDIUM_SEA_GREEN;
	static const Color MEDIUM_SLATE_BLUE;
	static const Color MEDIUM_SPRING_GREEN;
	static const Color MEDIUM_TURQUOISE;
	static const Color MEDIUM_VIOLET_RED;
	static const Color MIDNIGHT_BLUE;
	static const Color MINT_CREAM;
	static const Color MISTY_ROSE;
	static const Color MOCCASIN;
	static const Color NAVAJO_WHITE;
	static const Color NAVY;
	static const Color OLD_LACE;
	static const Color OLIVE;
	static const Color OLIVE_DRAB;
	static const Color ORANGE;
	static const Color ORANGE_RED;
	static const Color ORCHID;
	static const Color PALE_GOLDENROD;
	static const Color PALE_GREEN;
	static const Color PALE_TURQUOISE;
	static const Color PALE_VIOLET_RED;
	static const Color PAPAYA_WHIP;
	static const Color PEACH_PUFF;
	static const Color PERU;
	static const Color PINK;
	static const Color PLUM;
	static const Color POWDER_BLUE;
	static const Color PURPLE;
	static const Color RED;
	static const Color ROSY_BROWN;
	static const Color ROYAL_BLUE;
	static const Color SADDLE_BROWN;
	static const Color SALMON;
	static const Color SANDY_BROWN;
	static const Color SEA_GREEN;
	static const Color SEA_SHELL;
	static const Color SIENNA;
	static const Color SILVER;
	static const Color SKY_BLUE;
	static const Color SLATE_BLUE;
	static const Color SLATE_GRAY;
	static const Color SNOW;
	static const Color SPRING_GREEN;
	static const Color STEEL_BLUE;
	static const Color TAN;
	static const Color TEAL;
	static const Color THISTLE;
	static const Color TOMATO;
	static const Color TURQUOISE;
	static const Color VIOLET;
	static const Color WHEAT;
	static const Color WHITE;
	static const Color WHITE_SMOKE;
	static const Color YELLOW;
	static const Color YELLOW_GREEN;

private:
	RAW_Format m_RAW;
};


//////////////////////////////////////////////////////////////////////////
// Canvas class
class Canvas {
public: // glVertex overloads
	static void vertex(GLdouble x, GLdouble y) { ::glVertex2d(x, y); }
	static void vertex( GLfloat x,  GLfloat y) { ::glVertex2f(x, y); }
	static void vertex(   GLint x,    GLint y) { ::glVertex2i(x, y); }
	static void vertex( GLshort x,  GLshort y) { ::glVertex2s(x, y); }
	static void vertex(GLdouble x, GLdouble y, GLdouble z) { ::glVertex3d(x, y, z); }
	static void vertex( GLfloat x,  GLfloat y,  GLfloat z) { ::glVertex3f(x, y, z); }
	static void vertex(   GLint x,    GLint y,    GLint z) { ::glVertex3i(x, y, z); }
	static void vertex( GLshort x,  GLshort y,  GLshort z) { ::glVertex3s(x, y, z); }
	static void vertex(GLdouble x, GLdouble y, GLdouble z, GLdouble w) { ::glVertex4d(x, y, z, w); }
	static void vertex( GLfloat x,  GLfloat y,  GLfloat z,  GLfloat w) { ::glVertex4f(x, y, z, w); }
	static void vertex(   GLint x,    GLint y,    GLint z,    GLint w) { ::glVertex4i(x, y, z, w); }
	static void vertex( GLshort x,  GLshort y,  GLshort z,  GLshort w) { ::glVertex4s(x, y, z, w); }
	static void vertex(const Array<GLdouble, 2> &v) { ::glVertex2dv(&v[0]); }
	static void vertex(const Array< GLfloat, 2> &v) { ::glVertex2fv(&v[0]); }
	static void vertex(const Array<   GLint, 2> &v) { ::glVertex2iv(&v[0]); }
	static void vertex(const Array< GLshort, 2> &v) { ::glVertex2sv(&v[0]); }
	static void vertex(const Array<GLdouble, 3> &v) { ::glVertex3dv(&v[0]); }
	static void vertex(const Array< GLfloat, 3> &v) { ::glVertex3fv(&v[0]); }
	static void vertex(const Array<   GLint, 3> &v) { ::glVertex3iv(&v[0]); }
	static void vertex(const Array< GLshort, 3> &v) { ::glVertex3sv(&v[0]); }
	static void vertex(const Array<GLdouble, 4> &v) { ::glVertex4dv(&v[0]); }
	static void vertex(const Array< GLfloat, 4> &v) { ::glVertex4fv(&v[0]); }
	static void vertex(const Array<   GLint, 4> &v) { ::glVertex4iv(&v[0]); }
	static void vertex(const Array< GLshort, 4> &v) { ::glVertex4sv(&v[0]); }
public: // glColor overloads
	static void color(  GLbyte red,   GLbyte green,   GLbyte blue) { ::glColor3b(red, green, blue); }
	static void color(GLdouble red, GLdouble green, GLdouble blue) { ::glColor3d(red, green, blue); }
	static void color( GLfloat red,  GLfloat green,  GLfloat blue) { ::glColor3f(red, green, blue); }
	static void color(   GLint red,    GLint green,    GLint blue) { ::glColor3i(red, green, blue); }
	static void color( GLshort red,  GLshort green,  GLshort blue) { ::glColor3s(red, green, blue); }
	static void color( GLubyte red,  GLubyte green,  GLubyte blue) { ::glColor3ub(red, green, blue); }
	static void color(  GLuint red,   GLuint green,   GLuint blue) { ::glColor3ui(red, green, blue); }
	static void color(GLushort red, GLushort green, GLushort blue) { ::glColor3us(red, green, blue); }
	static void color(  GLbyte red,   GLbyte green,   GLbyte blue,   GLbyte alpha) { ::glColor4b(red, green, blue, alpha); }
	static void color(GLdouble red, GLdouble green, GLdouble blue, GLdouble alpha) { ::glColor4d(red, green, blue, alpha); }
	static void color( GLfloat red,  GLfloat green,  GLfloat blue,  GLfloat alpha) { ::glColor4f(red, green, blue, alpha); }
	static void color(   GLint red,    GLint green,    GLint blue,    GLint alpha) { ::glColor4i(red, green, blue, alpha); }
	static void color( GLshort red,  GLshort green,  GLshort blue,  GLshort alpha) { ::glColor4s(red, green, blue, alpha); }
	static void color( GLubyte red,  GLubyte green,  GLubyte blue,  GLubyte alpha) { ::glColor4ub(red, green, blue, alpha); }
	static void color(  GLuint red,   GLuint green,   GLuint blue,   GLuint alpha) { ::glColor4ui(red, green, blue, alpha); }
	static void color(GLushort red, GLushort green, GLushort blue, GLushort alpha) { ::glColor4us(red, green, blue, alpha); }
	static void color(const Array<  GLbyte, 3> &v) { ::glColor3bv(&v[0]); }
	static void color(const Array<GLdouble, 3> &v) { ::glColor3dv(&v[0]); }
	static void color(const Array< GLfloat, 3> &v) { ::glColor3fv(&v[0]); }
	static void color(const Array<   GLint, 3> &v) { ::glColor3iv(&v[0]); }
	static void color(const Array< GLshort, 3> &v) { ::glColor3sv(&v[0]); }
	static void color(const Array< GLubyte, 3> &v) { ::glColor3ubv(&v[0]); }
	static void color(const Array<  GLuint, 3> &v) { ::glColor3uiv(&v[0]); }
	static void color(const Array<GLushort, 3> &v) { ::glColor3usv(&v[0]); }
	static void color(const Array<  GLbyte, 4> &v) { ::glColor4bv(&v[0]); }
	static void color(const Array<GLdouble, 4> &v) { ::glColor4dv(&v[0]); }
	static void color(const Array< GLfloat, 4> &v) { ::glColor4fv(&v[0]); }
	static void color(const Array<   GLint, 4> &v) { ::glColor4iv(&v[0]); }
	static void color(const Array< GLshort, 4> &v) { ::glColor4sv(&v[0]); }
	static void color(const Array< GLubyte, 4> &v) { ::glColor4ubv(&v[0]); }
	static void color(const Array<  GLuint, 4> &v) { ::glColor4uiv(&v[0]); }
	static void color(const Array<GLushort, 4> &v) { ::glColor4usv(&v[0]); }
	static void color(const Color &c) { color(c.RAW()); }
public: // glIndex overloads
	static void index(GLdouble c) { ::glIndexd(c); }
	static void index( GLfloat c) { ::glIndexf(c); }
	static void index(   GLint c) { ::glIndexi(c); }
	static void index( GLshort c) { ::glIndexs(c); }
	static void index( GLubyte c) { ::glIndexub(c); }
	static void index(const Array< GLubyte, 1> &c) { ::glIndexubv(&c[0]); }
	static void index(const Array<GLdouble, 1> &c) { ::glIndexdv(&c[0]); }
	static void index(const Array< GLfloat, 1> &c) { ::glIndexfv(&c[0]); }
	static void index(const Array<   GLint, 1> &c) { ::glIndexiv(&c[0]); }
	static void index(const Array< GLshort, 1> &c) { ::glIndexsv(&c[0]); }
public: // glNormal overloads
	static void normal(  GLbyte nx,   GLbyte ny,   GLbyte nz) { ::glNormal3b(nx, ny, nz); }
	static void normal(GLdouble nx, GLdouble ny, GLdouble nz) { ::glNormal3d(nx, ny, nz); }
	static void normal( GLfloat nx,  GLfloat ny,  GLfloat nz) { ::glNormal3f(nx, ny, nz); }
	static void normal(   GLint nx,    GLint ny,    GLint nz) { ::glNormal3i(nx, ny, nz); }
	static void normal( GLshort nx,  GLshort ny,  GLshort nz) { ::glNormal3s(nx, ny, nz); }
	static void normal(const Array<  GLbyte, 3> &v) { ::glNormal3bv(&v[0]); }
	static void normal(const Array<GLdouble, 3> &v) { ::glNormal3dv(&v[0]); }
	static void normal(const Array< GLfloat, 3> &v) { ::glNormal3fv(&v[0]); }
	static void normal(const Array<   GLint, 3> &v) { ::glNormal3iv(&v[0]); }
	static void normal(const Array< GLshort, 3> &v) { ::glNormal3sv(&v[0]); }
public: // glRasterPos overloads
	static void raster_pos(GLdouble x, GLdouble y) { ::glRasterPos2d(x, y); }
	static void raster_pos( GLfloat x,  GLfloat y) { ::glRasterPos2f(x, y); }
	static void raster_pos(   GLint x,    GLint y) { ::glRasterPos2i(x, y); }
	static void raster_pos( GLshort x,  GLshort y) { ::glRasterPos2s(x, y); }
	static void raster_pos(GLdouble x, GLdouble y, GLdouble z) { ::glRasterPos3d(x, y, z); }
	static void raster_pos( GLfloat x,  GLfloat y,  GLfloat z) { ::glRasterPos3f(x, y, z); }
	static void raster_pos(   GLint x,    GLint y,    GLint z) { ::glRasterPos3i(x, y, z); }
	static void raster_pos( GLshort x,  GLshort y,  GLshort z) { ::glRasterPos3s(x, y, z); }
	static void raster_pos(GLdouble x, GLdouble y, GLdouble z, GLdouble w) { ::glRasterPos4d(x, y, z, w); }
	static void raster_pos( GLfloat x,  GLfloat y,  GLfloat z,  GLfloat w) { ::glRasterPos4f(x, y, z, w); }
	static void raster_pos(   GLint x,    GLint y,    GLint z,    GLint w) { ::glRasterPos4i(x, y, z, w); }
	static void raster_pos( GLshort x,  GLshort y,  GLshort z,  GLshort w) { ::glRasterPos4s(x, y, z, w); }
	static void raster_pos(const Array<GLdouble, 2> &v) { ::glRasterPos2dv(&v[0]); }
	static void raster_pos(const Array< GLfloat, 2> &v) { ::glRasterPos2fv(&v[0]); }
	static void raster_pos(const Array<   GLint, 2> &v) { ::glRasterPos2iv(&v[0]); }
	static void raster_pos(const Array< GLshort, 2> &v) { ::glRasterPos2sv(&v[0]); }
	static void raster_pos(const Array<GLdouble, 3> &v) { ::glRasterPos3dv(&v[0]); }
	static void raster_pos(const Array< GLfloat, 3> &v) { ::glRasterPos3fv(&v[0]); }
	static void raster_pos(const Array<   GLint, 3> &v) { ::glRasterPos3iv(&v[0]); }
	static void raster_pos(const Array< GLshort, 3> &v) { ::glRasterPos3sv(&v[0]); }
	static void raster_pos(const Array<GLdouble, 4> &v) { ::glRasterPos4dv(&v[0]); }
	static void raster_pos(const Array< GLfloat, 4> &v) { ::glRasterPos4fv(&v[0]); }
	static void raster_pos(const Array<   GLint, 4> &v) { ::glRasterPos4iv(&v[0]); }
	static void raster_pos(const Array< GLshort, 4> &v) { ::glRasterPos4sv(&v[0]); }
public: // glTexCoord overloads
	static void tex_coord(GLdouble s) { ::glTexCoord1d(s); }
	static void tex_coord( GLfloat s) { ::glTexCoord1f(s); }
	static void tex_coord(   GLint s) { ::glTexCoord1i(s); }
	static void tex_coord( GLshort s) { ::glTexCoord1s(s); }
	static void tex_coord(GLdouble s, GLdouble t) { ::glTexCoord2d(s, t); }
	static void tex_coord( GLfloat s,  GLfloat t) { ::glTexCoord2f(s, t); }
	static void tex_coord(   GLint s,    GLint t) { ::glTexCoord2i(s, t); }
	static void tex_coord( GLshort s,  GLshort t) { ::glTexCoord2s(s, t); }
	static void tex_coord(GLdouble s, GLdouble t, GLdouble r) { ::glTexCoord3d(s, t, r); }
	static void tex_coord( GLfloat s,  GLfloat t,  GLfloat r) { ::glTexCoord3f(s, t, r); }
	static void tex_coord(   GLint s,    GLint t,    GLint r) { ::glTexCoord3i(s, t, r); }
	static void tex_coord( GLshort s,  GLshort t,  GLshort r) { ::glTexCoord3s(s, t, r); }
	static void tex_coord(GLdouble s, GLdouble t, GLdouble r, GLdouble q) { ::glTexCoord4d(s, t, r, q); }
	static void tex_coord( GLfloat s,  GLfloat t,  GLfloat r,  GLfloat q) { ::glTexCoord4f(s, t, r, q); }
	static void tex_coord(   GLint s,    GLint t,    GLint r,    GLint q) { ::glTexCoord4i(s, t, r, q); }
	static void tex_coord( GLshort s,  GLshort t,  GLshort r,  GLshort q) { ::glTexCoord4s(s, t, r, q); }
	static void tex_coord(const Array<GLdouble, 1> &v) { ::glTexCoord1dv(&v[0]); }
	static void tex_coord(const Array< GLfloat, 1> &v) { ::glTexCoord1fv(&v[0]); }
	static void tex_coord(const Array<   GLint, 1> &v) { ::glTexCoord1iv(&v[0]); }
	static void tex_coord(const Array< GLshort, 1> &v) { ::glTexCoord1sv(&v[0]); }
	static void tex_coord(const Array<GLdouble, 2> &v) { ::glTexCoord2dv(&v[0]); }
	static void tex_coord(const Array< GLfloat, 2> &v) { ::glTexCoord2fv(&v[0]); }
	static void tex_coord(const Array<   GLint, 2> &v) { ::glTexCoord2iv(&v[0]); }
	static void tex_coord(const Array< GLshort, 2> &v) { ::glTexCoord2sv(&v[0]); }
	static void tex_coord(const Array<GLdouble, 3> &v) { ::glTexCoord3dv(&v[0]); }
	static void tex_coord(const Array< GLfloat, 3> &v) { ::glTexCoord3fv(&v[0]); }
	static void tex_coord(const Array<   GLint, 3> &v) { ::glTexCoord3iv(&v[0]); }
	static void tex_coord(const Array< GLshort, 3> &v) { ::glTexCoord3sv(&v[0]); }
	static void tex_coord(const Array<GLdouble, 4> &v) { ::glTexCoord4dv(&v[0]); }
	static void tex_coord(const Array< GLfloat, 4> &v) { ::glTexCoord4fv(&v[0]); }
	static void tex_coord(const Array<   GLint, 4> &v) { ::glTexCoord4iv(&v[0]); }
	static void tex_coord(const Array< GLshort, 4> &v) { ::glTexCoord4sv(&v[0]); }
public: // glEvalCoord overloads
	static void eval_coord(GLdouble u) { ::glEvalCoord1d(u); }
	static void eval_coord( GLfloat u) { ::glEvalCoord1f(u); }
	static void eval_coord(GLdouble u, GLdouble v) { ::glEvalCoord2d(u, v); }
	static void eval_coord( GLfloat u,  GLfloat v) { ::glEvalCoord2f(u, v); }
	static void eval_coord(const Array<GLdouble, 1> &u) { ::glEvalCoord1dv(&u[0]); }
	static void eval_coord(const Array< GLfloat, 1> &u) { ::glEvalCoord1fv(&u[0]); }
	static void eval_coord(const Array<GLdouble, 2> &u) { ::glEvalCoord2dv(&u[0]); }
	static void eval_coord(const Array< GLfloat, 2> &u) { ::glEvalCoord2fv(&u[0]); }
public: // glEvalPoint overloads
	static void eval_point(GLint i) { ::glEvalPoint1(i); }
	static void eval_point(GLint i, GLint j) { ::glEvalPoint2(i, j); }
public: // glEdgeFlag overloads
	static void edge_flag(GLboolean flag) { ::glEdgeFlag(flag); }
	static void edge_flag(const Array<GLboolean, 1> &flag) { ::glEdgeFlagv(&flag[0]); }
public: // glRect overloads
	static void rect(GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2) { ::glRectd(x1, y1, x2, y2); }
	static void rect( GLfloat x1,  GLfloat y1,  GLfloat x2,  GLfloat y2) { ::glRectf(x1, y1, x2, y2); }
	static void rect(   GLint x1,    GLint y1,    GLint x2,    GLint y2) { ::glRecti(x1, y1, x2, y2); }
	static void rect( GLshort x1,  GLshort y1,  GLshort x2,  GLshort y2) { ::glRects(x1, y1, x2, y2); }
	static void rect(const Array< GLshort, 2> &v1, const Array< GLshort, 2> &v2) { ::glRectsv(&v1[0], &v2[0]); }
	static void rect(const Array<GLdouble, 2> &v1, const Array<GLdouble, 2> &v2) { ::glRectdv(&v1[0], &v2[0]); }
	static void rect(const Array< GLfloat, 2> &v1, const Array< GLfloat, 2> &v2) { ::glRectfv(&v1[0], &v2[0]); }
	static void rect(const Array<   GLint, 2> &v1, const Array<   GLint, 2> &v2) { ::glRectiv(&v1[0], &v2[0]); }
};


//////////////////////////////////////////////////////////////////////////
// glClearColor
inline void clearColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
	::glClearColor(red, green, blue, alpha);
	assert(GL_NO_ERROR == ::glGetError()
		&& "Can't set GL clear color");
}
inline void clearColor(const Array<GLfloat, 4> &color)
{
	::glClearColor(color[0], color[1], color[2], color[3]);
	assert(GL_NO_ERROR == ::glGetError()
		&& "Can't set GL clear color");
}
inline void clearColor(const Color &color)
{
	clearColor(color.RAW());
}
inline Color clearColor()
{
	Color color;

	::glGetFloatv(GL_COLOR_CLEAR_VALUE, &color.RAW()[0]);
	assert(GL_NO_ERROR == ::glGetError()
		&& "Can't get GL clear color");

	return color;
}


//////////////////////////////////////////////////////////////////////////
// Buffer bit
struct Buffer {
	enum Enum {
		COLOR = GL_COLOR_BUFFER_BIT,
		DEPTH = GL_DEPTH_BUFFER_BIT,
		ACCUM = GL_ACCUM_BUFFER_BIT,
		STENCIL = GL_STENCIL_BUFFER_BIT
	};
};

// combine two buffers
inline Buffer::Enum operator|(const Buffer::Enum a, const Buffer::Enum b)
{
	return Buffer::Enum(GLbitfield(a) | GLbitfield(b));
}


//////////////////////////////////////////////////////////////////////////
// glClear
inline void clear(Buffer::Enum buffers)
{
	::glClear(buffers);
	assert(GL_NO_ERROR == ::glGetError()
		&& "Can't clear GL buffers");
}


//////////////////////////////////////////////////////////////////////////
// Attrib bit
struct Attrib {
	enum Enum {
		ACCUM_BUFFER    = GL_ACCUM_BUFFER_BIT,
		COLOR_BUFFER    = GL_COLOR_BUFFER_BIT,
		CURRENT         = GL_CURRENT_BIT,
		DEPTH_BUFFER    = GL_DEPTH_BUFFER_BIT,
		ENABLE          = GL_ENABLE_BIT,
		EVAL            = GL_EVAL_BIT,
		FOG             = GL_FOG_BIT,
		HINT            = GL_HINT_BIT,
		LIGHTING        = GL_LIGHTING_BIT,
		LINE            = GL_LINE_BIT,
		LIST            = GL_LIST_BIT,
		PIXEL_MODE      = GL_PIXEL_MODE_BIT,
		POINT           = GL_POINT_BIT,
		POLYGON         = GL_POLYGON_BIT,
		POLYGON_STIPPLE = GL_POLYGON_STIPPLE_BIT,
		SCISSOR         = GL_SCISSOR_BIT,
		STENCIL_BUFFER  = GL_STENCIL_BUFFER_BIT,
		TEXTURE         = GL_TEXTURE_BIT,
		TRANSFORM       = GL_TRANSFORM_BIT,
		VIEWPORT        = GL_VIEWPORT_BIT
	};
};

// combine two attributes
inline Attrib::Enum operator|(const Attrib::Enum a, const Attrib::Enum b)
{
	return Attrib::Enum(GLbitfield(a) | GLbitfield(b));
}


//////////////////////////////////////////////////////////////////////////
// glPushAttrib/glPopAttrib wrapper
class AttribBackup {
public:
	explicit AttribBackup(Attrib::Enum attrib)
	{
		::glPushAttrib(attrib);
		assert(GL_NO_ERROR == ::glGetError()
			&& "Can't push attributes");
	}

	~AttribBackup()
	{
		::glPopAttrib();
		assert(GL_NO_ERROR == ::glGetError()
			&& "Can't pop attributes");
	}
};


//////////////////////////////////////////////////////////////////////////
// Capability class
struct Capability {
	enum Enum {
		ALPHA_TEST           = GL_ALPHA_TEST,
		AUTO_NORMAL          = GL_AUTO_NORMAL,
		BLEND                = GL_BLEND,
		CLIP_PLANE0          = GL_CLIP_PLANE0,
		CLIP_PLANE1          = GL_CLIP_PLANE1,
		CLIP_PLANE2          = GL_CLIP_PLANE2,
		CLIP_PLANE3          = GL_CLIP_PLANE3,
		CLIP_PLANE4          = GL_CLIP_PLANE4,
		CLIP_PLANE5          = GL_CLIP_PLANE5,
		COLOR_LOGIC_OP       = GL_COLOR_LOGIC_OP,
		COLOR_MATERIAL       = GL_COLOR_MATERIAL,
		CULL_FACE            = GL_CULL_FACE,
		DEPTH_TEST           = GL_DEPTH_TEST,
		DITHER               = GL_DITHER,
		FOG                  = GL_FOG,
		INDEX_LOGIC_OP       = GL_INDEX_LOGIC_OP,
		LIGHT0               = GL_LIGHT0,
		LIGHT1               = GL_LIGHT1,
		LIGHT2               = GL_LIGHT2,
		LIGHT3               = GL_LIGHT3,
		LIGHT4               = GL_LIGHT4,
		LIGHT5               = GL_LIGHT5,
		LIGHT6               = GL_LIGHT6,
		LIGHT7               = GL_LIGHT7,
		LIGHTING             = GL_LIGHTING,
		LINE_SMOOTH          = GL_LINE_SMOOTH,
		LINE_STIPPLE         = GL_LINE_STIPPLE,
		LOGIC_OP             = GL_LOGIC_OP,
		MAP1_COLOR_4         = GL_MAP1_COLOR_4,
		MAP1_INDEX           = GL_MAP1_INDEX,
		MAP1_NORMAL          = GL_MAP1_NORMAL,
		MAP1_TEXTURE_COORD_1 = GL_MAP1_TEXTURE_COORD_1,
		MAP1_TEXTURE_COORD_2 = GL_MAP1_TEXTURE_COORD_2,
		MAP1_TEXTURE_COORD_3 = GL_MAP1_TEXTURE_COORD_3,
		MAP1_TEXTURE_COORD_4 = GL_MAP1_TEXTURE_COORD_4,
		MAP1_VERTEX_3        = GL_MAP1_VERTEX_3,
		MAP1_VERTEX_4        = GL_MAP1_VERTEX_4,
		MAP2_COLOR_4         = GL_MAP2_COLOR_4,
		MAP2_INDEX           = GL_MAP2_INDEX,
		MAP2_NORMAL          = GL_MAP2_NORMAL,
		MAP2_TEXTURE_COORD_1 = GL_MAP2_TEXTURE_COORD_1,
		MAP2_TEXTURE_COORD_2 = GL_MAP2_TEXTURE_COORD_2,
		MAP2_TEXTURE_COORD_3 = GL_MAP2_TEXTURE_COORD_3,
		MAP2_TEXTURE_COORD_4 = GL_MAP2_TEXTURE_COORD_4,
		MAP2_VERTEX_3        = GL_MAP2_VERTEX_3,
		MAP2_VERTEX_4        = GL_MAP2_VERTEX_4,
		NORMALIZE            = GL_NORMALIZE,
		POINT_SMOOTH         = GL_POINT_SMOOTH,
		POLYGON_OFFSET_FILL  = GL_POLYGON_OFFSET_FILL,
		POLYGON_OFFSET_LINE  = GL_POLYGON_OFFSET_LINE,
		POLYGON_OFFSET_POINT = GL_POLYGON_OFFSET_POINT,
		POLYGON_SMOOTH       = GL_POLYGON_SMOOTH,
		POLYGON_STIPPLE      = GL_POLYGON_STIPPLE,
		SCISSOR_TEST         = GL_SCISSOR_TEST,
		STENCIL_TEST         = GL_STENCIL_TEST,
		TEXTURE_1D           = GL_TEXTURE_1D,
		TEXTURE_2D           = GL_TEXTURE_2D,
		TEXTURE_GEN_Q        = GL_TEXTURE_GEN_Q,
		TEXTURE_GEN_R        = GL_TEXTURE_GEN_R,
		TEXTURE_GEN_S        = GL_TEXTURE_GEN_S,
		TEXTURE_GEN_T        = GL_TEXTURE_GEN_T
	};
};

// glEnable
inline void enable(Capability::Enum cap)
{
	::glEnable(cap);
	assert(GL_NO_ERROR == ::glGetError()
		&& "can't enable specified capability");
}

// glDisable
inline void disable(Capability::Enum cap)
{
	::glDisable(cap);
	assert(GL_NO_ERROR == ::glGetError()
		&& "can't disable specified capability");
}


//////////////////////////////////////////////////////////////////////////
// GroupMode enum
struct GroupMode {
	enum Enum {
		POINTS         = GL_POINTS,
		LINES          = GL_LINES,
		LINE_LOOP      = GL_LINE_LOOP,
		LINE_STRIP     = GL_LINE_STRIP,
		TRIANGLES      = GL_TRIANGLES,
		TRIANGLE_STRIP = GL_TRIANGLE_STRIP,
		TRIANGLE_FAN   = GL_TRIANGLE_FAN,
		QUADS          = GL_QUADS,
		QUAD_STRIP     = GL_QUAD_STRIP,
		POLYGON        = GL_POLYGON
	};
};


//////////////////////////////////////////////////////////////////////////
// glBegin/glEnd wrapper
template<GroupMode::Enum MODE>
class Group: public Canvas {
public:
	Group()
	{
		assert(GL_NO_ERROR == ::glGetError()
			&& "GL error before glBegin()");
		::glBegin(MODE);
	}

	~Group()
	{
		::glEnd();
		assert(GL_NO_ERROR == ::glGetError()
			&& "GL error after glEnd()");
	}

private:
	using Canvas::rect; // rect() not allowed
};

typedef Group<GroupMode::POINTS>         GroupPoints;
typedef Group<GroupMode::LINES>          GroupLines;
typedef Group<GroupMode::LINE_LOOP>      GroupLineLoop;
typedef Group<GroupMode::LINE_STRIP>     GroupLineStrip;
typedef Group<GroupMode::TRIANGLES>      GroupTriangles;
typedef Group<GroupMode::TRIANGLE_STRIP> GroupTriangleStrip;
typedef Group<GroupMode::TRIANGLE_FAN>   GroupTriangleFan;
typedef Group<GroupMode::QUADS>          GroupQuads;
typedef Group<GroupMode::QUAD_STRIP>     GroupQuadStrip;
typedef Group<GroupMode::POLYGON>        GroupPolygon;


//////////////////////////////////////////////////////////////////////////
// MatrixMode enum
struct MatrixMode {
	enum Enum {
		MODELVIEW  = GL_MODELVIEW,
		PROJECTION = GL_PROJECTION,
		TEXTURE    = GL_TEXTURE
	};
};

// glMatrixMode - set new matrix mode
inline void matrixMode(MatrixMode::Enum mode)
{
	::glMatrixMode(mode);
	assert(GL_NO_ERROR == ::glGetError()
		&& "Can't set new matrix mode");
}

// get current matrix mode
inline MatrixMode::Enum matrixMode()
{
	Array<GLint, 1> params;
	::glGetIntegerv(GL_MATRIX_MODE, &params[0]);
	assert(GL_NO_ERROR == ::glGetError()
		&& "Can't get current matrix mode");

	return MatrixMode::Enum(params[0]);
}


//////////////////////////////////////////////////////////////////////////
// get ModelView matrix
inline void get_modelview_matrix(Array<GLdouble, 16> &m)
{
	::glGetDoublev(GL_MODELVIEW_MATRIX, &m[0]);
	assert(GL_NO_ERROR == ::glGetError()
		&& "Can't get ModelView matrix");
}
inline void get_modelview_matrix(Array<GLfloat, 16> &m)
{
	::glGetFloatv(GL_MODELVIEW_MATRIX, &m[0]);
	assert(GL_NO_ERROR == ::glGetError()
		&& "Can't get ModelView matrix");
}


//////////////////////////////////////////////////////////////////////////
// get Projection matrix
inline void get_projection_matrix(Array<GLdouble, 16> &m)
{
	::glGetDoublev(GL_PROJECTION_MATRIX, &m[0]);
	assert(GL_NO_ERROR == ::glGetError()
		&& "Can't get Projection matrix");
}
inline void get_projection_matrix(Array<GLfloat, 16> &m)
{
	::glGetFloatv(GL_PROJECTION_MATRIX, &m[0]);
	assert(GL_NO_ERROR == ::glGetError()
		&& "Can't get Projection matrix");
}


//////////////////////////////////////////////////////////////////////////
// get Texture matrix
inline void get_texture_matrix(Array<GLdouble, 16> &m)
{
	::glGetDoublev(GL_TEXTURE_MATRIX, &m[0]);
	assert(GL_NO_ERROR == ::glGetError()
		&& "Can't get Texture matrix");
}
inline void get_texture_matrix(Array<GLfloat, 16> &m)
{
	::glGetFloatv(GL_TEXTURE_MATRIX, &m[0]);
	assert(GL_NO_ERROR == ::glGetError()
		&& "Can't get Texture matrix");
}


//////////////////////////////////////////////////////////////////////////
// get Viewport {x, y, w, h}
inline void get_viewport(Array<GLint, 4> &vp)
{
	::glGetIntegerv(GL_VIEWPORT, &vp[0]);
	assert(GL_NO_ERROR == ::glGetError()
		&& "Can't get Viewport");
}


//////////////////////////////////////////////////////////////////////////
// glPushMatrix/glPopMatrix wrapper
class MatrixBackup {
public:
	MatrixBackup()
	{
		::glPushMatrix();
		assert(GL_NO_ERROR == ::glGetError()
			&& "Can't push current matrix");
	}

	~MatrixBackup()
	{
		::glPopMatrix();
		assert(GL_NO_ERROR == ::glGetError()
			&& "Can't pop current matrix");
	}
};


//////////////////////////////////////////////////////////////////////////
// glLoadIdentity
inline void load_identity()
{
	::glLoadIdentity();
	assert(GL_NO_ERROR == ::glGetError()
		&& "Can't load identity matrix");
}


//////////////////////////////////////////////////////////////////////////
// glLoadMatrix
inline void load_matrix(const Array<GLdouble, 16> &m)
{
	::glLoadMatrixd(&m[0]);
	assert(GL_NO_ERROR == ::glGetError()
		&& "Can't load new matrix");
}
inline void load_matrix(const Array<GLfloat, 16> &m)
{
	::glLoadMatrixf(&m[0]);
	assert(GL_NO_ERROR == ::glGetError()
		&& "Can't load new matrix");
}


//////////////////////////////////////////////////////////////////////////
// glMultMatrix
inline void mult_matrix(const Array<GLdouble, 16> &m)
{
	::glMultMatrixd(&m[0]);
	assert(GL_NO_ERROR == ::glGetError()
		&& "Can't mult current matrix");
}
inline void mult_matrix(const Array<GLfloat, 16> &m)
{
	::glMultMatrixf(&m[0]);
	assert(GL_NO_ERROR == ::glGetError()
		&& "Can't mult current matrix");
}


//////////////////////////////////////////////////////////////////////////
// glTranslate
inline void translate(GLdouble x, GLdouble y, GLdouble z)
{
	::glTranslated(x, y, z);
	assert(GL_NO_ERROR == ::glGetError()
		&& "Can't translate current matrix");
}
inline void translate(GLfloat x, GLfloat y, GLfloat z)
{
	::glTranslatef(x, y, z);
	assert(GL_NO_ERROR == ::glGetError()
		&& "Can't translate current matrix");
}


//////////////////////////////////////////////////////////////////////////
// glRotate
inline void rotate(GLdouble angle, GLdouble x, GLdouble y, GLdouble z)
{
	::glRotated(angle, x, y, z);
	assert(GL_NO_ERROR == ::glGetError()
		&& "Can't rotate current matrix");
}
inline void rotate(GLfloat angle, GLfloat x, GLfloat y, GLfloat z)
{
	::glRotatef(angle, x, y, z);
	assert(GL_NO_ERROR == ::glGetError()
		&& "Can't rotate current matrix");
}


//////////////////////////////////////////////////////////////////////////
// glScale
inline void scale(GLdouble x, GLdouble y, GLdouble z)
{
	::glScaled(x, y, z);
	assert(GL_NO_ERROR == ::glGetError()
		&& "Can't scale current matrix");
}
inline void scale(GLfloat x, GLfloat y, GLfloat z)
{
	::glScalef(x, y, z);
	assert(GL_NO_ERROR == ::glGetError()
		&& "Can't scale current matrix");
}


//////////////////////////////////////////////////////////////////////////
// Pen attributes - contains line style: color, width and stipple
class Pen {
public:
	enum Style {
		STYLE_SOLID,
		STYLE_DASH,
		STYLE_DOT,
		STYLE_DASH_DOT,
		STYLE_DASH_DOTDOT
	};

public:
	Pen();
	Pen(const Color c, float w);
	Pen(const Color c, float w, Style style);
	Pen(const Color c, float w, GLint sfactor, GLushort spattern);

public:
	void select() const;

public:
	const Color& color() const { return m_color; }
	float width() const { return m_width; }

	GLushort pattern() const { return m_pattern; }
	GLint factor() const { return m_factor; }

private:
	Color m_color;
	float m_width;

	// stipple
	GLushort m_pattern;
	GLint    m_factor;
};


//////////////////////////////////////////////////////////////////////////
// Font
class Font: private NonCopyable {
public:
	Font();
	~Font();

public:
	void select() const;

public:
	GLuint base() const;

	float max_ascent() const;
	float max_descent() const;
	float max_height() const;

public:
	typedef GLYPHMETRICSFLOAT Glyph;
	const Glyph& glyph(size_t ch) const;

	float text_ascent(const char *text) const;
	float text_descent(const char *text) const;
	float text_height(const char *text) const;
	float text_width(const char *text) const;

private:
	enum { N_CHARS = 128 };

private:
	Glyph m_glyphs[N_CHARS];
	float m_max_ascent;
	float m_max_descent;
	GLuint m_base;
};


//////////////////////////////////////////////////////////////////////////
// TextAlign
struct TextAlign {
	enum Enum {
		// horizontal
		LEFT    = 0x01,
		RIGHT   = 0x02,
		HCENTER = 0x04,

		// vertical
		TOP      = 0x10,
		BOTTOM   = 0x20,
		VCENTER  = 0x40,
		BASELINE = 0x80
	};
};

// combine two TextAlign
inline TextAlign::Enum operator|(const TextAlign::Enum a, const TextAlign::Enum b)
{
	return TextAlign::Enum(GLuint(a) | GLuint(b));
}


//////////////////////////////////////////////////////////////////////////
// drawText
void drawText(const Font &font, const char *text,
	double x, double y, double x_scale, double y_scale,
		double angle, TextAlign::Enum align);


	} // GL namespace
} // omni namespace

#endif // __OMNI_GL_PLUS_H_
