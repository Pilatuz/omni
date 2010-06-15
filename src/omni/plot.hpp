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
#ifndef __OMNI_PLOT_H_
#define __OMNI_PLOT_H_

#include <omni/defs.hpp>

#if !defined(_WIN32_WINNT)
#	define _WIN32_WINNT 0x0500
#endif

#include <Windows.h>
#include <GdiPlus.h>

#include <memory>
#include <string>
#include <vector>

namespace omni
{
	// forward declarations
	namespace plot
	{
		class Plotter;
			class Window;
				class XYPlotter;
				// class PolarPlotter;
		class Object;
			class LineGraph;
		class Marker;
	}


	// misc
	namespace plot
	{

//////////////////////////////////////////////////////////////////////////
/// @brief The real value type.
typedef Gdiplus::REAL Real;


//////////////////////////////////////////////////////////////////////////
/// @brief The size class.
/**
		This class stores the 2D size (X and Y).
*/
class Size
{
public:
	Size();
	Size(Real dx,
		Real dy);

public:
	Real X() const;
	Real Y() const;

public:
	Size& operator+=(const Size &sz);
	Size& operator-=(const Size &sz);

public:

	/// @brief Convert to GDI+ size.
	operator const Gdiplus::SizeF() const
	{
		return Gdiplus::SizeF(m_dx, m_dy);
	}

private:
	Real m_dx; ///< @brief The X size.
	Real m_dy; ///< @brief The Y size.
};

const Size operator+(const Size &a, const Size &b);
const Size operator-(const Size &a, const Size &b);
const Size operator-(const Size &sz);


//////////////////////////////////////////////////////////////////////////
/// @brief The point class.
/**
		This class stores the 2D position (X and Y).
*/
class Point
{
public:
	Point();
	Point(Real x,
		Real y);

public:
	Real X() const;
	Real Y() const;

public:
	void offset(Real dx, Real dy);
	void offset(const Size &sz);

	Point& operator+=(const Size &sz);
	Point& operator-=(const Size &sz);

public:

	/// @brief Convert to GDI+ point.
	operator const Gdiplus::PointF() const
	{
		return Gdiplus::PointF(m_x, m_y);
	}

private:
	Real m_x; ///< @brief The X coordinate.
	Real m_y; ///< @brief The Y coordinate.
};

const Point operator+(const Point &pt, const Size &sz);
const Point operator-(const Point &pt, const Size &sz);
const Size operator-(const Point &a, const Point &b);


///////////////////////////////////////////////////////////////////////////////
/// @brief The rectangle class.
/**
		This class stores the 2D rectangle coordinates.
*/
class Rect {
public:
	Rect();
	Rect(const Point &pt_min, const Size &rc_size);
	Rect(const Point &pt_min, const Point &pt_max);

public:
	Real Xmin() const;
	Real Xmax() const;
	Real Xsize() const;

	Real Ymin() const;
	Real Ymax() const;
	Real Ysize() const;

public:
	const Point center() const;

	void normalize();
	bool empty() const;
	bool is_null() const;
	bool contains(const Point &pt) const;
	bool contains(const Rect &rc) const;

	void inflate(Real dx, Real dy);
	void inflate(const Size &sz);
	void inflate(Real dx_min, Real dy_min,
		Real dx_max, Real dy_max);
	void inflate(const Rect &rc);

	void deflate(Real dx, Real dy);
	void deflate(const Size &sz);
	void deflate(Real dx_min, Real dy_min,
		Real dx_max, Real dy_max);
	void deflate(const Rect &rc);

	void offset(Real dx, Real dy);
	void offset(const Size &sz);

public:
	Rect& operator|=(const Rect &rc);
	Rect& operator|=(const Point &pt);
	Rect& operator&=(const Rect &rc);

	Rect& operator+=(const Size &sz);
	Rect& operator-=(const Size &sz);

public:

	/// @brief Convert to GDI+ rectangle.
	operator const Gdiplus::RectF() const
	{
		return Gdiplus::RectF(m_Xmin, m_Ymin,
			m_Xmax-m_Xmin, m_Ymax-m_Ymin);
	}

private:
	Real m_Xmin; ///< @brief The minimum X coordinate.
	Real m_Ymin; ///< @brief The minimum Y coordinate.
	Real m_Xmax; ///< @brief The maximum X coordinate.
	Real m_Ymax; ///< @brief The maximum Y coordinate.
};


//////////////////////////////////////////////////////////////////////////
/// @brief The color type.
typedef Gdiplus::Color Color;


//////////////////////////////////////////////////////////////////////////
/// @brief The canvas type.
typedef Gdiplus::Graphics Canvas;

	} // misc


