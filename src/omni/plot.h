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

#if !defined(_WINDOWS_)
#	error windows.h must be included first
#endif // _WINDOWS_

// TODO: method to set "fixed_aspect_ratio" flag

#include <omni/GL+.h>

#include <memory>
#include <string>
#include <vector>

namespace omni
{
	// declarations
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
// Size class
class Size {
public:
	Size();
	Size(double x_size,
		double y_size);

public:
	double Xsize() const;
	double Ysize() const;

public:
	Size& operator+=(const Size &sz);
	Size& operator-=(const Size &sz);

private:
	double m_Xsize;
	double m_Ysize;
};

Size operator+(const Size &a, const Size &b);
Size operator-(const Size &a, const Size &b);


//////////////////////////////////////////////////////////////////////////
// Point class
class Point {
public:
	Point();
	Point(double x,
		double y);

public:
	double X() const;
	double Y() const;

public:
	void offset(double dx, double dy);
	void offset(const Size &sz);

	Point& operator+=(const Size &sz);
	Point& operator-=(const Size &sz);

private:
	double m_X;
	double m_Y;
};

Point operator+(const Point &pt, const Size &sz);
Point operator-(const Point &pt, const Size &sz);
Size operator-(const Point &A, const Point &B);


///////////////////////////////////////////////////////////////////////////////
// Rect class
class Rect {
public:
	Rect();
	Rect(const Point &pt_min, const Size &rc_size);
	Rect(const Point &pt_min, const Point &pt_max);

public:
	double Xmin() const;
	double Xmax() const;
	double Xsize() const;

	double Ymin() const;
	double Ymax() const;
	double Ysize() const;

public:
	Point center() const;

	void normalize();
	bool empty() const;
	bool contain(const Point &pt) const;
	bool contain(const Rect &rc) const;

	void inflate(double dx, double dy);
	void inflate(const Size &sz);
	void inflate(double dx_min, double dy_min,
		double dx_max, double dy_max);
	void inflate(const Rect &rc);

	void deflate(double dx, double dy);
	void deflate(const Size &sz);
	void deflate(double dx_min, double dy_min,
		double dx_max, double dy_max);
	void deflate(const Rect &rc);

	void offset(double dx, double dy);
	void offset(const Size &sz);

public:
	Rect& operator|=(const Rect &rc);
	Rect& operator|=(const Point &pt);
	Rect& operator&=(const Rect &rc);

