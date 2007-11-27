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

#include <omni/plot.hpp>
#include <omni/util.hpp>

#include <algorithm>

#include <math.h>


#include <WindowsX.h>
#include <tchar.h>


namespace omni
{

	// Size
	namespace plot
	{

//////////////////////////////////////////////////////////////////////////
/// @brief The default constructor.
/**
		This constructor initializes the X and the Y sizes by zero.
*/
Size::Size()
	: m_Xsize(0.0),
	  m_Ysize(0.0)
{}


//////////////////////////////////////////////////////////////////////////
/// @brief The main constructor.
/**
@param x_size The X size.
@param y_size The Y size.
*/
Size::Size(double x_size, double y_size)
	: m_Xsize(x_size), m_Ysize(y_size)
{}


//////////////////////////////////////////////////////////////////////////
/// @brief Get the X size.
/**
@return The X size.
*/
double Size::Xsize() const
{
	return m_Xsize;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Get the Y size.
/**
@return The Y size.
*/
double Size::Ysize() const
{
	return m_Ysize;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Increase.
/**
		This operator increases the current size.

@param sz The increasing size.
@return The self reference.
*/
Size& Size::operator+=(const Size &sz)
{
	m_Xsize += sz.Xsize();
	m_Ysize += sz.Ysize();

	return *this;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Decrease.
/**
		This operator decreases the current size.

@param sz The decreasing size.
@return The self reference.
*/
Size& Size::operator-=(const Size &sz)
{
	m_Xsize -= sz.Xsize();
	m_Ysize -= sz.Ysize();

	return *this;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Addition.
/**
@param a The first size.
@param b The second size.
@return The sum of two sizes.
@relates Size
*/
Size operator+(const Size &a, const Size &b)
{
	return Size(a.Xsize() + b.Xsize(),
		a.Ysize() + b.Ysize());
}


//////////////////////////////////////////////////////////////////////////
/// @brief Subtraction.
/**
@param a The first size.
@param b The second size.
@return The difference between two sizes.
@relates Size
*/
Size operator-(const Size &a, const Size &b)
{
	return Size(a.Xsize() - b.Xsize(),
		a.Ysize() - b.Ysize());
}


//////////////////////////////////////////////////////////////////////////
/// @brief Negation.
/**
@param sz The size.
@param The negated size.
@relates Size
*/
Size operator-(const Size &sz)
{
	return Size(-sz.Xsize(), -sz.Ysize());
}

	} // Size


	// Point
	namespace plot
	{

//////////////////////////////////////////////////////////////////////////
/// @brief The default constructor.
/**
		This constructor initializes the point coordinates by zero.
*/
Point::Point()
	: m_X(0.0),
	  m_Y(0.0)
{}


//////////////////////////////////////////////////////////////////////////
/// @brief The main constructor.
/**
@param x The X coordinate.
@param y The Y coordinate.
*/
Point::Point(double x, double y)
	: m_X(x), m_Y(y)
{}


//////////////////////////////////////////////////////////////////////////
/// @brief Get the X coordinate.
/**
@return The X coordinate.
*/
double Point::X() const
{
	return m_X;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Get the Y coordinate.
/**
@return The Y coordinate.
*/
double Point::Y() const
{
	return m_Y;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Offset the point.
/**
@param dx The X offset size.
@param dy The Y offset size.
*/
void Point::offset(double dx, double dy)
{
	m_X += dx;
	m_Y += dy;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Offset the point.
/**
@param sz The offset size.
*/
void Point::offset(const Size &sz)
{
	m_X += sz.Xsize();
	m_Y += sz.Ysize();
}


//////////////////////////////////////////////////////////////////////////
/// @brief Increase.
/**
@param sz The increasing size.
@return The self reference.
*/
Point& Point::operator+=(const Size &sz)
{
	m_X += sz.Xsize();
	m_Y += sz.Ysize();

	return *this;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Decrease.
/**
@param sz The decreasing size.
@return The self reference.
*/
Point& Point::operator-=(const Size &sz)
{
	m_X -= sz.Xsize();
	m_Y -= sz.Ysize();

	return *this;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Increase.
/**
@param pt The point.
@param sz The increasing size.
@return The new point.
*/
Point operator+(const Point &pt, const Size &sz)
{
	return Point(
		pt.X() + sz.Xsize(),
		pt.Y() + sz.Ysize()
	);
}


//////////////////////////////////////////////////////////////////////////
/// @brief Decrease.
/**
@param pt The point.
@param sz The decreasing size.
@return The new point.
*/
Point operator-(const Point &pt, const Size &sz)
{
	return Point(
		pt.X() - sz.Xsize(),
		pt.Y() - sz.Ysize()
	);
}


//////////////////////////////////////////////////////////////////////////
/// @brief Distance.
/**
@param A The first point.
@param B The second point.
@return The distance.
*/
Size operator-(const Point &A, const Point &B)
{
	return Size(
		B.X() - A.X(),
		B.Y() - A.Y()
	);
}

	} // Point


	// Rect
	namespace plot
	{

//////////////////////////////////////////////////////////////////////////
/// @brief The default constructor.
/**
		Constructs the empty rectangle.
*/
Rect::Rect()
	: m_Xmin(0.0), m_Xmax(0.0),
	  m_Ymin(0.0), m_Ymax(0.0)
{}


//////////////////////////////////////////////////////////////////////////
/// @brief The main constructor (point and size).
/**
@param pt The reference point.
@param sz The rectangle size.
*/
Rect::Rect(const Point &pt, const Size &sz)
	: m_Xmin(pt.X()),
	  m_Xmax(pt.X() + sz.Xsize()),
	  m_Ymin(pt.Y()),
	  m_Ymax(pt.Y() + sz.Ysize())
{}


//////////////////////////////////////////////////////////////////////////
/// @brief The main constructor (two points).
/**
@param pt_min The first reference point.
@param pt_max The second reference point.
*/
Rect::Rect(const Point &pt_min, const Point &pt_max)
	: m_Xmin(pt_min.X()),
	  m_Xmax(pt_max.X()),
	  m_Ymin(pt_min.Y()),
	  m_Ymax(pt_max.Y())
{}


//////////////////////////////////////////////////////////////////////////
/// @brief Get the minimum X coordinate.
/**
@return The minimum X coordinate.
*/
double Rect::Xmin() const
{
	return m_Xmin;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Get the maximum X coordinate.
/**
@return The maximum X coordinate.
*/
double Rect::Xmax() const
{
	return m_Xmax;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Get the X size.
/**
@return The X size (i.e. the rectangle's width).
*/
double Rect::Xsize() const
{
	return m_Xmax - m_Xmin;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Get the minimum Y coordinate.
/**
@return The minimum Y coordinate.
*/
double Rect::Ymin() const
{
	return m_Ymin;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Get the maximum Y coordinate.
/**
@return The maximum Y coordinate.
*/
double Rect::Ymax() const
{
	return m_Ymax;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Get the Y size.
/**
@return The Y size (i.e. the rectangle's height).
*/
double Rect::Ysize() const
{
	return m_Ymax - m_Ymin;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Get the center point.
/**
@return The center point.
*/
Point Rect::center() const
{
	return Point(
		(m_Xmin + m_Xmax) / 2,
		(m_Ymin + m_Ymax) / 2
	);
}


//////////////////////////////////////////////////////////////////////////
/// @brief Normalize rectangle.
/**
		This method normalizes the rectangle:
	the Xsize() and Ysize() methods will return positive values.
*/
void Rect::normalize()
{
	if (m_Xmax < m_Xmin)
	{
		double t = m_Xmin; // (!) std::swap
		m_Xmin = m_Xmax;
		m_Xmax = t;
	}

	if (m_Ymax < m_Ymin)
	{
		double t = m_Ymin; // (!) std::swap
		m_Ymin = m_Ymax;
		m_Ymax = t;
	}
}


//////////////////////////////////////////////////////////////////////////
/// @brief Is rectangle empty?
/**
		The rectangle is empty if the Xsize() and Ysize() returns zero.

@return @b true if rectangle is empty, @b false otherwise.
*/
bool Rect::empty() const
{
	return (m_Xmax <= m_Xmin)
		|| (m_Ymax <= m_Ymin);
}


//////////////////////////////////////////////////////////////////////////
/// @brief Is rectangle null?
/**
		The rectangle is null if all of Xmin(), Xmax(),
	Ymin() and Ymax() returns zero.

@return @b true if rectangle is null, @b false otherwise.
*/
bool Rect::is_null() const
{
	return (m_Xmin == 0.0)
		&& (m_Xmax == 0.0)
		&& (m_Ymin == 0.0)
		&& (m_Ymax == 0.0);
}


//////////////////////////////////////////////////////////////////////////
/// @brief Is the point inside?
/**
@param pt The point of interest.
@return @b true if the point is inside, @b false otherwise.
*/
bool Rect::contains(const Point &pt) const
{
	return m_Xmin <= pt.X() && pt.X() <= m_Xmax
		&& m_Ymin <= pt.Y() && pt.Y() <= m_Ymax;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Is the rectangle inside?
/**
@param rc The rectangle of interest.
@return @b true if the rectangle is inside, @b false otherwise.
*/
bool Rect::contains(const Rect &rc) const
{
	return m_Xmin <= rc.Xmin() && rc.Xmax() <= m_Xmax
		&& m_Ymin <= rc.Ymin() && rc.Ymax() <= m_Ymax;
}


// inflate rect
void Rect::inflate(double dx, double dy)
{
	m_Xmin -= dx;
	m_Xmax += dx;
	m_Ymin -= dy;
	m_Ymax += dy;
}

// inflate rect
void Rect::inflate(const Size &sz)
{
	m_Xmin -= sz.Xsize();
	m_Xmax += sz.Xsize();
	m_Ymin -= sz.Ysize();
	m_Ymax += sz.Ysize();
}

// inflate rect
void Rect::inflate(double dx_min, double dy_min, double dx_max, double dy_max)
{
	m_Xmin -= dx_min;
	m_Xmax += dx_max;
	m_Ymin -= dy_min;
	m_Ymax += dy_max;
}

// inflate rect
void Rect::inflate(const Rect &rc)
{
	m_Xmin -= rc.Xmin();
	m_Xmax += rc.Xmax();
	m_Ymin -= rc.Ymin();
	m_Ymax += rc.Ymax();
}

// deflate rect
void Rect::deflate(double dx, double dy)
{
	m_Xmin += dx;
	m_Xmax -= dx;
	m_Ymin += dy;
	m_Ymax -= dy;
}

// deflate rect
void Rect::deflate(const Size &sz)
{
	m_Xmin += sz.Xsize();
	m_Xmax -= sz.Xsize();
	m_Ymin += sz.Ysize();
	m_Ymax -= sz.Ysize();
}

// deflate rect
void Rect::deflate(double dx_min, double dy_min, double dx_max, double dy_max)
{
	m_Xmin += dx_min;
	m_Xmax -= dx_max;
	m_Ymin += dy_min;
	m_Ymax -= dy_max;
}

// deflate rect
void Rect::deflate(const Rect &rc)
{
	m_Xmin += rc.Xmin();
	m_Xmax -= rc.Xmax();
	m_Ymin += rc.Ymin();
	m_Ymax -= rc.Ymax();
}

// offset rect
void Rect::offset(double dx, double dy)
{
	m_Xmin += dx;
	m_Xmax += dx;
	m_Ymin += dy;
	m_Ymax += dy;
}


// offset rect
void Rect::offset(const Size &sz)
{
	m_Xmin += sz.Xsize();
	m_Xmax += sz.Xsize();
	m_Ymin += sz.Ysize();
	m_Ymax += sz.Ysize();
}


// Rect's union
Rect& Rect::operator|=(const Rect &rc)
{
	if (rc.Xmin() < m_Xmin) // min
		m_Xmin = rc.Xmin();
	if (m_Xmax < rc.Xmax()) // max
		m_Xmax = rc.Xmax();
	if (rc.Ymin() < m_Ymin) // min
		m_Ymin = rc.Ymin();
	if (m_Ymax < rc.Ymax()) // max
		m_Ymax = rc.Ymax();

	return *this;
}

// Rect's union
Rect& Rect::operator|=(const Point &pt)
{
	if (pt.X() < m_Xmin) // min
		m_Xmin = pt.X();
	if (m_Xmax < pt.X()) // max
		m_Xmax = pt.X();
	if (pt.Y() < m_Ymin) // min
		m_Ymin = pt.Y();
	if (m_Ymax < pt.Y()) // max
		m_Ymax = pt.Y();

	return *this;
}

// Rect's intersection
Rect& Rect::operator&=(const Rect &rc)
{
	if (m_Xmin < rc.Xmin()) // max
		m_Xmin = rc.Xmin();
	if (rc.Xmax() < m_Xmax) // min
		m_Xmax = rc.Xmax();
	if (m_Ymin < rc.Ymin()) // max
		m_Ymin = rc.Ymin();
	if (rc.Ymax() < m_Ymax) // min
		m_Ymax = rc.Ymax();

	return *this;
}

// offset
Rect& Rect::operator+=(const Size &sz)
{
	offset(sz.Xsize(), sz.Ysize());
	return *this;
}

// offset
Rect& Rect::operator-=(const Size &sz)
{
	offset(-sz.Xsize(), -sz.Ysize());
	return *this;
}

// inflate
Rect& Rect::operator+=(const Rect &rc)
{
	inflate(rc);
	return *this;
}

// deflate
Rect& Rect::operator-=(const Rect &rc)
{
	deflate(rc);
	return *this;
}

	} // Rect


	// Object
	namespace plot
	{

//////////////////////////////////////////////////////////////////////////
// Object construction
Object::Object()
{}


//////////////////////////////////////////////////////////////////////////
// Object destruction
Object::~Object()
{
	// detach all Plotters
	while (!m_plotters.empty())
		detach(*m_plotters.back());
}


//////////////////////////////////////////////////////////////////////////
// attach to the Plotter
void Object::attach(Plotter &plotter, size_t level)
{
	plotter.attach(*this, level);
}


//////////////////////////////////////////////////////////////////////////
// detach from the Plotter
void Object::detach(Plotter &plotter)
{
	plotter.detach(*this);
}


//////////////////////////////////////////////////////////////////////////
// draw the object
void Object::draw(const Plotter &plotter, const GL::Font &font) const
{
	// DO NOTHING
}


//////////////////////////////////////////////////////////////////////////
// hit testing
bool Object::hit_test(const Point &pt, double eps) const
{
	return false;
}


//////////////////////////////////////////////////////////////////////////
// there is no full rectangle
Rect Object::full() const
{
	return Rect();
}

	} // Object


	// Plotter
	namespace plot
	{

//////////////////////////////////////////////////////////////////////////
// Plotter construction
Plotter::Plotter()
	: m_fixed_aspect_ratio(true),
	  m_color(0.0f, 0.0f, 0.0f, 1.0f)
{}


//////////////////////////////////////////////////////////////////////////
// Plotter destruction
Plotter::~Plotter()
{
	// detach all objects
	LayerList::iterator i = m_layers.begin();
	LayerList::iterator ie = m_layers.end();

	for (; i != ie; ++i)
	{
		Layer &layer = *i;

		while (!layer.empty())
			detach(*layer.back());
	}
}


//////////////////////////////////////////////////////////////////////////
// attach the object
void Plotter::attach(Object &obj, size_t level)
{
	if (m_layers.size() <= level)
		m_layers.resize(level+1); // ensure that level is valid
	Layer &layer = m_layers[level];

	Layer::iterator found = std::find(
		layer.begin(), layer.end(), &obj);
	if (layer.end() == found)
	{
		obj.m_plotters.push_back(this);
		layer.push_back(&obj);
	}
}


//////////////////////////////////////////////////////////////////////////
// detach the object
void Plotter::detach(Object &obj)
{
	LayerList::reverse_iterator i = m_layers.rbegin();
	LayerList::reverse_iterator ie = m_layers.rend();

	for (; i != ie; ++i)
	{
		Layer &layer = *i;

		Layer::iterator found = std::find(
			layer.begin(), layer.end(), &obj);
		if (layer.end() != found)
		{
			layer.erase(found);

			Object::PlotterList::iterator found = std::find(
				obj.m_plotters.begin(), obj.m_plotters.end(), this);
			if (obj.m_plotters.end() != found)
				obj.m_plotters.erase(found);
		}
	}
}


//////////////////////////////////////////////////////////////////////////
// hit test
Object* Plotter::hit_test(const Point &pt, double eps)
{
	LayerList::reverse_iterator i = m_layers.rbegin();
	LayerList::reverse_iterator ie = m_layers.rend();

	for (; i != ie; ++i)
	{
		Layer &layer = *i;

		Layer::reverse_iterator i = layer.rbegin();
		Layer::reverse_iterator ie = layer.rend();
		for (; i != ie; ++i)
		{
			Object *obj = *i;
			if (obj->hit_test(pt, eps))
				return obj;
		}
	}

	return 0; // (!) not found
}


//////////////////////////////////////////////////////////////////////////
// get current world rect
const Rect& Plotter::world() const
{
	return m_world;
}


//////////////////////////////////////////////////////////////////////////
// get current wish world rect
const Rect& Plotter::wish() const
{
	return m_wish;
}


//////////////////////////////////////////////////////////////////////////
// get current full rect
const Rect& Plotter::full() const
{
	return m_full;
}


//////////////////////////////////////////////////////////////////////////
// get current viewport
const Rect& Plotter::view() const
{
	return m_view;
}


//////////////////////////////////////////////////////////////////////////
// convert point from view to world
Point Plotter::v2w(const Point &pt) const
{
	const Rect &W = world();
	const Rect &V = view();

	if (!V.empty())
	{
		const double x = (pt.X() - V.Xmin()) / V.Xsize();
		const double y = (V.Ymax() - pt.Y()) / V.Ysize();

		return Point(
			x*W.Xsize() + W.Xmin(),
			y*W.Ysize() + W.Ymin());
	}
	else
		return Point();
}


//////////////////////////////////////////////////////////////////////////
// convert point from world to view
Point Plotter::w2v(const Point &pt) const
{
	const Rect &W = world();
	const Rect &V = view();

	if (!W.empty())
	{
		const double x = (pt.X() - W.Xmin()) / W.Xsize();
		const double y = (W.Ymax() - pt.Y()) / W.Ysize();

		return Point(
			x*V.Xsize() + V.Xmin(),
			y*V.Ysize() + V.Ymin());
	}
	else
		return Point();
}


//////////////////////////////////////////////////////////////////////////
// convert size from view to world
Size Plotter::v2w(const Size &sz) const
{
	const Rect &W = world();
	const Rect &V = view();

	if (!V.empty())
	{
		return Size(
			sz.Xsize() * W.Xsize() / V.Xsize(),
			sz.Ysize() * W.Ysize() / V.Ysize());
	}
	else
		return Size();
}


//////////////////////////////////////////////////////////////////////////
// convert size from world to view
Size Plotter::w2v(const Size &sz) const
{
	const Rect &W = world();
	const Rect &V = view();

	if (!W.empty())
	{
		return Size(
			sz.Xsize() * V.Xsize() / W.Xsize(),
			sz.Ysize() * V.Ysize() / W.Ysize());
	}
	else
		return Size();
}


//////////////////////////////////////////////////////////////////////////
// convert rect from view to world
Rect Plotter::v2w(const Rect &rc) const
{
	const Rect &W = world();
	const Rect &V = view();

	if (!V.empty())
	{
		const double x_min = (rc.Xmin() - V.Xmin()) / V.Xsize();
		const double y_min = (V.Ymax() - rc.Ymax()) / V.Ysize();
		const double x_max = (rc.Xmax() - V.Xmin()) / V.Xsize();
		const double y_max = (V.Ymax() - rc.Ymin()) / V.Ysize();

		const Point pt_min(x_min*W.Xsize() + W.Xmin(),
			y_min*W.Ysize() + W.Ymin());
		const Point pt_max(x_max*W.Xsize() + W.Xmin(),
			y_max*W.Ysize() + W.Ymin());

		return Rect(pt_min, pt_max);
	}
	else
		return Rect();
}


//////////////////////////////////////////////////////////////////////////
// convert rect from world to view
Rect Plotter::w2v(const Rect &rc) const
{
	const Rect &W = world();
	const Rect &V = view();

	if (!W.empty())
	{
		const double x_min = (rc.Xmin() - W.Xmin()) / W.Xsize();
		const double y_min = (W.Ymax() - rc.Ymax()) / W.Ysize();
		const double x_max = (rc.Xmax() - W.Xmin()) / W.Xsize();
		const double y_max = (W.Ymax() - rc.Ymin()) / W.Ysize();

		const Point pt_min(x_min*V.Xsize() + V.Xmin(),
			y_min*V.Ysize() + V.Ymin());
		const Point pt_max(x_max*V.Xsize() + V.Xmin(),
			y_max*V.Ysize() + V.Ymin());

		return Rect(pt_min, pt_max);
	}
	else
		return Rect();
}


//////////////////////////////////////////////////////////////////////////
// get background color
const GL::Color& Plotter::background() const
{
	return m_color;
}


//////////////////////////////////////////////////////////////////////////
// set background color
void Plotter::set_background(const GL::Color &color)
{
	m_color = color;
}


//////////////////////////////////////////////////////////////////////////
// get fixed aspect ratio flag
bool Plotter::fixedAspectRatio() const
{
	return m_fixed_aspect_ratio;
}


//////////////////////////////////////////////////////////////////////////
// set fixed aspect ratio flag
void Plotter::set_fixedAspectRatio(bool fixed_AR)
{
	m_fixed_aspect_ratio = fixed_AR;
}


//////////////////////////////////////////////////////////////////////////
// set the new world rect
void Plotter::show(const Rect &wr)
{
	m_wish = wr;
	update_world();
}


//////////////////////////////////////////////////////////////////////////
// set the new viewport rect
void Plotter::set_viewport(int x, int y, int w, int h)
{
	m_view = Rect(Point(x, y), Size(w, h));
	update_world();
}


//////////////////////////////////////////////////////////////////////////
// draw plotter to the current GL context
void Plotter::draw(const GL::Font &font) const
{
	// clear background
	GL::clearColor(m_color);
	GL::clear(GL::Buffer::COLOR);

	if (!m_world.empty() && !m_view.empty())
	{
		// set viewport
		::glViewport(
			GLint(m_view.Xmin()),
			GLint(m_view.Ymin()),
			GLint(m_view.Xsize()),
			GLint(m_view.Ysize())
		);

		// set Projection matrix
		GL::matrixMode(GL::MatrixMode::PROJECTION);
		GL::load_identity();
		::glOrtho(
			m_world.Xmin(),
			m_world.Xmax(),
			m_world.Ymin(),
			m_world.Ymax(),
			-1.0, +1.0
		);

		// reset ModelView matrix
		GL::matrixMode(GL::MatrixMode::MODELVIEW);
		GL::load_identity();

		// start drawing
		on_draw_start();

		{ // draw all objects on all layers
			LayerList::const_iterator i = m_layers.begin();
			LayerList::const_iterator ie = m_layers.end();

			for (; i != ie; ++i)
			{
				const Layer &layer = *i;

				Layer::const_iterator i = layer.begin();
				Layer::const_iterator ie = layer.end();

				for (; i != ie; ++i)
					(*i)->draw(*this, font);
			}
		}

		// stop drawing
		on_draw_stop();
	}
}


//////////////////////////////////////////////////////////////////////////
// update Plotter's world rect
void Plotter::update()
{
	LayerList::const_iterator i = m_layers.begin();
	LayerList::const_iterator ie = m_layers.end();

	bool initialized = false;
	m_full = Rect();

	// calculate full area
	for (; i != ie; ++i)
	{
		const Layer &layer = *i;

		Layer::const_iterator i = layer.begin();
		Layer::const_iterator ie = layer.end();

		for (; i != ie; ++i)
		{
			Rect rc = (*i)->full();

			if (!rc.is_null())
			{
				if (!initialized)
				{
					initialized = true;
					m_full = rc;
				}
				else
					m_full |= rc;
			}
		}
	}

	// (?)
	if (0.0 == m_full.Xsize())
		m_full.inflate(0.5, 0.0);
	if (0.0 == m_full.Ysize())
		m_full.inflate(0.0, 0.5);

	// TODO: update world by specific strategy
	// TODO: inflate world rect by 5 %

	on_full_changed();
}


//////////////////////////////////////////////////////////////////////////
// world changed event
void Plotter::on_world_changed()
{
	// DO NOTHING
}


//////////////////////////////////////////////////////////////////////////
// full changed event
void Plotter::on_full_changed()
{
	// DO NOTHING
}


//////////////////////////////////////////////////////////////////////////
// prepare drawing
void Plotter::on_draw_start() const
{
	// DO NOTHING

	::glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	GL::enable(GL::Capability::BLEND);

	::glShadeModel(GL_SMOOTH);
	GL::enable(GL::Capability::POLYGON_SMOOTH);
	GL::enable(GL::Capability::POINT_SMOOTH);
	GL::enable(GL::Capability::LINE_SMOOTH);

	::glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	::glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	::glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
}


//////////////////////////////////////////////////////////////////////////
// stop drawing
void Plotter::on_draw_stop() const
{
	// DO NOTHING
}



//////////////////////////////////////////////////////////////////////////
// update world rect
void Plotter::update_world()
{
	m_world = m_wish;

	if (m_fixed_aspect_ratio && !m_view.empty())
	{
		const double X_ratio = m_wish.Xsize() / m_view.Xsize();
		const double Y_ratio = m_wish.Ysize() / m_view.Ysize();

		m_world.inflate(
			(X_ratio < Y_ratio) ? (m_view.Xsize()*Y_ratio - m_world.Xsize())/2 : 0.0,
			(X_ratio > Y_ratio) ? (m_view.Ysize()*X_ratio - m_world.Ysize())/2 : 0.0
		);
	}

	on_world_changed();
}

	} // Plotter


	// Window
	namespace plot
	{

//////////////////////////////////////////////////////////////////////////
// Window::Class implementation
class Window::Class {
public:
	//static INT_PTR CALLBACK dialog_proc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
	//{
	//	switch (uMsg)
	//	{
	//		case WM_COMMAND:
	//		{
	//			int id = GET_WM_COMMAND_ID(wParam, lParam);
	//			if (id == IDCANCEL)
	//			{
	//				::EndDialog(hDlg, id);
	//				return TRUE;
	//			}
	//		} break;
	//	}

	//	return FALSE;
	//}

	//void showDlg()
	//{
	//	HGLOBAL global = ::GlobalAlloc(GMEM_ZEROINIT,
	//		sizeof(DLGITEMTEMPLATE) + 3*sizeof(WORD));

	//	LPDLGTEMPLATE dlg_temp = (LPDLGTEMPLATE)::GlobalLock(global);

	//	// Define a dialog box template
	//	dlg_temp->style = WS_POPUP|WS_BORDER|WS_SYSMENU
	//		|WS_CAPTION|DS_MODALFRAME|DS_CENTER;
	//	dlg_temp->cx = 100;
	//	dlg_temp->cy = 100;

	//	::GlobalUnlock(global);
	//	int ret = DialogBoxIndirectParam(::GetModuleHandle(NULL),
	//		(LPCDLGTEMPLATE)global, m_handle, dialog_proc, 0);
	//	GlobalFree(global);
	//}

public:
	// register window class
	static void attach_class()
	{
		if (N_windows++ == 0)
		{
			wnd_class.cbSize = sizeof(wnd_class);
			wnd_class.style = CS_DBLCLKS|CS_HREDRAW|CS_VREDRAW|CS_OWNDC;
			wnd_class.lpfnWndProc = wnd_proc;
			wnd_class.hInstance = ::GetModuleHandle(NULL);
			wnd_class.hCursor = ::LoadCursor(NULL, IDC_ARROW);
			wnd_class.lpszClassName = _T("omni::plot::Window");

			ATOM atom = ::RegisterClassEx(&wnd_class);
			if (NULL == atom) throw std::runtime_error("Can't register Plotter's window class");
		}
	}

	// unregister window class
	static void detach_class()
	{
		if (--N_windows == 0)
		{
			BOOL ret = ::UnregisterClass(
				wnd_class.lpszClassName,
				wnd_class.hInstance);
			assert(ret != FALSE
				&& "Can't unregister Plotter's window class");
		}
	}

	// create window
	static HWND create_window(HWND parent, Window *wnd, DWORD style)
	{
		HWND hWnd = ::CreateWindowEx(0,
			wnd_class.lpszClassName, NULL, style,
			CW_USEDEFAULT, CW_USEDEFAULT,
			CW_USEDEFAULT, CW_USEDEFAULT,
			parent, NULL, wnd_class.hInstance, wnd);
		if (NULL == hWnd)
			throw std::runtime_error("Can't create Plotter's window");

		return hWnd;
	}

	// convert HWND to Window*
	static Window* get_Window(HWND hWnd)
	{
		LONG_PTR X = ::GetWindowLongPtr(hWnd, GWL_USERDATA);
		return reinterpret_cast<Window*>(X);
	}

	// set Window* to HWND
	static void set_Window(HWND hWnd, Window *wnd)
	{
		LONG_PTR X = reinterpret_cast<LONG_PTR>(wnd);
		::SetWindowLongPtr(hWnd, GWL_USERDATA, X);
	}

	// window procedure
	static LRESULT CALLBACK wnd_proc(HWND hWnd,
		UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
			case WM_CREATE:
			{
				LPCREATESTRUCT params = reinterpret_cast<LPCREATESTRUCT>(lParam);
				Window *wnd = reinterpret_cast<Window*>(params->lpCreateParams);

				set_Window(hWnd, wnd);
				wnd->on_create(hWnd);
			} break;

			case WM_DESTROY:
			{
				Window *wnd = get_Window(hWnd);
				wnd->on_destroy();

				set_Window(hWnd, 0);
			} break;
		}

		if (Window *wnd = get_Window(hWnd))
		{
			LRESULT result = 0;
			if (wnd->on_message(uMsg, wParam, lParam, result))
				return result;
		}

		return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

private:
	static WNDCLASSEX wnd_class;
	static int N_windows;
};


//////////////////////////////////////////////////////////////////////////
// static members
WNDCLASSEX Window::Class::wnd_class = {0};
int Window::Class::N_windows = 0;


//////////////////////////////////////////////////////////////////////////
// Window construction
Window::Window(DWORD style, HWND parent)
	: m_handle(0), m_context(0), m_font(0)
{
	Class::attach_class();

	show(Rect( // by default
		Point(-1.0, -1.0),
		Point(+1.0, +1.0)
	));

	m_handle = Class::create_window(
		parent, this, style);
}


//////////////////////////////////////////////////////////////////////////
// Window destruction
Window::~Window()
{
	if (m_handle)
	{
		BOOL ret = ::DestroyWindow(m_handle);
		assert(FALSE != ret
			&& "Can't destroy window");
	}

	Class::detach_class();
}


//////////////////////////////////////////////////////////////////////////
// get window handle
HWND Window::handle() const
{
	return m_handle;
}


//////////////////////////////////////////////////////////////////////////
// invalidate and repaint whole window
void Window::redraw(bool force) const
{
	::InvalidateRect(handle(), NULL, FALSE);
	if (force) ::UpdateWindow(handle());
}


//////////////////////////////////////////////////////////////////////////
// show specified World Rect
void Window::show(const Rect &W)
{
	inherited::show(W);
}


//////////////////////////////////////////////////////////////////////////
// automatic update World Rect
void Window::update()
{
	inherited::update();
}


//////////////////////////////////////////////////////////////////////////
// world changed event
void Window::on_world_changed()
{
	update_HScroll();
	update_VScroll();
}


//////////////////////////////////////////////////////////////////////////
// full changed event
void Window::on_full_changed()
{
	update_HScroll();
	update_VScroll();
}


//////////////////////////////////////////////////////////////////////////
// get full and wish Rect
Rect Window::full_and_wish() const
{
	const Rect &F = full();
	const Rect &W = wish();

	if (!F.empty())
	{
		Rect rc = F;

		if (!W.empty())
			rc |= W;

		return rc;
	}
	else
		return W;
}


//////////////////////////////////////////////////////////////////////////
// update HSCROLL info
void Window::update_HScroll()
{
	if (is_Hscroll())
	{
		SCROLLINFO info = { 0 };
		info.cbSize = sizeof(info);
		info.fMask = SIF_PAGE|SIF_POS|SIF_RANGE;
		info.nMin = 0; info.nMax = 1000000;

		const Rect FW = full_and_wish();
		if (0.0 < FW.Xsize())
		{
			const double SCALE = (info.nMax - info.nMin) / FW.Xsize();
			info.nPos = (int)floor((wish().Xmin() - FW.Xmin())*SCALE + 0.5) + info.nMin;

			int page = (int)floor(wish().Xsize()*SCALE + 0.5);
			if (info.nMax < page) page = info.nMax;
			info.nPage = page;
		}
		else
			info.nPage = info.nMax;

		::SetScrollInfo(handle(),
			SB_HORZ, &info, TRUE);
	}
}


//////////////////////////////////////////////////////////////////////////
// update VSCROLL info
void Window::update_VScroll()
{
	if (is_Vscroll())
	{
		SCROLLINFO info = { 0 };
		info.cbSize = sizeof(info);
		info.fMask = SIF_PAGE|SIF_POS|SIF_RANGE;
		info.nMin = 0; info.nMax = 1000000;

		const Rect FW = full_and_wish();
		if (0.0 < FW.Ysize())
		{
			double SCALE = (info.nMax - info.nMin) / FW.Ysize();
			info.nPos = (int)floor((FW.Ymax() - wish().Ymax())*SCALE + 0.5) + info.nMin;

			int page = (int)floor(wish().Ysize()*SCALE + 0.5);
			if (info.nMax < page) page = info.nMax;
			info.nPage = page;
		}
		else
			info.nPage = info.nMax;

		::SetScrollInfo(handle(),
			SB_VERT, &info, TRUE);
	}
}


//////////////////////////////////////////////////////////////////////////
// is HSCROLL visible?
bool Window::is_Hscroll() const
{
	LONG style = ::GetWindowLong(handle(), GWL_STYLE);
	return (style & WS_HSCROLL) != 0;
}


//////////////////////////////////////////////////////////////////////////
// is VSCROLL visible?
bool Window::is_Vscroll() const
{
	LONG style = ::GetWindowLong(handle(), GWL_STYLE);
	return (style & WS_VSCROLL) != 0;
}


//////////////////////////////////////////////////////////////////////////
// set HSCROLL and VSCROLL visible
void Window::set_scroll(bool horz, bool vert)
{
	LONG style = ::GetWindowLong(handle(), GWL_STYLE);

	if (horz)
		style |= WS_HSCROLL;
	else
		style &= ~WS_HSCROLL;

	if (vert)
		style |= WS_VSCROLL;
	else
		style &= ~WS_VSCROLL;

	::SetWindowLong(handle(), GWL_STYLE, style);
	::SetWindowPos(handle(), NULL, 0, 0, 0, 0,
		SWP_NOMOVE|SWP_NOZORDER|SWP_NOSIZE
		|SWP_FRAMECHANGED|SWP_NOACTIVATE);
}


//////////////////////////////////////////////////////////////////////////
// custom message
bool Window::on_message(UINT msg, WPARAM wparam, LPARAM lparam, LRESULT &result)
{
	switch (msg)
	{
		case WM_SIZE:
			on_size(GET_X_LPARAM(lparam),
					GET_Y_LPARAM(lparam));
			return true;

		case WM_PAINT:
			on_paint();
			return true;

		case WM_HSCROLL:
			on_Hscroll(GET_WM_HSCROLL_CODE(wparam, lparam));
			return true;

		case WM_VSCROLL:
			on_Vscroll(GET_WM_VSCROLL_CODE(wparam, lparam));
			return true;
	}

	return false;
}


//////////////////////////////////////////////////////////////////////////
// WM_CREATE
void Window::on_create(HWND hWnd)
{
	m_handle = hWnd;
	m_context = new GL::Context(hWnd);

	m_context->start();
}


//////////////////////////////////////////////////////////////////////////
// WM_DESTROY
void Window::on_destroy()
{
	// TODO: m_context->start();
	delete m_font;
	m_font = 0;

	delete m_context;
	m_context = 0;

	m_handle = NULL;
}


//////////////////////////////////////////////////////////////////////////
// WM_SIZE
void Window::on_size(int cx, int cy)
{
	inherited::set_viewport(0, 0, cx, cy);
}


//////////////////////////////////////////////////////////////////////////
// WM_PAINT
void Window::on_paint()
{
	m_context->start();
	if (!m_font)
		m_font = new GL::Font();

	inherited::draw(*m_font);

	//{ // draw test text
	//	m_font->select();

	//	const double textSize = 20.0;
	//	const double fontSize = m_font->max_height();
	//	const double X_v2w = world().Xsize() / view().Xsize();
	//	const double Y_v2w = world().Ysize() / view().Ysize();
	//	double W_textSize = textSize * Y_v2w;

	//	GL::matrixMode(GL::MatrixMode::MODELVIEW);
	//	GL::load_identity();

	//	GL::Canvas::color(1.0f, 0.0f, 0.0f, 1.0f);
	//	GL::drawText(*m_font, "[|QMg|]", 0.0, 0.0,
	//		textSize/fontSize * X_v2w, textSize/fontSize * Y_v2w,
	//		45.0, GL::TextAlign::HCENTER | GL::TextAlign::VCENTER);
	//}

	m_context->flush();

	::ValidateRect(m_handle, NULL);
}


//////////////////////////////////////////////////////////////////////////
// WM_HSCROLL
void Window::on_Hscroll(int request)
{
	SCROLLINFO info = { 0 };
	info.cbSize = sizeof(info);
	info.fMask  = SIF_PAGE|SIF_RANGE|SIF_POS|SIF_TRACKPOS;

	// get current scroll
	::GetScrollInfo(handle(),
		SB_HORZ, &info);

	const Rect FW = full_and_wish();
	if (0.0 < FW.Xsize())
	{
		double SCALE = FW.Xsize() / (info.nMax - info.nMin);
		double pagew = wish().Xsize();

		double WX = wish().Xmin();
		switch (request)
		{
			case SB_LEFT:      WX = FW.Xmin(); break;
			case SB_RIGHT:     WX = FW.Xmax()-pagew; break;
			case SB_LINELEFT:  WX -= 0.1*pagew; break;
			case SB_PAGELEFT:  WX -= 0.9*pagew; break;
			case SB_LINERIGHT: WX += 0.1*pagew; break;
			case SB_PAGERIGHT: WX += 0.9*pagew; break;
			case SB_THUMBPOSITION:
			case SB_THUMBTRACK:
				WX = FW.Xmin() + info.nTrackPos*SCALE;
				break;
		}

		Rect W = wish();
		W.offset(WX-W.Xmin(), 0.0);
		inherited::show(W);
		redraw(false);
	}
}


//////////////////////////////////////////////////////////////////////////
// WM_VSCROLL
void Window::on_Vscroll(int request)
{
	SCROLLINFO info = { 0 };
	info.cbSize = sizeof(info);
	info.fMask  = SIF_PAGE|SIF_RANGE|SIF_POS|SIF_TRACKPOS;

	// get current scroll
	::GetScrollInfo(handle(),
		SB_VERT, &info);

	const Rect FW = full_and_wish();
	if (0.0 < FW.Ysize())
	{
		double SCALE = FW.Ysize() / (info.nMax - info.nMin);
		double pageh = wish().Ysize();

		double WY = wish().Ymin();
		switch (request)
		{
			case SB_TOP:       WY = FW.Ymax()-pageh; break;
			case SB_BOTTOM:    WY = FW.Ymin(); break;
			case SB_LINEUP:    WY += 0.1*pageh; break;
			case SB_PAGEUP:    WY += 0.9*pageh; break;
			case SB_LINEDOWN:  WY -= 0.1*pageh; break;
			case SB_PAGEDOWN:  WY -= 0.9*pageh; break;
			case SB_THUMBPOSITION:
			case SB_THUMBTRACK:
				WY = FW.Ymax() - info.nTrackPos*SCALE - pageh;
				break;
		}

		Rect W = wish();
		W.offset(0.0, WY-W.Ymin());
		inherited::show(W);
		redraw(false);
	}
}

	} // Window


	// Tools
	namespace plot
	{

////////////////////////////////////////////////////////////////////////////
//// Zoom Tool construction
//Window::ZoomTool::ZoomTool()
//	: m_active(false),
//	  m_color(0.0f, 0.0f, 1.0f, 0.3f),
//	  m_line(GL::Color(0.5f, 0.5f, 0.5f, 0.7f), 1.0f, 2, 0xAAAA)
//{}
//
//
////////////////////////////////////////////////////////////////////////////
//// is Zoom Tool active?
//bool Window::ZoomTool::active() const
//{
//	return m_active;
//}
//
//
////////////////////////////////////////////////////////////////////////////
//// start zoom tool
//void Window::ZoomTool::start(Window &wnd, int x, int y)
//{
//	m_active = true;
//	m_ref_pt = Point(x, y);
//	m_world = Rect();
//
//	wnd.push_world();
//	::SetCapture(wnd.handle());
//}
//
//
////////////////////////////////////////////////////////////////////////////
//// update zoom tool
//void Window::ZoomTool::update(Window &wnd, int x, int y)
//{
//	m_world = wnd.v2w(Rect(m_ref_pt, Point(x, y)));
//	m_world.normalize();
//
//	wnd.redraw(false);
//}
//
//
////////////////////////////////////////////////////////////////////////////
//// stop zoom tool
//void Window::ZoomTool::stop(Window &wnd, int x, int y, bool zoomIn)
//{
//	m_active = false;
//
//	update(wnd, x, y);
//
//	const int CX = ::GetSystemMetrics(SM_CXDOUBLECLK);
//	const int CY = ::GetSystemMetrics(SM_CYDOUBLECLK);
//	const Size TRESHOLD = wnd.v2w(Size(CX, CY));
//
//	if (TRESHOLD.Xsize() < m_world.Xsize()
//		&& TRESHOLD.Ysize() < m_world.Ysize())
//	{
//		if (!zoomIn)
//		{
//			Rect W = wnd.wish(); // TODO: upgrade this algorithm
//			double x_min = m_world.Xmin() - wnd.world().Xmin();
//			double x_max = wnd.world().Xmax() - m_world.Xmax();
//			double y_min = m_world.Ymin() - wnd.world().Ymin();
//			double y_max = wnd.world().Ymax() - m_world.Ymax();
//			W.inflate(x_min, y_min, x_max, y_max);
//
//			wnd.Plotter::show(W); // (!)
//		}
//		else
//			wnd.Plotter::show(m_world); // (!)
//	}
//	else
//		wnd.pop_world();
//
//	::ReleaseCapture();
//	wnd.redraw(true);
//}
//
//
////////////////////////////////////////////////////////////////////////////
//// zoom
//void Window::ZoomTool::zoom(Window &wnd, int x, int y, bool zoomIn)
//{
//	Rect W = wnd.wish();
//
//	// center point
//	Point pt = wnd.v2w(Point(x, y));
//	W.offset(W.center() - pt);
//
//	if (zoomIn)
//		W.deflate(W.Xsize()*0.1, W.Ysize()*0.1);
//	else
//		W.inflate(W.Xsize()*0.1, W.Ysize()*0.1);
//
//	wnd.push_world();
//	wnd.Plotter::show(W); // (!)
//
//	wnd.redraw(true);
//}
//
//
////////////////////////////////////////////////////////////////////////////
//// cancel zoom tool
//void Window::ZoomTool::cancel(Window &wnd)
//{
//	m_active = false;
//
//	::ReleaseCapture();
//	wnd.pop_world();
//	wnd.redraw(true);
//}
//
//
////////////////////////////////////////////////////////////////////////////
//// draw zoom tool
//void Window::ZoomTool::draw()
//{
//		GL::Canvas::color(m_color);
//		GL::Canvas::rect(m_world.Xmin(), m_world.Ymin(),
//			m_world.Xmax(), m_world.Ymax());
//
//		GL::AttribBackup backup(GL::Attrib::ENABLE);
//		GL::disable(GL::Capability::LINE_SMOOTH);
//
//		m_line.select();
//
//		GL::GroupLineLoop g;
//		g.vertex(m_world.Xmin(), m_world.Ymin());
//		g.vertex(m_world.Xmin(), m_world.Ymax());
//		g.vertex(m_world.Xmax(), m_world.Ymax());
//		g.vertex(m_world.Xmax(), m_world.Ymin());
//}
//
//
//
////////////////////////////////////////////////////////////////////////////
//// Move Tool construction
//Window::MoveTool::MoveTool()
//	: m_active(false)
//{}
//
//
////////////////////////////////////////////////////////////////////////////
//// is move tool active?
//bool Window::MoveTool::active() const
//{
//	return m_active;
//}
//
//
////////////////////////////////////////////////////////////////////////////
//// start move tool
//void Window::MoveTool::start(Window &wnd, int x, int y)
//{
//	m_active = true;
//	m_ref_pt = Point(x, y);
//	m_world = wnd.wish();
//
//	wnd.push_world();
//	::SetCapture(wnd.handle());
//}
//
//
////////////////////////////////////////////////////////////////////////////
//// update move tool
//bool Window::MoveTool::update(Window &wnd, int x, int y)
//{
//	bool ret = false;
//
//	//const int CX = ::GetSystemMetrics(SM_CXDOUBLECLK);
//	//const int CY = ::GetSystemMetrics(SM_CYDOUBLECLK);
//
//	//if (CX < fabs(m_ref_pt.X() - x)
//	//	&& CY < fabs(m_ref_pt.Y() - y))
//	//{
//		Rect W = m_world;
//		Size sz = wnd.v2w(Point(x, y) - m_ref_pt);
//		W.offset(+sz.Xsize(), -sz.Ysize());
//
//		wnd.Plotter::show(W); // (!)
//		ret = true;
//	//}
//
//	wnd.redraw(false);
//	return ret;
//}
//
//
////////////////////////////////////////////////////////////////////////////
//// stop move tool
//void Window::MoveTool::stop(Window &wnd, int x, int y)
//{
//	m_active = false;
//
//	if (!update(wnd, x, y))
//		wnd.pop_world();
//
//	::ReleaseCapture();
//	wnd.redraw(true);
//}
//
//
////////////////////////////////////////////////////////////////////////////
//// center tool
//void Window::MoveTool::center(Window &wnd, int x, int y)
//{
//	Rect W = wnd.wish();
//
//	// center point
//	Point pt = wnd.v2w(Point(x, y));
//	W.offset(W.center() - pt);
//
//	wnd.push_world();
//	wnd.Plotter::show(W); // (!)
//
//	wnd.redraw(true);
//}
//
//
////////////////////////////////////////////////////////////////////////////
//// cancel tool
//void Window::MoveTool::cancel(Window &wnd)
//{
//	m_active = false;
//
//	::ReleaseCapture();
//	wnd.pop_world();
//	wnd.redraw(true);
//}
	} // Tools



	// Axis
	namespace plot
	{

//////////////////////////////////////////////////////////////////////////
// Axis construction
Axis::Axis()
	: m_majorLine(GL::Color(0.5f, 0.5f, 0.5f), 2.0f, GL::Pen::STYLE_SOLID),
	  m_minorLine(GL::Color(0.2f, 0.2f, 0.2f), 1.0f, GL::Pen::STYLE_DOT),
	  m_majorStep(0.0), m_minorStep(0.0),
	  m_majorAutoStep(true), m_majorAutoStepHint(70.0),
	  m_minorAutoStep(true), m_minorAutoStepHint(0.2),
	  m_backColor(0.3f, 0.3f, 0.3f, 0.9f),
	  m_textColor(1.0f, 1.0f, 1.0f),
	  m_axisName(), m_textSize(14.0),
	  m_textFormat("%+g"),
	  m_textOffset(0.0),
	  m_textScale(1.0)
{}


//////////////////////////////////////////////////////////////////////////
// Axis destruction
Axis::~Axis()
{}


//////////////////////////////////////////////////////////////////////////
// get minor Pen Attributes
const GL::Pen& Axis::minorLine() const
{
	return m_minorLine;
}


//////////////////////////////////////////////////////////////////////////
// set minor Pen Attributes
void Axis::set_minorLine(const GL::Pen &pen)
{
	m_minorLine = pen;
}


//////////////////////////////////////////////////////////////////////////
// get minor step
double Axis::minorStep() const
{
	return m_minorStep;
}


//////////////////////////////////////////////////////////////////////////
// set minor step
void Axis::set_minorStep(double step)
{
	set_minorAutoStep(false); // (!)
	m_minorStep = step;
}


//////////////////////////////////////////////////////////////////////////
// get minor auto step flag
bool Axis::minorAutoStep() const
{
	return m_minorAutoStep;
}


//////////////////////////////////////////////////////////////////////////
// set minor auto step flag
void Axis::set_minorAutoStep(bool enabled)
{
	m_minorAutoStep = enabled;
}


//////////////////////////////////////////////////////////////////////////
// get major auto step hint
double Axis::minorAutoStepHint() const
{
	return m_minorAutoStepHint;
}


//////////////////////////////////////////////////////////////////////////
// set major auto step hint
void Axis::set_minorAutoStepHint(double step_hint)
{
	m_minorAutoStepHint = step_hint;
}


///////////////////////////////////////////////////////////////////////////
// get major Pen attributes
const GL::Pen& Axis::majorLine() const
{
	return m_majorLine;
}


///////////////////////////////////////////////////////////////////////////
// set major Pen attributes
void Axis::set_majorLine(const GL::Pen &pen)
{
	m_majorLine = pen;
}


//////////////////////////////////////////////////////////////////////////
// get major step
double Axis::majorStep() const
{
	return m_majorStep;
}


//////////////////////////////////////////////////////////////////////////
// set major step
void Axis::set_majorStep(double step)
{
	set_majorAutoStep(false); // (!)
	m_majorStep = step;
}


//////////////////////////////////////////////////////////////////////////
// get major auto step flag
bool Axis::majorAutoStep() const
{
	return m_majorAutoStep;
}


//////////////////////////////////////////////////////////////////////////
// set major auto step flag
void Axis::set_majorAutoStep(bool enabled)
{
	m_majorAutoStep = enabled;
}


//////////////////////////////////////////////////////////////////////////
// get major auto step hint
double Axis::majorAutoStepHint() const
{
	return m_majorAutoStepHint;
}


//////////////////////////////////////////////////////////////////////////
// set major auto step hint
void Axis::set_majorAutoStepHint(double step_hint)
{
	m_majorAutoStepHint = step_hint;
}


//////////////////////////////////////////////////////////////////////////
// get background color
const GL::Color& Axis::backColor() const
{
	return m_backColor;
}


//////////////////////////////////////////////////////////////////////////
// set background color
void Axis::set_backColor(const GL::Color &color)
{
	m_backColor = color;
}


//////////////////////////////////////////////////////////////////////////
// get text color
const GL::Color& Axis::textColor() const
{
	return m_textColor;
}


//////////////////////////////////////////////////////////////////////////
// set text color
void Axis::set_textColor(const GL::Color &color)
{
	m_textColor = color;
}


//////////////////////////////////////////////////////////////////////////
// get axis name
const std::string& Axis::axisName() const
{
	return m_axisName;
}


//////////////////////////////////////////////////////////////////////////
// set axis name
void Axis::set_axisName(const std::string &name)
{
	m_axisName = name;
}


//////////////////////////////////////////////////////////////////////////
// get text size in pixels
double Axis::textSize() const
{
	return m_textSize;
}


//////////////////////////////////////////////////////////////////////////
// set text size in pixels
void Axis::set_textSize(double size)
{
	m_textSize = size;
}


//////////////////////////////////////////////////////////////////////////
// get text format string
const std::string& Axis::textFormat() const
{
	return m_textFormat;
}


//////////////////////////////////////////////////////////////////////////
// set text format string
void Axis::set_textFormat(const std::string &format)
{
	m_textFormat = format;
}


//////////////////////////////////////////////////////////////////////////
// get transformation offset
double Axis::textOffset() const
{
	return m_textOffset;
}


//////////////////////////////////////////////////////////////////////////
// set transformation offset
void Axis::set_textOffset(double offset)
{
	m_textOffset = offset;
}


//////////////////////////////////////////////////////////////////////////
// get transformation scale
double Axis::textScale() const
{
	return m_textScale;
}


//////////////////////////////////////////////////////////////////////////
// set transformation scale
void Axis::set_textScale(double scale)
{
	m_textScale = scale;
}


//////////////////////////////////////////////////////////////////////////
// get transformed value
double Axis::transform(double x) const
{
	return x * m_textScale
		+ m_textOffset;
}


//////////////////////////////////////////////////////////////////////////
// update major and minor steps
void Axis::update(double world_range, double viewport_range)
{
	// major step
	if (m_majorAutoStep)
	{
		m_majorStep = auto_step(world_range,
			viewport_range, m_majorAutoStepHint);
	}

	// minor step
	if (m_minorAutoStep)
	{
		m_minorStep = m_majorStep
			* m_minorAutoStepHint;
	}
}


//////////////////////////////////////////////////////////////////////////
// calculate auto step
double Axis::auto_step(double world_range, double viewport_range, double viewport_step)
{
	int N_steps = int(viewport_range / viewport_step);
	if (N_steps <= 0) N_steps = 1;  // (?) min=1
	if (10 < N_steps) N_steps = 10; // (?) max=10

	double step = world_range / N_steps;
	double order = pow(10.0, floor(log10(step)));

	step /= order;
	if (step < 1.00) step = 1.00;
	else if (step < 1.25) step = 1.25;
	else if (step < 2.00) step = 2.00;
	else if (step < 2.50) step = 2.50;
	else if (step < 5.00) step = 5.00;
	else                  step = 10.0;

	return step * order;
}

	} // Axis


	// XYPlotter::XAxis
	namespace plot
	{

//////////////////////////////////////////////////////////////////////////
// XAxis class
class XYPlotter::XAxis: public Axis {
	typedef Axis inherited;

public:
	XAxis()
	{
		m_minor_lines.set_axis(this);
		m_major_lines.set_axis(this);
		m_text.set_axis(this);
	}

public:

//////////////////////////////////////////////////////////////////////////
// minor lines drawer
	class MinorLines: public Object {
	public:
		MinorLines(): m_axis(0) {}
		void set_axis(Axis *axis)
			{ m_axis = axis; }

	public:
		virtual void draw(const Plotter &plotter, const GL::Font&) const
		{
			const double step = m_axis->minorStep();
			const Rect &W = plotter.world();

			// draw grid lines
			if (0.0 < step)
			{
				m_axis->minorLine().select();
				GL::GroupLines g;

				double Xstart = ceil(W.Xmin()/step) * step;
				for (double x = Xstart; x <= W.Xmax(); x += step)
				{
					g.vertex(x, W.Ymin());
					g.vertex(x, W.Ymax());
				}
			}
		}

	private:
		Axis *m_axis;
	};


//////////////////////////////////////////////////////////////////////////
// major lines drawer
	class MajorLines: public Object {
	public:
		MajorLines(): m_axis(0) {}
		void set_axis(Axis *axis)
			{ m_axis = axis; }

	public:
		virtual void draw(const Plotter &plotter, const GL::Font&) const
		{
			const double step = m_axis->majorStep();
			const Rect &W = plotter.world();

			// draw grid lines
			if (0.0 < step)
			{
				m_axis->majorLine().select();
				GL::GroupLines g;

				double Xstart = ceil(W.Xmin()/step) * step;
				for (double x = Xstart; x <= W.Xmax(); x += step)
				{
					g.vertex(x, W.Ymin());
					g.vertex(x, W.Ymax());
				}
			}
		}

	private:
		Axis *m_axis;
	};


//////////////////////////////////////////////////////////////////////////
// text drawer
	class Text: public Object {
	public:
		Text(): m_axis(0) {}
		void set_axis(Axis *axis)
			{ m_axis = axis; }

	public:
		enum { MARGIN = 2 };

	public:
		virtual void draw(const Plotter &plotter, const GL::Font &font) const
		{
			const Rect &W = plotter.world();
			const Rect &V = plotter.view();

			// convert height from view to world
			const double X_v2w = W.Xsize() / V.Xsize();
			const double Y_v2w = W.Ysize() / V.Ysize();
			const double S = (m_axis->textSize() + 2*MARGIN) * Y_v2w; // [world]
			const double FONT_scale = m_axis->textSize() / font.max_height();

			// draw axis RECT
			GL::Canvas::color(m_axis->backColor());
			GL::Canvas::rect(W.Xmin(), W.Ymin(),
				W.Xmax(), W.Ymin() + S);

			GL::Canvas::color(m_axis->textColor());
			font.select();

			// draw Axis name
			GL::drawText(font, m_axis->axisName().c_str(),
				W.Xmax() - MARGIN*X_v2w, W.Ymin() + MARGIN*Y_v2w,
				FONT_scale * X_v2w, FONT_scale * Y_v2w, 0.0,
				GL::TextAlign::RIGHT|GL::TextAlign::BOTTOM);

			// draw axis text
			const double step = m_axis->majorStep();
			if (0.0 < step)
			{
				enum { buf_sz = 1024 };
				char buf[buf_sz];

				const double Xstart = ceil(W.Xmin()/step) * step;
				const double Xstop = W.Xmax() - 2*MARGIN*X_v2w
					- font.text_width(m_axis->axisName().c_str())*FONT_scale*X_v2w;
				for (double x = Xstart; x <= W.Xmax(); x += step)
				{
					_snprintf(buf, buf_sz, m_axis->textFormat().c_str(), m_axis->transform(x));
					const double buf_width = font.text_width(buf) * FONT_scale*X_v2w;

					if (x + buf_width/2 <= Xstop && W.Xmin() <= x - buf_width/2)
						GL::drawText(font, buf, x, W.Ymin() + MARGIN*Y_v2w,
							FONT_scale * X_v2w, FONT_scale * Y_v2w, 0.0,
							GL::TextAlign::HCENTER|GL::TextAlign::BOTTOM);
				}
			}
		}

	private:
		Axis *m_axis;
	};

public:
	void update(const Plotter &plotter)
	{
		inherited::update(
			plotter.world().Xsize(),
			plotter.view().Xsize());
	}


	Object& minorLines() { return m_minor_lines; }
	Object& majorLines() { return m_major_lines; }
	Object& text() { return m_text; }

private:
	MinorLines m_minor_lines;
	MajorLines m_major_lines;
	Text m_text;
};


//////////////////////////////////////////////////////////////////////////
// get margin
//Rect XAxis::margin() const
//{
//	return Rect(Point(0.0, textSize()+2*MARGIN), Point());
//}

	} // XYPlotter::XAxis


	// XYPlotter::YAxis
	namespace plot
	{

//////////////////////////////////////////////////////////////////////////
// YAxis class
class XYPlotter::YAxis: public Axis {
	typedef Axis inherited;

public:
	YAxis()
	{
		m_minor_lines.set_axis(this);
		m_major_lines.set_axis(this);
		m_text.set_axis(this);
	}

public:

//////////////////////////////////////////////////////////////////////////
// minor lines drawer
	class MinorLines: public Object {
	public:
		MinorLines(): m_axis(0) {}
		void set_axis(Axis *axis)
			{ m_axis = axis; }

	public:
		virtual void draw(const Plotter &plotter, const GL::Font&) const
		{
			const double step = m_axis->minorStep();
			const Rect W = plotter.world();

			// draw grid lines
			if (0.0 < step)
			{
				m_axis->minorLine().select();
				GL::GroupLines g;

				double Ystart = ceil(W.Ymin()/step) * step;
				for (double y = Ystart; y <= W.Ymax(); y += step)
				{
					g.vertex(W.Xmin(), y);
					g.vertex(W.Xmax(), y);
				}
			}
		}

	private:
		Axis *m_axis;
	};


//////////////////////////////////////////////////////////////////////////
// major lines drawer
	class MajorLines: public Object {
	public:
		MajorLines(): m_axis(0) {}
		void set_axis(Axis *axis)
			{ m_axis = axis; }

	public:
		virtual void draw(const Plotter &plotter, const GL::Font&) const
		{
			const double step = m_axis->majorStep();
			const Rect W = plotter.world();

			// draw major lines
			if (0.0 < step)
			{
				m_axis->majorLine().select();
				GL::GroupLines g;

				double Ystart = ceil(W.Ymin()/step) * step;
				for (double y = Ystart; y <= W.Ymax(); y += step)
				{
					g.vertex(W.Xmin(), y);
					g.vertex(W.Xmax(), y);
				}
			}
		}

	private:
		Axis *m_axis;
	};


//////////////////////////////////////////////////////////////////////////
// text drawer
	class Text: public Object {
	public:
		Text(): m_axis(0) {}
		void set_axis(Axis *axis)
			{ m_axis = axis; }

	public:
		enum { MARGIN = 2 };

	public:
		virtual void draw(const Plotter &plotter, const GL::Font &font) const
		{
			const Rect &W = plotter.world();
			const Rect &V = plotter.view();

			// convert height from view to world
			const double X_v2w = W.Xsize() / V.Xsize();
			const double Y_v2w = W.Ysize() / V.Ysize();
			const double S = (m_axis->textSize() + 2*MARGIN) * X_v2w; // [world]
			const double FONT_scale = m_axis->textSize() / font.max_height();

			// draw axis RECT
			GL::Canvas::color(m_axis->backColor());
			GL::Canvas::rect(W.Xmin(), W.Ymin(),
				W.Xmin() + S, W.Ymax());

			GL::Canvas::color(m_axis->textColor());
			font.select();

			// draw Axis name
			GL::drawText(font, m_axis->axisName().c_str(),
				W.Xmin() + S - MARGIN*X_v2w, W.Ymax() - MARGIN*Y_v2w,
				FONT_scale * X_v2w, FONT_scale * Y_v2w, 90.0,
				GL::TextAlign::RIGHT|GL::TextAlign::BOTTOM);

			// draw axis text
			const double step = m_axis->majorStep();
			if (0.0 < step)
			{
				enum { buf_sz = 1024 };
				char buf[buf_sz];

				const double Ystart = ceil(W.Ymin()/step) * step;
				const double Ystop = W.Ymax() - 2*MARGIN*Y_v2w
					- font.text_width(m_axis->axisName().c_str())*FONT_scale*Y_v2w;
				for (double y = Ystart; y <= W.Ymax(); y += step)
				{
					_snprintf(buf, buf_sz, m_axis->textFormat().c_str(), m_axis->transform(y));
					const double buf_width = font.text_width(buf) * FONT_scale*Y_v2w;

					if (y + buf_width/2 <= Ystop && W.Ymin() <= y - buf_width/2)
						GL::drawText(font, buf, W.Xmin() + S - MARGIN*X_v2w, y,
						FONT_scale * X_v2w, FONT_scale * Y_v2w, 90.0,
						GL::TextAlign::HCENTER|GL::TextAlign::BOTTOM);
				}
			}
		}

	private:
		Axis *m_axis;
	};

public:
	void update(const Plotter &plotter)
	{
		inherited::update(
			plotter.world().Ysize(),
			plotter.view().Ysize());
	}


	Object& minorLines() { return m_minor_lines; }
	Object& majorLines() { return m_major_lines; }
	Object& text() { return m_text; }

private:
	MinorLines m_minor_lines;
	MajorLines m_major_lines;
	Text m_text;
};

////////////////////////////////////////////////////////////////////////////
//// get margin
//Rect YAxis::margin() const
//{
//	return Rect(Point(textSize()+2*MARGIN, 0.0), Point());
//}

	} // XYPlotter::YAxis


	// XYPlotter
	namespace plot
	{

//////////////////////////////////////////////////////////////////////////
// XYPlotter construction
XYPlotter::XYPlotter(DWORD style, HWND parent)
	: inherited(style, parent),
	  m_Xaxis(new XAxis()),
	  m_Yaxis(new YAxis())
{
	attach(m_Xaxis->minorLines(), LEVEL_GRID);
	attach(m_Yaxis->minorLines(), LEVEL_GRID);

	attach(m_Xaxis->majorLines(), LEVEL_GRID);
	attach(m_Yaxis->majorLines(), LEVEL_GRID);

	attach(m_Xaxis->text(), LEVEL_TEXT);
	attach(m_Yaxis->text(), LEVEL_TEXT);

	show(wish());
}


//////////////////////////////////////////////////////////////////////////
// XYPlotter destruction
XYPlotter::~XYPlotter()
{
	delete m_Xaxis;
	delete m_Yaxis;
}


//////////////////////////////////////////////////////////////////////////
// attach the object
void XYPlotter::attach(Object &obj)
{
	inherited::attach(obj, LEVEL_GRAPH);
}


//////////////////////////////////////////////////////////////////////////
// attach the object
void XYPlotter::attach(Object &obj, size_t level)
{
	inherited::attach(obj, level);
}


//////////////////////////////////////////////////////////////////////////
// detach the object
void XYPlotter::detach(Object &obj)
{
	inherited::detach(obj);
}


//////////////////////////////////////////////////////////////////////////
// get the constant X axis
const Axis& XYPlotter::Xaxis() const
{
	return *m_Xaxis;
}


//////////////////////////////////////////////////////////////////////////
// get the X axis
Axis& XYPlotter::Xaxis()
{
	return *m_Xaxis;
}

//////////////////////////////////////////////////////////////////////////
// get the constant Y axis
const Axis& XYPlotter::Yaxis() const
{
	return *m_Yaxis;
}


//////////////////////////////////////////////////////////////////////////
// get the constant Y axis
Axis& XYPlotter::Yaxis()
{
	return *m_Yaxis;
}


//////////////////////////////////////////////////////////////////////////
// on World changed
void XYPlotter::on_world_changed()
{
	inherited::on_world_changed();

	//const double dx = m_Xaxis->textSize() + 2*XAxis::Text::MARGIN;
	//const double dy = m_Yaxis->textSize() + 2*YAxis::Text::MARGIN;

	//const double Xscale = dx / view().Xsize();
	//const double Yscale = dy / view().Ysize();

	//const double Xmin = (W.Xmin() - W.Xmax()*Xscale) / (1.0 - Xscale);
	//const double Ymin = (W.Ymin() - W.Ymax()*Yscale) / (1.0 - Yscale);

	//inherited::show(Rect(Point(Xmin, Ymin),
	//	Point(W.Xmax(), W.Ymax())));

	m_Xaxis->update(*this);
	m_Yaxis->update(*this);
}


//////////////////////////////////////////////////////////////////////////
// on FULL changed
void XYPlotter::on_full_changed()
{
	inherited::on_full_changed();
}

	} // XYPlotter


	// PolarPlotter::RadialAxis
	namespace plot
	{

//////////////////////////////////////////////////////////////////////////
// RadialAxis class
class PolarPlotter::RadialAxis: public Axis {
	typedef Axis inherited;

public:
	RadialAxis()
	{
		m_minor_lines.set_axis(this);
		m_major_lines.set_axis(this);
		m_text.set_axis(this);
	}

public:

//////////////////////////////////////////////////////////////////////////
// minor lines drawer
	class MinorLines: public Object {
	public:
		MinorLines(): m_axis(0) {}
		void set_axis(Axis *axis)
			{ m_axis = axis; }

	public:
		virtual void draw(const Plotter &plotter, const GL::Font&) const
		{
			const double step = m_axis->minorStep();
			const Rect &W = plotter.world();

			// draw grid lines
			if (0.0 < step)
			{
				m_axis->minorLine().select();
				GL::GroupLines g;

				double Xstart = ceil(W.Xmin()/step) * step;
				for (double x = Xstart; x <= W.Xmax(); x += step)
				{
					g.vertex(x, W.Ymin());
					g.vertex(x, W.Ymax());
				}
			}
		}

	private:
		Axis *m_axis;
	};


//////////////////////////////////////////////////////////////////////////
// major lines drawer
	class MajorLines: public Object {
	public:
		MajorLines(): m_axis(0) {}
		void set_axis(Axis *axis)
			{ m_axis = axis; }

	public:
		virtual void draw(const Plotter &plotter, const GL::Font&) const
		{
			const double step = m_axis->majorStep();
			const Rect &W = plotter.world();

			// draw grid lines
			if (0.0 < step)
			{
				m_axis->majorLine().select();
				GL::GroupLines g;

				double Xstart = ceil(W.Xmin()/step) * step;
				for (double x = Xstart; x <= W.Xmax(); x += step)
				{
					g.vertex(x, W.Ymin());
					g.vertex(x, W.Ymax());
				}
			}
		}

	private:
		Axis *m_axis;
	};


//////////////////////////////////////////////////////////////////////////
// text drawer
	class Text: public Object {
	public:
		Text(): m_axis(0) {}
		void set_axis(Axis *axis)
			{ m_axis = axis; }

	public:
		enum { MARGIN = 2 };

	public:
		virtual void draw(const Plotter &plotter, const GL::Font &font) const
		{
			const Rect &W = plotter.world();
			const Rect &V = plotter.view();

			// convert height from view to world
			const double X_v2w = W.Xsize() / V.Xsize();
			const double Y_v2w = W.Ysize() / V.Ysize();
			const double S = (m_axis->textSize() + 2*MARGIN) * Y_v2w; // [world]
			const double FONT_scale = m_axis->textSize() / font.max_height();

			// draw axis RECT
			GL::Canvas::color(m_axis->backColor());
			GL::Canvas::rect(W.Xmin(), W.Ymin(),
				W.Xmax(), W.Ymin() + S);

			GL::Canvas::color(m_axis->textColor());
			font.select();

			// draw Axis name
			GL::drawText(font, m_axis->axisName().c_str(),
				W.Xmax() - MARGIN*X_v2w, W.Ymin() + MARGIN*Y_v2w,
				FONT_scale * X_v2w, FONT_scale * Y_v2w, 0.0,
				GL::TextAlign::RIGHT|GL::TextAlign::BOTTOM);

			// draw axis text
			const double step = m_axis->majorStep();
			if (0.0 < step)
			{
				enum { buf_sz = 1024 };
				char buf[buf_sz];

				const double Xstart = ceil(W.Xmin()/step) * step;
				const double Xstop = W.Xmax() - 2*MARGIN*X_v2w
					- font.text_width(m_axis->axisName().c_str())*FONT_scale*X_v2w;
				for (double x = Xstart; x <= W.Xmax(); x += step)
				{
					_snprintf(buf, buf_sz, m_axis->textFormat().c_str(), m_axis->transform(x));
					const double buf_width = font.text_width(buf) * FONT_scale*X_v2w;

					if (x + buf_width/2 <= Xstop && W.Xmin() <= x - buf_width/2)
						GL::drawText(font, buf, x, W.Ymin() + MARGIN*Y_v2w,
							FONT_scale * X_v2w, FONT_scale * Y_v2w, 0.0,
							GL::TextAlign::HCENTER|GL::TextAlign::BOTTOM);
				}
			}
		}

	private:
		Axis *m_axis;
	};

public:
	void update(const Plotter &plotter)
	{
		inherited::update(
			plotter.world().Xsize(),
			plotter.view().Xsize());
	}


	Object& minorLines() { return m_minor_lines; }
	Object& majorLines() { return m_major_lines; }
	Object& text() { return m_text; }

private:
	MinorLines m_minor_lines;
	MajorLines m_major_lines;
	Text m_text;
};


//////////////////////////////////////////////////////////////////////////
// get margin
//Rect XAxis::margin() const
//{
//	return Rect(Point(0.0, textSize()+2*MARGIN), Point());
//}

	} // PolarPlotter::RadialAxis


	// PolarPlotter::AngularAxis
	namespace plot
	{

//////////////////////////////////////////////////////////////////////////
// AngularAxis class
class PolarPlotter::AngularAxis: public Axis {
	typedef Axis inherited;

public:
	AngularAxis()
	{
		m_minor_lines.set_axis(this);
		m_major_lines.set_axis(this);
		m_text.set_axis(this);
	}

public:

//////////////////////////////////////////////////////////////////////////
// minor lines drawer
	class MinorLines: public Object {
	public:
		MinorLines(): m_axis(0) {}
		void set_axis(Axis *axis)
			{ m_axis = axis; }

	public:
		virtual void draw(const Plotter &plotter, const GL::Font&) const
		{
			const double step = m_axis->minorStep();
			const Rect W = plotter.world();

			// draw grid lines
			if (0.0 < step)
			{
				m_axis->minorLine().select();
				GL::GroupLines g;

				double Ystart = ceil(W.Ymin()/step) * step;
				for (double y = Ystart; y <= W.Ymax(); y += step)
				{
					g.vertex(W.Xmin(), y);
					g.vertex(W.Xmax(), y);
				}
			}
		}

	private:
		Axis *m_axis;
	};


//////////////////////////////////////////////////////////////////////////
// major lines drawer
	class MajorLines: public Object {
	public:
		MajorLines(): m_axis(0) {}
		void set_axis(Axis *axis)
			{ m_axis = axis; }

	public:
		virtual void draw(const Plotter &plotter, const GL::Font&) const
		{
			const double step = m_axis->majorStep();
			const Rect W = plotter.world();

			// draw major lines
			if (0.0 < step)
			{
				m_axis->majorLine().select();
				GL::GroupLines g;

				double Ystart = ceil(W.Ymin()/step) * step;
				for (double y = Ystart; y <= W.Ymax(); y += step)
				{
					g.vertex(W.Xmin(), y);
					g.vertex(W.Xmax(), y);
				}
			}
		}

	private:
		Axis *m_axis;
	};


//////////////////////////////////////////////////////////////////////////
// text drawer
	class Text: public Object {
	public:
		Text(): m_axis(0) {}
		void set_axis(Axis *axis)
			{ m_axis = axis; }

	public:
		enum { MARGIN = 2 };

	public:
		virtual void draw(const Plotter &plotter, const GL::Font &font) const
		{
			const Rect &W = plotter.world();
			const Rect &V = plotter.view();

			// convert height from view to world
			const double X_v2w = W.Xsize() / V.Xsize();
			const double Y_v2w = W.Ysize() / V.Ysize();
			const double S = (m_axis->textSize() + 2*MARGIN) * X_v2w; // [world]
			const double FONT_scale = m_axis->textSize() / font.max_height();

			// draw axis RECT
			GL::Canvas::color(m_axis->backColor());
			GL::Canvas::rect(W.Xmin(), W.Ymin(),
				W.Xmin() + S, W.Ymax());

			GL::Canvas::color(m_axis->textColor());
			font.select();

			// draw Axis name
			GL::drawText(font, m_axis->axisName().c_str(),
				W.Xmin() + S - MARGIN*X_v2w, W.Ymax() - MARGIN*Y_v2w,
				FONT_scale * X_v2w, FONT_scale * Y_v2w, 90.0,
				GL::TextAlign::RIGHT|GL::TextAlign::BOTTOM);

			// draw axis text
			const double step = m_axis->majorStep();
			if (0.0 < step)
			{
				enum { buf_sz = 1024 };
				char buf[buf_sz];

				const double Ystart = ceil(W.Ymin()/step) * step;
				const double Ystop = W.Ymax() - 2*MARGIN*Y_v2w
					- font.text_width(m_axis->axisName().c_str())*FONT_scale*Y_v2w;
				for (double y = Ystart; y <= W.Ymax(); y += step)
				{
					_snprintf(buf, buf_sz, m_axis->textFormat().c_str(), m_axis->transform(y));
					const double buf_width = font.text_width(buf) * FONT_scale*Y_v2w;

					if (y + buf_width/2 <= Ystop && W.Ymin() <= y - buf_width/2)
						GL::drawText(font, buf, W.Xmin() + S - MARGIN*X_v2w, y,
						FONT_scale * X_v2w, FONT_scale * Y_v2w, 90.0,
						GL::TextAlign::HCENTER|GL::TextAlign::BOTTOM);
				}
			}
		}

	private:
		Axis *m_axis;
	};

public:
	void update(const Plotter &plotter)
	{
		inherited::update(
			plotter.world().Ysize(),
			plotter.view().Ysize());
	}


	Object& minorLines() { return m_minor_lines; }
	Object& majorLines() { return m_major_lines; }
	Object& text() { return m_text; }

private:
	MinorLines m_minor_lines;
	MajorLines m_major_lines;
	Text m_text;
};

////////////////////////////////////////////////////////////////////////////
//// get margin
//Rect YAxis::margin() const
//{
//	return Rect(Point(textSize()+2*MARGIN, 0.0), Point());
//}

	} // PolarPlotter::YAxis


	// PolarPlotter
	namespace plot
	{

//////////////////////////////////////////////////////////////////////////
// PolarPlotter construction
PolarPlotter::PolarPlotter(DWORD style, HWND parent)
	: inherited(style, parent),
	  m_radialAxis(new RadialAxis()),
	  m_angularAxis(new AngularAxis())
{
	attach(m_radialAxis->minorLines(), LEVEL_GRID);
	attach(m_angularAxis->minorLines(), LEVEL_GRID);

	attach(m_radialAxis->majorLines(), LEVEL_GRID);
	attach(m_angularAxis->majorLines(), LEVEL_GRID);

	attach(m_radialAxis->text(), LEVEL_TEXT);
	attach(m_angularAxis->text(), LEVEL_TEXT);

	show(wish());
}


//////////////////////////////////////////////////////////////////////////
// PolarPlotter destruction
PolarPlotter::~PolarPlotter()
{
	delete m_radialAxis;
	delete m_angularAxis;
}


//////////////////////////////////////////////////////////////////////////
// attach the object
void PolarPlotter::attach(Object &obj)
{
	inherited::attach(obj, LEVEL_GRAPH);
}


//////////////////////////////////////////////////////////////////////////
// attach the object
void PolarPlotter::attach(Object &obj, size_t level)
{
	inherited::attach(obj, level);
}


//////////////////////////////////////////////////////////////////////////
// detach the object
void PolarPlotter::detach(Object &obj)
{
	inherited::detach(obj);
}


//////////////////////////////////////////////////////////////////////////
// get the constant radial axis
const Axis& PolarPlotter::radialAxis() const
{
	return *m_radialAxis;
}


//////////////////////////////////////////////////////////////////////////
// get the radial axis
Axis& PolarPlotter::radialAxis()
{
	return *m_radialAxis;
}

//////////////////////////////////////////////////////////////////////////
// get the constant angular axis
const Axis& PolarPlotter::angularAxis() const
{
	return *m_angularAxis;
}


//////////////////////////////////////////////////////////////////////////
// get the constant angular axis
Axis& PolarPlotter::angularAxis()
{
	return *m_angularAxis;
}


//////////////////////////////////////////////////////////////////////////
// on World changed
void PolarPlotter::on_world_changed()
{
	inherited::on_world_changed();

	//const double dx = m_Xaxis->textSize() + 2*XAxis::Text::MARGIN;
	//const double dy = m_Yaxis->textSize() + 2*YAxis::Text::MARGIN;

	//const double Xscale = dx / view().Xsize();
	//const double Yscale = dy / view().Ysize();

	//const double Xmin = (W.Xmin() - W.Xmax()*Xscale) / (1.0 - Xscale);
	//const double Ymin = (W.Ymin() - W.Ymax()*Yscale) / (1.0 - Yscale);

	//inherited::show(Rect(Point(Xmin, Ymin),
	//	Point(W.Xmax(), W.Ymax())));

	m_angularAxis->update(*this);
	m_radialAxis->update(*this);
}


//////////////////////////////////////////////////////////////////////////
// on FULL changed
void PolarPlotter::on_full_changed()
{
	inherited::on_full_changed();
}

	} // PolarPlotter


	// Marker
	namespace plot
	{

//////////////////////////////////////////////////////////////////////////
// Marker construction
Marker::Marker(const GL::Color &c, const GL::Pen &pen, double s)
	: m_color(c), m_line(pen), m_size(s)
{}


//////////////////////////////////////////////////////////////////////////
// get filling color
const GL::Color& Marker::color() const
{
	return m_color;
}


//////////////////////////////////////////////////////////////////////////
// get line properties
const GL::Pen& Marker::line() const
{
	return m_line;
}


//////////////////////////////////////////////////////////////////////////
// get marker size (in pixels)
double Marker::size() const
{
	return m_size;
}


//////////////////////////////////////////////////////////////////////////
// set filling color
void Marker::set_color(const GL::Color &c)
{
	m_color = c;
}


//////////////////////////////////////////////////////////////////////////
// set line properties
void Marker::set_line(const GL::Pen &pen)
{
	m_line = pen;
}


//////////////////////////////////////////////////////////////////////////
// set marker size (in pixels)
void Marker::set_size(double s)
{
	m_size = s;
}


//////////////////////////////////////////////////////////////////////////
// start drawing
void Marker::start()
{
	// DO NOTHING
}


//////////////////////////////////////////////////////////////////////////
// draw marker
void Marker::draw()
{
	// DO NOTHING
}


//////////////////////////////////////////////////////////////////////////
// stop drawing
void Marker::finish()
{
	// DO NOTHING
}

	} // Marker


	// SquareMarker
	namespace plot
	{

//////////////////////////////////////////////////////////////////////////
// SquareMarker construction
SquareMarker::SquareMarker(bool is_rhomb, const GL::Color &c)
	: inherited(c, GL::Pen(c, 1.0f), 4.0),
	  m_list_base(0), m_is_rhomb(is_rhomb)
{}


//////////////////////////////////////////////////////////////////////////
// SquareMarker construction
SquareMarker::SquareMarker(bool is_rhomb, const GL::Color &c, const GL::Pen &pen)
	: inherited(c, pen, 4.0), m_list_base(0), m_is_rhomb(is_rhomb)
{}


//////////////////////////////////////////////////////////////////////////
// SquareMarker construction
SquareMarker::SquareMarker(bool is_rhomb, const GL::Color &c, const GL::Pen &pen, double s)
	: inherited(c, pen, s), m_list_base(0), m_is_rhomb(is_rhomb)
{}


//////////////////////////////////////////////////////////////////////////
// start drawing
void SquareMarker::start()
{
	m_list_base = ::glGenLists(2);

	// filling list
	{ ::glNewList(m_list_base+0, GL_COMPILE);

		::glBegin(GL_POLYGON);
		if (m_is_rhomb)
		{
			GL::Canvas::vertex(0.0, -1.0);
			GL::Canvas::vertex(+1.0, 0.0);
			GL::Canvas::vertex(0.0, +1.0);
			GL::Canvas::vertex(-1.0, 0.0);
		}
		else
		{
			GL::Canvas::vertex(-1.0, -1.0);
			GL::Canvas::vertex(+1.0, -1.0);
			GL::Canvas::vertex(+1.0, +1.0);
			GL::Canvas::vertex(-1.0, +1.0);
		}
		::glEnd();

		::glEndList();
	}

	// contour list
	{ ::glNewList(m_list_base+1, GL_COMPILE);

		::glBegin(GL_LINE_LOOP);
		if (m_is_rhomb)
		{
			GL::Canvas::vertex(0.0, -1.0);
			GL::Canvas::vertex(+1.0, 0.0);
			GL::Canvas::vertex(0.0, +1.0);
			GL::Canvas::vertex(-1.0, 0.0);
		}
		else
		{
			GL::Canvas::vertex(-1.0, -1.0);
			GL::Canvas::vertex(+1.0, -1.0);
			GL::Canvas::vertex(+1.0, +1.0);
			GL::Canvas::vertex(-1.0, +1.0);
		}
		::glEnd();

		::glEndList();
	}
}


//////////////////////////////////////////////////////////////////////////
// draw marker
void SquareMarker::draw()
{
	// filling
	GL::Canvas::color(color());
	::glCallList(m_list_base+0);

	// contour
	line().select();
	::glCallList(m_list_base+1);
}


//////////////////////////////////////////////////////////////////////////
// stop drawing
void SquareMarker::finish()
{
	::glDeleteLists(m_list_base, 2);
}

	} // SquareMarker


	// CircleMarker
	namespace plot
	{

//////////////////////////////////////////////////////////////////////////
// CircleMarker construction
CircleMarker::CircleMarker(const GL::Color &c)
	: inherited(c, GL::Pen(c, 1.0f), 4.0),
	  m_list_base(0)
{}


//////////////////////////////////////////////////////////////////////////
// CircleMarker construction
CircleMarker::CircleMarker(const GL::Color &c, const GL::Pen &pen)
	: inherited(c, pen, 4.0), m_list_base(0)
{}


//////////////////////////////////////////////////////////////////////////
// CircleMarker construction
CircleMarker::CircleMarker(const GL::Color &c, const GL::Pen &pen, double s)
	: inherited(c, pen, s), m_list_base(0)
{}


//////////////////////////////////////////////////////////////////////////
// start drawing
void CircleMarker::start()
{
	m_list_base = ::glGenLists(2);
	enum { N_VERTEX = 16 };

	// filling list
	{ ::glNewList(m_list_base+0, GL_COMPILE);

		::glBegin(GL_POLYGON);
		for (size_t i = 0; i < N_VERTEX; ++i)
		{
			const double A = (2*util::PI*i)/N_VERTEX;
			GL::Canvas::vertex(cos(A), sin(A));
		}
		::glEnd();

		::glEndList();
	}

	// contour list
	{ ::glNewList(m_list_base+1, GL_COMPILE);

		::glBegin(GL_LINE_LOOP);
		for (size_t i = 0; i < N_VERTEX; ++i)
		{
			const double A = (2*util::PI*i)/N_VERTEX;
			GL::Canvas::vertex(cos(A), sin(A));
		}
		::glEnd();

		::glEndList();
	}
}


//////////////////////////////////////////////////////////////////////////
// draw marker
void CircleMarker::draw()
{
	// filling
	GL::Canvas::color(color());
	::glCallList(m_list_base+0);

	// contour
	line().select();
	::glCallList(m_list_base+1);
}


//////////////////////////////////////////////////////////////////////////
// stop drawing
void CircleMarker::finish()
{
	::glDeleteLists(m_list_base, 2);
}

	} // CircleMarker


	// StarMarker
	namespace plot
	{

//////////////////////////////////////////////////////////////////////////
// StarMarker construction
StarMarker::StarMarker(size_t Nanlges, const GL::Color &c)
	: inherited(c, GL::Pen(c, 1.0f), 4.0),
	  m_Nangles(Nanlges), m_list_base(0)
{}


//////////////////////////////////////////////////////////////////////////
// StarMarker construction
StarMarker::StarMarker(size_t Nanlges, const GL::Color &c, const GL::Pen &pen)
	: inherited(c, pen, 4.0), m_Nangles(Nanlges), m_list_base(0)
{}


//////////////////////////////////////////////////////////////////////////
// StarMarker construction
StarMarker::StarMarker(size_t Nanlges, const GL::Color &c, const GL::Pen &pen, double s)
	: inherited(c, pen, s), m_Nangles(Nanlges), m_list_base(0)
{}


//////////////////////////////////////////////////////////////////////////
// start drawing
void StarMarker::start()
{
	m_list_base = ::glGenLists(2);
	const double step = 2*util::PI / m_Nangles;
	const double Rmin = 0.5;
	const double Rmax = 1.0;

	// filling list
	{ ::glNewList(m_list_base+0, GL_COMPILE);

		::glBegin(GL_TRIANGLES);
		for (size_t i = 0; i < m_Nangles; ++i)
		{
			const double A = i*step + util::PI/2;

			GL::Canvas::vertex(0.0, 0.0);
			GL::Canvas::vertex(Rmin*cos(A-step/2), Rmin*sin(A-step/2));
			GL::Canvas::vertex(Rmax*cos(A), Rmax*sin(A));

			GL::Canvas::vertex(0.0, 0.0);
			GL::Canvas::vertex(Rmax*cos(A), Rmax*sin(A));
			GL::Canvas::vertex(Rmin*cos(A+step/2), Rmin*sin(A+step/2));
		}
		::glEnd();

		::glEndList();
	}

	// contour list
	{ ::glNewList(m_list_base+1, GL_COMPILE);

		::glBegin(GL_LINE_LOOP);
		for (size_t i = 0; i < m_Nangles; ++i)
		{
			const double A = i*step + util::PI/2;

			GL::Canvas::vertex(Rmin*cos(A-step/2), Rmin*sin(A-step/2));
			GL::Canvas::vertex(Rmax*cos(A), Rmax*sin(A));
			GL::Canvas::vertex(Rmin*cos(A+step/2), Rmin*sin(A+step/2));
		}
		::glEnd();

		::glEndList();
	}
}


//////////////////////////////////////////////////////////////////////////
// draw marker
void StarMarker::draw()
{
	// filling
	GL::Canvas::color(color());
	::glCallList(m_list_base+0);

	// contour
	line().select();
	::glCallList(m_list_base+1);
}


//////////////////////////////////////////////////////////////////////////
// stop drawing
void StarMarker::finish()
{
	::glDeleteLists(m_list_base, 2);
}

	} // StarMarker


	// LineGraph
	namespace plot
	{

//////////////////////////////////////////////////////////////////////////
// LineGraph default construction
LineGraph::LineGraph()
	: m_pen(GL::Color(0.5f, 0.5f, 0.5f, 0.8f), 2.0f),
	  m_marker(0)
{}


//////////////////////////////////////////////////////////////////////////
// LineGraph destruction
LineGraph::~LineGraph()
{}


//////////////////////////////////////////////////////////////////////////
// get pen
const GL::Pen& LineGraph::line() const
{
	return m_pen;
}


//////////////////////////////////////////////////////////////////////////
// set new pen
void LineGraph::set_line(const GL::Pen &pen)
{
	m_pen = pen;
}


//////////////////////////////////////////////////////////////////////////
// get current Marker (can be null)
const Marker* LineGraph::marker() const
{
	return m_marker;
}


//////////////////////////////////////////////////////////////////////////
// get current Marker (can be null)
Marker* LineGraph::marker()
{
	return m_marker;
}


//////////////////////////////////////////////////////////////////////////
// set new Marker object
void LineGraph::set_marker(Marker *m)
{
	m_marker = m;
}


//////////////////////////////////////////////////////////////////////////
// assign new points
void LineGraph::assign(size_t N_points, const double *Ys, const double *Xs)
{
	m_points.resize(N_points);
	for (size_t i = 0; i < N_points; ++i)
	{
		PointType &pt = m_points[i];
		pt[0] = Xs[i];
		pt[1] = Ys[i];
	}

	update();
}


//////////////////////////////////////////////////////////////////////////
// assign new points
void LineGraph::assign(size_t N_points, const double *Ys, double X_start, double X_step)
{
	m_points.resize(N_points);
	for (size_t i = 0; i < N_points; ++i)
	{
		PointType &pt = m_points[i];
		pt[0] = X_start + i*X_step;
		pt[1] = Ys[i];
	}

	update();
}


//////////////////////////////////////////////////////////////////////////
// add one point
void LineGraph::push_back(const Point &pt)
{
	// update full area
	if (m_points.empty())
		m_full = Rect(pt, Size());
	else
		m_full |= pt;

	PointType new_pt;
	new_pt[0] = pt.X();
	new_pt[1] = pt.Y();

	m_points.push_back(new_pt);
}


//////////////////////////////////////////////////////////////////////////
// add one point
void LineGraph::push_back(double x, double y)
{
	push_back(Point(x, y));
}


//////////////////////////////////////////////////////////////////////////
// clear all points
void LineGraph::clear()
{
	m_points.clear();
	m_full = Rect();
}


//////////////////////////////////////////////////////////////////////////
// draw the LineGraph
void LineGraph::draw(const Plotter &plotter, const omni::GL::Font&) const
{
	const size_t Npoints = m_points.size();

	{ // draw line
		m_pen.select();
		GL::GroupLineStrip g;

		for (size_t i = 0; i < Npoints; ++i)
			g.vertex(m_points[i]);
	}

	// draw markers
	if (marker())
	{
		const Size SCALE = plotter.v2w(Size(1.0, 1.0));

		m_marker->start();
		for (size_t i = 0; i < Npoints; ++i)
		{
			GL::MatrixBackup backup;

			const PointType &pt = m_points[i];

			GL::translate(pt[0], pt[1], 0.0);
			GL::scale(m_marker->size()*SCALE.Xsize(),
				m_marker->size()*SCALE.Ysize(), 1.0);

			m_marker->draw();
		}
		m_marker->finish();
	}
}


//////////////////////////////////////////////////////////////////////////
// hit testing
bool LineGraph::hit_test(const Point &pt, double eps) const
{
	// TODO: hit testing for LineGraph
	return false;
}


//////////////////////////////////////////////////////////////////////////
// get full Rect
Rect LineGraph::full() const
{
	return m_full;
}


//////////////////////////////////////////////////////////////////////////
// update full Rect
void LineGraph::update()
{
	if (!m_points.empty())
	{
		const PointType &pt = m_points.front();
		m_full = Rect(Point(pt[0], pt[1]), Size());

		for (size_t i = 1; i < m_points.size(); ++i)
		{
			const PointType &pt = m_points[i];
			m_full |= Point(pt[0], pt[1]);
		}
	}
	else
		m_full = Rect();
}

	} // LineGraph

} // omni namespace