	// Plotter & Object
	namespace plot
	{

//////////////////////////////////////////////////////////////////////////
/// @brief The graphics object.
/**
	TODO: Object description

	The "full" rectangle is the minimum rectangle that contains object.
*/
class Object:
	private NonCopyable
{
	friend class Plotter;
public:
	Object();
	virtual ~Object();

public:
	bool attach(Plotter &plotter, size_t level);
	bool detach(Plotter &plotter);

public:
	virtual void draw(const Plotter &plotter, Canvas &canvas) const;
	virtual bool hit_test(const Point &pt, Real eps) const;
	virtual const Rect full() const;

private:
	typedef std::vector<Plotter*> PlotterList;
	PlotterList m_plotters;
};


//////////////////////////////////////////////////////////////////////////
/// @brief The Plotter base class.
/**
	TODO: Plotter description

	TODO: draw levels description.

	TODO: "world", "view" coordinate systems (co-systems).
*/
class Plotter:
	private NonCopyable
{
public:
	Plotter();
	virtual ~Plotter();

public:
	bool attach(Object &obj, size_t level);
	bool detach(Object &obj);

public:
	Object* hit_test(const Point &pt, Real eps);

public:
	const Rect& world() const;
	const Rect& wish() const;
	const Rect& full() const;
	const Rect& view() const;

public:
	//const Real v2w_x(const Real &x) const;
	//const Real w2v_x(const Real &x) const;
	//const Real v2w_y(const Real &y) const;
	//const Real w2v_y(const Real &y) const;

	const Point v2w(const Point &pt) const;
	const Point w2v(const Point &pt) const;

	const Size v2w(const Size &sz) const;
	const Size w2v(const Size &sz) const;

	const Rect v2w(const Rect &rc) const;
	const Rect w2v(const Rect &rc) const;

public:
	void setColor(const Color &color);
	const Color& getColor() const;

public:
	void setFixedAspectRatio(bool fixed_AR);
	bool getFixedAspectRatio() const;

public:
	void show(const Rect &wr);
	void setViewport(Real x,
		Real y, Real w, Real h);

	void draw(Canvas &canvas) const;
	void update();

protected:
	virtual void on_world_changed();
	virtual void on_full_changed();
	virtual void on_view_changing(Real &x,
		Real &y, Real &w, Real &h);
	virtual void on_view_changed();

	virtual void on_draw_start(Canvas &canvas) const;
	virtual void on_draw_stop(Canvas &canvas) const;

private:
	void update_world();

private:
	typedef std::vector<Object*> Layer;
	typedef std::vector<Layer> LayerList;
	LayerList m_layers; ///< @brief The list of the layers.

private:
	Rect m_world; ///< @brief The current "world" rectangle.
	Rect m_wish;  ///< @brief The current "wish" rectangle.
	Rect m_full;  ///< @brief The current "full" rectangle.
	Rect m_view;  ///< @brief The current "view" rectangle.

private:
	bool m_fixed_aspect_ratio; ///< @brief The "fixed aspect ratio" flag.
	Color m_color; ///< @brief The main (background) color.
};

	} // Plotter & Object