	Rect& operator+=(const Size &sz);
	Rect& operator-=(const Size &sz);
	Rect& operator+=(const Rect &rc);
	Rect& operator-=(const Rect &rc);

private:
	double m_Xmin;
	double m_Ymin;
	double m_Xmax;
	double m_Ymax;
};

	} // misc


	// Plotter & Object
	namespace plot
	{

//////////////////////////////////////////////////////////////////////////
// Plotter class
class Plotter: private NonCopyable {
public:
	Plotter();
	virtual ~Plotter();

public:
	void attach(Object &obj, size_t level);
	void detach(Object &obj);

public:
	Object* hit_test(const Point &pt, double eps);

public:
	const Rect& world() const;
	const Rect& wish() const;
	const Rect& full() const;
	const Rect& view() const;

public:
	Point v2w(const Point &pt) const;
	Point w2v(const Point &pt) const;
	Size v2w(const Size &sz) const;
	Size w2v(const Size &sz) const;
	Rect v2w(const Rect &rc) const;
	Rect w2v(const Rect &rc) const;

public:
	const GL::Color& background() const;
	void set_background(const GL::Color &color);

protected:
	void show(const Rect &wr);
	void set_viewport(int x,
		int y, int w, int h);

	void draw(const GL::Font &font) const;
	void update();

protected:
	virtual void on_world_changed();
	virtual void on_full_changed();

	virtual void on_draw_start() const;
	virtual void on_draw_stop() const;

private:
	void update_world();

private:
	typedef std::vector<Object*> Layer;
	typedef std::vector<Layer> LayerList;
	LayerList m_layers; // list of the layers

private:
	Rect m_world;
	Rect m_wish;
	Rect m_full;
	Rect m_view;

private:
	bool m_fixed_aspect_ratio;
	GL::Color m_color; // background color
};


//////////////////////////////////////////////////////////////////////////
// Object class
class Object: private NonCopyable {
	friend class Plotter;
public:
	Object();
	virtual ~Object();

public:
	void attach(Plotter &plotter, size_t level);
	void detach(Plotter &plotter);

public:
	virtual void draw(const Plotter &plotter, const GL::Font &font) const;
	virtual bool hit_test(const Point &pt, double eps) const;
	virtual Rect full() const;

private:
	typedef std::vector<Plotter*> PlotterList;
	PlotterList m_plotters;
};

	} // Plotter & Object


	// Window
	namespace plot
	{

//////////////////////////////////////////////////////////////////////////
// Window class - Plotter's child window
class Window: public Plotter {
	typedef Plotter inherited;

public:
	Window(DWORD style,
		HWND parent);
	~Window();

public:
	HWND handle() const;

public:
	void redraw(bool force) const;

	void show(const Rect &W);
	void update();

protected:
	virtual void on_world_changed();
	virtual void on_full_changed();

private:
	Rect full_and_wish() const;
	void update_HScroll();
	void update_VScroll();

public:
	bool is_Hscroll() const;
	bool is_Vscroll() const;

	void set_scroll(bool horz, bool vert);

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

	void on_Hscroll(int request);
	void on_Vscroll(int request);

private:
	class Class;
	friend class Class;

private:
	HWND m_handle;

	GL::Context *m_context;
	GL::Font *m_font;
};

	} // Window


	// Tools
	namespace plot
	{
/*
//////////////////////////////////////////////////////////////////////////
// Zoom Tool
class ZoomTool {
public:
	ZoomTool();

public:
	bool active() const;

public:
	void start(Window &wnd, int x, int y);
	void update(Window &wnd, int x, int y);
	void stop(Window &wnd, int x, int y, bool zoomIn);

	void zoom(Window &wnd, int x, int y, bool zoomIn);
	void cancel(Window &wnd);

	void draw();

private:
	bool m_active;
	Point m_ref_pt;
	Rect m_world;

	GL::Color m_color; // background color
	GL::Pen m_line;    // selection line
};


//////////////////////////////////////////////////////////////////////////
// Move Tool
class MoveTool {
public:
	MoveTool();

public:
	bool active() const;

public:
	void start(Window &wnd, int x, int y);
	bool update(Window &wnd, int x, int y);
	void stop(Window &wnd, int x, int y);

	void center(Window &wnd, int x, int y);
	void cancel(Window &wnd);

private:
	bool m_active;
	Point m_ref_pt;
	Rect m_world;
};
*/
	} // Tools


	// Axis
	namespace plot
	{

//////////////////////////////////////////////////////////////////////////
// Axis basic class
class Axis {
public:
	Axis();
	virtual ~Axis();

public: // minor lines
	const GL::Pen& minorLine() const;
	void set_minorLine(const GL::Pen &pen);

	double minorStep() const;
	void set_minorStep(double step);

	bool minorAutoStep() const;
	void set_minorAutoStep(bool enabled);

	double minorAutoStepHint() const;
	void set_minorAutoStepHint(double step_hint);

public: // major lines
	const GL::Pen& majorLine() const;
	void set_majorLine(const GL::Pen &pen);

	double majorStep() const;
	void set_majorStep(double step);

	bool majorAutoStep() const;
	void set_majorAutoStep(bool enabled);

	double majorAutoStepHint() const;
	void set_majorAutoStepHint(double step_hint);

public: // colors
	const GL::Color& backColor() const;
	void set_backColor(const GL::Color &color);

	const GL::Color& textColor() const;
	void set_textColor(const GL::Color &color);

public: // axis name and size
	const std::string& axisName() const;
	void set_axisName(const std::string &name);

	double textSize() const;
	void set_textSize(double size);

public: // text format
	const std::string& textFormat() const;
	void set_textFormat(const std::string &format);

	double textOffset() const;
	void set_textOffset(double offset);

	double textScale() const;
	void set_textScale(double scale);

	double transform(double x) const;

protected:
	void update(double world_range,
		double viewport_range);

	static double auto_step(double world_range,
		double viewport_range, double viewport_step);

private:
	GL::Pen m_minorLine;
	double  m_minorStep;
	bool    m_minorAutoStep;
	double  m_minorAutoStepHint;

private:
	GL::Pen m_majorLine;
	double  m_majorStep;
	bool    m_majorAutoStep;
	double  m_majorAutoStepHint;

private:
	GL::Color m_backColor;
	GL::Color m_textColor;

private:
	std::string m_axisName;
	double      m_textSize;

private:
	std::string m_textFormat;
	double m_textOffset;
	double m_textScale;
};

	} // Axis


	// XYPlotter
	namespace plot
	{

//////////////////////////////////////////////////////////////////////////
// XYPlotter
class XYPlotter: public Window {
	typedef Window inherited;
public:
	XYPlotter(DWORD style,
		HWND parent);
	~XYPlotter();

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
	const Axis& Xaxis() const;
	      Axis& Xaxis();

	const Axis& Yaxis() const;
	      Axis& Yaxis();

private:
	virtual void on_world_changed();
	virtual void on_full_changed();

private:
	class XAxis;
	class YAxis;

	XAxis *m_Xaxis;
	YAxis *m_Yaxis;
};

	} // XYPlotter


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


	// Markers
	namespace plot
	{

//////////////////////////////////////////////////////////////////////////
// Marker - the basic class
class Marker {
public:
	Marker(const GL::Color &c,
		const GL::Pen &pen, double s);

public:
	const GL::Color& color() const;
	const GL::Pen& line() const;
	double size() const;

	void set_color(const GL::Color &c);
	void set_line(const GL::Pen &pen);
	void set_size(double s);

public:
	virtual void start();
	virtual void draw();
	virtual void finish();

private:
	GL::Color m_color;
	GL::Pen m_line;
	double m_size;
};


//////////////////////////////////////////////////////////////////////////
// SquareMarker
class SquareMarker: public Marker {
	typedef Marker inherited;

public:
	SquareMarker(bool is_rhomb, const GL::Color &c);
	SquareMarker(bool is_rhomb, const GL::Color &c,
		const GL::Pen &pen);
	SquareMarker(bool is_rhomb, const GL::Color &c,
		const GL::Pen &pen, double s);

public:
	virtual void start();
	virtual void draw();
	virtual void finish();

private:
	GLuint m_list_base;
	bool m_is_rhomb;
};


//////////////////////////////////////////////////////////////////////////
// CircleMarker
class CircleMarker: public Marker {
	typedef Marker inherited;

public:
	explicit CircleMarker(const GL::Color &c);
	CircleMarker(const GL::Color &c,
		const GL::Pen &pen);
	CircleMarker(const GL::Color &c,
		const GL::Pen &pen, double s);

public:
	virtual void start();
	virtual void draw();
	virtual void finish();

private:
	GLuint m_list_base;
};


//////////////////////////////////////////////////////////////////////////
// StarMarker
class StarMarker: public Marker {
	typedef Marker inherited;

public:
	StarMarker(size_t Nanlges, const GL::Color &c);
	StarMarker(size_t Nanlges, const GL::Color &c,
		const GL::Pen &pen);
	StarMarker(size_t Nanlges, const GL::Color &c,
		const GL::Pen &pen, double s);

public:
	virtual void start();
	virtual void draw();
	virtual void finish();

private:
	GLuint m_list_base;
	size_t m_Nangles;
};

	} // Markers


	// LineGraph
	namespace plot
	{

//////////////////////////////////////////////////////////////////////////
// Line Graph
class LineGraph: public Object {
	typedef Object inherited;

public:
	LineGraph();
	virtual ~LineGraph();

public:
	const GL::Pen& line() const;
	void set_line(const GL::Pen &pen);

	const Marker* marker() const;
	      Marker* marker();
	void set_marker(Marker *m);

public:
	void assign(size_t N_points, const double *Ys, const double *Xs);
	void assign(size_t N_points, const double *Ys,
		double X_start = 0.0, double X_step = 1.0);

	void push_back(const Point &pt);
	void push_back(double x, double y);
	void clear();

public:
	virtual void draw(const Plotter &plotter, const omni::GL::Font&) const;
	virtual bool hit_test(const Point &pt, double eps) const;
	virtual Rect full() const;

private:
	void update();

private:
	typedef GL::Array<GLdouble, 2> PointType;
	typedef std::vector<PointType> PointList;
	PointList m_points;

	Marker *m_marker;
	GL::Pen m_pen;
	Rect m_full;
};

	} // LineGraph

} // omni namespace

#endif // __OMNI_PLOT_H_