	// Window
	namespace plot
	{

//////////////////////////////////////////////////////////////////////////
/// @brief The child window for the plotter.
/**
		To enable zooming feature the parent window should forward
	the WM_MOUSEWHEEL message. For example, like this:

@code
BOOL MainFrame::PreTranslateMessage(MSG* pMsg)
{
	if (inherited::PreTranslateMessage(pMsg))
		return TRUE;

	if (WM_MOUSEWHEEL == pMsg->message)
	{
		if (HWND hWnd = ::WindowFromPoint(pMsg->pt))
			pMsg->hwnd = hWnd;
	}

	return FALSE;
}
@endcode
*/
class Window
	: public Plotter
{
	typedef Plotter inherited;
public:
	Window(HWND parent, DWORD style,
		DWORD ex_style);
	~Window();

public:
	HWND handle() const;

public:
	void redraw(bool force) const;

protected:
	virtual void on_world_changed();
	virtual void on_full_changed();

private:
	Rect full_and_wish() const;
	void update_hscroll();
	void update_vscroll();

public:
	void setScroll(bool horz, bool vert);
	bool isHScroll() const;
	bool isVScroll() const;

	void enableMoving(bool enabled);
	bool isMovingEnabled() const;

	void enableZooming(bool enabled);
	bool isZoomingEnabled() const;

public:
	void offsetWorld(Real dx, Real dy);
	void scaleWorld(Real dx, Real dy, const Point &ref_w);
	void scaleWorld(Real dx, Real dy);

protected:
	virtual bool on_message(UINT msg,
		WPARAM wparam, LPARAM lparam,
			LRESULT &result);

private:
	void on_create(HWND hWnd);
	void on_destroy();

private:
	void on_size(int cx, int cy);
	void on_paint();

	void on_hscroll(int request);
	void on_vscroll(int request);

private:
	bool on_mouse_move(int vkeys, int x, int y);
	bool on_mouse_wheel(int delta, int vkeys, int x, int y);
	bool on_Lbtn_down(int vkeys, int x, int y);
	bool on_Lbtn_up(int vkeys, int x, int y);
	bool on_Lbtn_2click(int vkeys, int x, int y);
	bool on_Rbtn_down(int vkeys, int x, int y);
	bool on_Rbtn_up(int vkeys, int x, int y);
	bool on_Rbtn_2click(int vkeys, int x, int y);

private:
	class Class;
	friend class Class;

private: // move tool
	bool m_moveEnabled;
	bool m_moveActive;
	Point m_movePoint;
	Rect m_moveWish;

private: // zoom tool
	bool m_zoomEnabled;
	bool m_zoomActive;

private:
	HWND m_handle; ///< @brief The window handle.
	std::auto_ptr<Gdiplus::Bitmap> m_pbmp; ///< @brief The back buffer for drawing.
};

	} // Window


	// Axis
	namespace plot
	{

//////////////////////////////////////////////////////////////////////////
/// @brief The basic axis.
/**
	TODO: axis properties
	TODO: text labels (offset and scale)
*/
class Axis
{
public:
	Axis();
	virtual ~Axis();

public: // minor lines
	void setMinorPen(const Gdiplus::Pen &pen);
	const Gdiplus::Pen* getMinorPen() const;
	      Gdiplus::Pen* getMinorPen();

	void setMinorStep(Real step);
	Real getMinorStep() const;

	void setMinorAutoStep(bool enabled);
	bool getMinorAutoStep() const;

	void setMinorAutoStepHint(Real hint);
	Real getMinorAutoStepHint() const;

public: // major lines
	void setMajorPen(const Gdiplus::Pen &pen);
	const Gdiplus::Pen* getMajorPen() const;
	      Gdiplus::Pen* getMajorPen();

	void setMajorStep(Real step);
	Real getMajorStep() const;

	void setMajorAutoStep(bool enabled);
	bool getMajorAutoStep() const;

	void setMajorAutoStepHint(Real hint);
	Real getMajorAutoStepHint() const;

public: // colors
	void setBackColor(const Color &color);
	const Color& getBackColor() const;

	void setTextColor(const Color &color);
	const Color& getTextColor() const;

public: // axis name and size
	void setAxisName(const wchar_t *name);
	const wchar_t* getAxisName() const;

	void setTextSize(Real size);
	Real getTextSize() const;

public: // text format
	void setTextFormat(const wchar_t *format);
	const wchar_t* getTextFormat() const;

	void setTextOffset(Real offset);
	Real getTextOffset() const;

	void setTextScale(Real scale);
	Real getTextScale() const;

	Real transform(Real x) const;

protected:
	void update(Real wrange,
		Real vrange);

	static Real auto_step(Real wrange,
		Real vrange, Real vstep);

private:
	std::auto_ptr<Gdiplus::Pen> m_minorLine; ///< @brief The minor line pen.
	Real m_minorStep;                     ///< @brief The minor line step.
	bool m_minorAutoStep;                 ///< @brief The minor line "auto step" flag.
	Real m_minorAutoStepHint;             ///< @brief The minor line auto step hint.

private:
	std::auto_ptr<Gdiplus::Pen> m_majorLine; ///< @brief The major line pen.
	Real m_majorStep;                     ///< @brief The major line step.
	bool m_majorAutoStep;                 ///< @brief The major line "auto step" flag.
	Real m_majorAutoStepHint;             ///< @brief The major line auto step hint.

private:
	Color m_backColor;   ///< @brief The background color.
	Color m_textColor;   ///< @brief The text color.

private:
	std::wstring m_axisName; ///< @brief The axis name.
	Real       m_textSize; ///< @brief The axis name text size.

private:
	std::wstring m_textFormat; ///< @brief The text printf()-like format.
	Real m_textOffset;       ///< @brief The text values offset.
	Real m_textScale;        ///< @brief The text values scale.
};

	} // Axis


	// XYPlotter
	namespace plot
	{

//////////////////////////////////////////////////////////////////////////
/// @brief The 2D plotter.
class XYPlotter
	: public Window
{
	typedef Window inherited;
public:
	XYPlotter(HWND parent, DWORD style,
		DWORD ex_style);
	~XYPlotter();

public:
	/// @brief The draw levels.
	enum Level
	{
		LEVEL_GRID,
		LEVEL_GRAPH,
		LEVEL_TEXT
	};

	void attach(Object &obj);
	void attach(Object &obj, Level level);
	void detach(Object &obj);

public:
	const Axis& Xaxis() const;
	      Axis& Xaxis();

	const Axis& Yaxis() const;
	      Axis& Yaxis();

private:
	virtual void on_world_changed();
	virtual void on_full_changed();
	virtual void on_view_changing(Real &x,
		Real &y, Real &w, Real &h);

private:
	class XAxis;
	class YAxis;

	std::auto_ptr<XAxis> m_Xaxis;
	std::auto_ptr<YAxis> m_Yaxis;
};

	} // XYPlotter


#if 0
	// PolarPlotter
	namespace plot
	{

//////////////////////////////////////////////////////////////////////////
// PolarPlotter
class PolarPlotter: public Window {
	typedef Window inherited;
public:
	PolarPlotter(DWORD style,
		HWND parent);
	~PolarPlotter();

public:
	void attach(Object &obj);
	void attach(Object &obj, size_t level);
	void detach(Object &obj);

	enum Level {
		LEVEL_GRID,
		LEVEL_GRAPH,
		LEVEL_TEXT
	};

public:
	const Axis& radialAxis() const;
	      Axis& radialAxis();

	const Axis& angularAxis() const;
	      Axis& angularAxis();

private:
	virtual void on_world_changed();
	virtual void on_full_changed();

private:
	class RadialAxis;
	class AngularAxis;

	RadialAxis  *m_radialAxis;
	AngularAxis *m_angularAxis;
};

	} // PolarPlotter
#endif


	// Markers
	namespace plot
	{

//////////////////////////////////////////////////////////////////////////
/// @brief The basic marker.
class Marker:
	private NonCopyable
{
public:
	Marker();

protected:
	Marker(const Marker &other);

public:
	void setPen(const Gdiplus::Pen &pen);
	const Gdiplus::Pen* getPen() const;
	      Gdiplus::Pen* getPen();

	void setBrush(const Gdiplus::Brush &brush);
	const Gdiplus::Brush* getBrush() const;
	      Gdiplus::Brush* getBrush();

	void setSize(Real size);
	Real getSize() const;

public:
	virtual void start(const Plotter &plotter, Canvas &canvas) const;
	virtual void draw(const Plotter &plotter, Canvas &canvas, Real x, Real y) const;
	virtual void finish(const Plotter &plotter, Canvas &canvas) const;

	virtual Marker* clone() const = 0;

private:
	std::auto_ptr<Gdiplus::Pen> m_pen;
	std::auto_ptr<Gdiplus::Brush> m_brush;
	Real m_size;
};


//////////////////////////////////////////////////////////////////////////
/// @brief The "square" marker.
class SquareMarker:
	public Marker
{
	typedef Marker inherited;

public:
	SquareMarker();

private:
	SquareMarker(const SquareMarker &other);

public:
	virtual void start(const Plotter &plotter, Canvas &canvas) const;
	virtual void draw(const Plotter &plotter, Canvas &canvas, Real x, Real y) const;
	virtual void finish(const Plotter &plotter, Canvas &canvas) const;

	virtual Marker* clone() const;
};


//////////////////////////////////////////////////////////////////////////
/// @brief The "circle" marker
class CircleMarker:
	public Marker
{
	typedef Marker inherited;

public:
	CircleMarker();

private:
	CircleMarker(const CircleMarker &other);

public:
	virtual void start(const Plotter &plotter, Canvas &canvas) const;
	virtual void draw(const Plotter &plotter, Canvas &canvas, Real x, Real y) const;
	virtual void finish(const Plotter &plotter, Canvas &canvas) const;

	virtual Marker* clone() const;
};


//////////////////////////////////////////////////////////////////////////
/// @brief The "star" marker.
class StarMarker:
	public Marker
{
	typedef Marker inherited;

public:
	explicit StarMarker(size_t Nanlges = 5);

private:
	StarMarker(const StarMarker &other);

public:
	virtual void start(const Plotter &plotter, Canvas &canvas) const;
	virtual void draw(const Plotter &plotter, Canvas &canvas, Real x, Real y) const;
	virtual void finish(const Plotter &plotter, Canvas &canvas) const;

	virtual Marker* clone() const;

private:
	void updateFigure() const;

private:
	mutable Gdiplus::GraphicsPath m_figure;
	mutable Real m_figureSize;
	size_t m_Nangles;
};

	} // Markers


	// LineGraph
	namespace plot
	{

//////////////////////////////////////////////////////////////////////////
// Line Graph
class LineGraph:
	public Object
{
	typedef Object inherited;
public:
	LineGraph();
	virtual ~LineGraph();

public:
	void setPen(const Gdiplus::Pen &pen);
	const Gdiplus::Pen* getPen() const;
	      Gdiplus::Pen* getPen();

	void setMarker(const Marker &marker);
	const Marker* getMarker() const;
	      Marker* getMarker();

	void setSmoothCurve(bool smooth);
	bool isSmoothCurve() const;

public:
	void assign(size_t Npoints, const Point *XYs);
	void assign(size_t Npoints, const Real *Ys, const Real *Xs);
	void assign(size_t Npoints, const Real *Ys,
		Real Xstart = 0, Real Xstep = 1);

public:
	void push_back(const Point &pt);
	void push_back(Real x, Real y);
	void clear();

public:
	virtual void draw(const Plotter &plotter, Canvas &canvas) const;
	virtual bool hit_test(const Point &pt, Real eps) const;
	virtual const Rect full() const;

private:
	void update();

private:
	std::vector<Point> m_wpoints; ///< @brief The "world" points.
	mutable std::vector<Gdiplus::PointF> m_vpoints; ///< @brief The "view" points.

	std::auto_ptr<Marker> m_marker; ///< @brief The marker.
	std::auto_ptr<Gdiplus::Pen> m_pen; ///< @brief The pen.
	bool m_smoothCurve; ///< @brief Draw smooth curve.
	Rect m_full; ///< @brief The "full" rectangle.
};

	} // LineGraph

#if 0
	// HistGraph
	namespace plot
	{

//////////////////////////////////////////////////////////////////////////
// Hist Graph
class HistGraph: public Object {
	typedef Object inherited;

public:
	HistGraph();
	virtual ~HistGraph();

public:
	const GL::Pen& line() const;
	void set_line(const GL::Pen &pen);

	const GL::Color& color() const;
	void set_color(const GL::Color &c);

public:
	void assign(size_t N_points, const Real *Ys, const Real *Xs);
	void assign(size_t N_points, const Real *Ys,
		Real X_start = 0.0, Real X_step = 1.0);

	void set_zeroLevel(Real zero);
	void set_barWidth(Real width);

	void push_back(const Point &pt);
	void push_back(Real x, Real y);
	void clear();

public:
	virtual void draw(const Plotter &plotter, const omni::GL::Font&) const;
	virtual bool hit_test(const Point &pt, Real eps) const;
	virtual Rect full() const;

private:
	void update();

private:
	typedef GL::Array<GLdouble, 2> PointType;
	typedef std::vector<PointType> PointList;
	PointList m_points;
	Real m_zero_level;
	Real m_bar_width;

	GL::Color m_color;
	GL::Pen m_pen;
	Rect m_full;
};

	} // HistGraph
#endif

} // omni namespace

#endif // __OMNI_PLOT_H_
