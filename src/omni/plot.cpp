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
#if !defined(_CRT_SECURE_NO_WARNINGS)
#	define _CRT_SECURE_NO_WARNINGS
#endif

#if !defined(_CRT_SECURE_NO_DEPRECATE)
#	define _CRT_SECURE_NO_DEPRECATE
#endif

#if !defined(_USE_MATH_DEFINES)
#	define _USE_MATH_DEFINES
#endif

#include <omni/plot.hpp>
#include <omni/util.hpp>

#include <algorithm>
#include <utility>

#include <float.h>
#include <math.h>

#include <WindowsX.h>
#include <tchar.h>

#pragma comment(lib, "Gdiplus.lib")

namespace
{

//////////////////////////////////////////////////////////////////////////
/// @brief Round.
/**
@param[in] x The value to round.
@param[in] eps The epsilon.
@return The rounded value.
*/
inline omni::plot::Real roundto(omni::plot::Real x, omni::plot::Real eps)
{
	return floorf(x/eps + 0.5f)*eps;
}

} // local


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
	: m_dx(0.0),
	  m_dy(0.0)
{}


//////////////////////////////////////////////////////////////////////////
/// @brief The main constructor.
/**
@param[in] dx The X size.
@param[in] dy The Y size.
*/
Size::Size(Real dx, Real dy)
	: m_dx(dx), m_dy(dy)
{}


//////////////////////////////////////////////////////////////////////////
/// @brief Get the X size.
/**
@return The X size.
*/
Real Size::X() const
{
	return m_dx;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Get the Y size.
/**
@return The Y size.
*/
Real Size::Y() const
{
	return m_dy;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Increase.
/**
		This operator increases the current size.

@param[in] sz The increasing size.
@return The self reference.
*/
Size& Size::operator+=(const Size &sz)
{
	m_dx += sz.X();
	m_dy += sz.Y();

	return *this;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Decrease.
/**
		This operator decreases the current size.

@param[in] sz The decreasing size.
@return The self reference.
*/
Size& Size::operator-=(const Size &sz)
{
	m_dx -= sz.X();
	m_dy -= sz.Y();

	return *this;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Addition.
/**
@param[in] a The first size.
@param[in] b The second size.
@return The sum of two sizes.
@relates Size
*/
const Size operator+(const Size &a, const Size &b)
{
	return Size(a.X() + b.X(),
		a.Y() + b.Y());
}


//////////////////////////////////////////////////////////////////////////
/// @brief Subtraction.
/**
@param[in] a The first size.
@param[in] b The second size.
@return The difference between two sizes.
@relates Size
*/
const Size operator-(const Size &a, const Size &b)
{
	return Size(a.X() - b.X(),
		a.Y() - b.Y());
}


//////////////////////////////////////////////////////////////////////////
/// @brief Negation.
/**
@param[in] sz The size.
@return The negated size.
@relates Size
*/
const Size operator-(const Size &sz)
{
	return Size(-sz.X(), -sz.Y());
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
	: m_x(0.0),
	  m_y(0.0)
{}


//////////////////////////////////////////////////////////////////////////
/// @brief The main constructor.
/**
@param[in] x The X coordinate.
@param[in] y The Y coordinate.
*/
Point::Point(Real x, Real y)
	: m_x(x), m_y(y)
{}


//////////////////////////////////////////////////////////////////////////
/// @brief Get the X coordinate.
/**
@return The X coordinate.
*/
Real Point::X() const
{
	return m_x;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Get the Y coordinate.
/**
@return The Y coordinate.
*/
Real Point::Y() const
{
	return m_y;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Offset the point.
/**
@param[in] dx The X offset size.
@param[in] dy The Y offset size.
*/
void Point::offset(Real dx, Real dy)
{
	m_x += dx;
	m_y += dy;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Offset the point.
/**
@param[in] sz The offset size.
*/
void Point::offset(const Size &sz)
{
	m_x += sz.X();
	m_y += sz.Y();
}


//////////////////////////////////////////////////////////////////////////
/// @brief Increase.
/**
@param[in] sz The increasing size.
@return The self reference.
@see offset()
*/
Point& Point::operator+=(const Size &sz)
{
	m_x += sz.X();
	m_y += sz.Y();

	return *this;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Decrease.
/**
@param[in] sz The decreasing size.
@return The self reference.
@see offset()
*/
Point& Point::operator-=(const Size &sz)
{
	m_x -= sz.X();
	m_y -= sz.Y();

	return *this;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Increase.
/**
@param[in] pt The point.
@param[in] sz The increasing size.
@return The new point.
@relates Point
*/
const Point operator+(const Point &pt, const Size &sz)
{
	return Point(
		pt.X() + sz.X(),
		pt.Y() + sz.Y()
	);
}


//////////////////////////////////////////////////////////////////////////
/// @brief Decrease.
/**
@param[in] pt The point.
@param[in] sz The decreasing size.
@return The new point.
@relates Point
*/
const Point operator-(const Point &pt, const Size &sz)
{
	return Point(
		pt.X() - sz.X(),
		pt.Y() - sz.Y()
	);
}


//////////////////////////////////////////////////////////////////////////
/// @brief Distance.
/**
@param[in] a The first point.
@param[in] b The second point.
@return The distance.
@relates Point
*/
const Size operator-(const Point &a, const Point &b)
{
	return Size(
		b.X() - a.X(),
		b.Y() - a.Y()
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
@param[in] pt_min The reference point.
@param[in] rc_size The rectangle size.
*/
Rect::Rect(const Point &pt_min, const Size &rc_size)
	: m_Xmin(pt_min.X()),
	  m_Xmax(pt_min.X() + rc_size.X()),
	  m_Ymin(pt_min.Y()),
	  m_Ymax(pt_min.Y() + rc_size.Y())
{}


//////////////////////////////////////////////////////////////////////////
/// @brief The main constructor (two points).
/**
@param[in] pt_min The first reference point.
@param[in] pt_max The second reference point.
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
Real Rect::Xmin() const
{
	return m_Xmin;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Get the maximum X coordinate.
/**
@return The maximum X coordinate.
*/
Real Rect::Xmax() const
{
	return m_Xmax;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Get the X size.
/**
@return The X size (i.e. the rectangle's width).
*/
Real Rect::Xsize() const
{
	return m_Xmax - m_Xmin;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Get the minimum Y coordinate.
/**
@return The minimum Y coordinate.
*/
Real Rect::Ymin() const
{
	return m_Ymin;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Get the maximum Y coordinate.
/**
@return The maximum Y coordinate.
*/
Real Rect::Ymax() const
{
	return m_Ymax;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Get the Y size.
/**
@return The Y size (i.e. the rectangle's height).
*/
Real Rect::Ysize() const
{
	return m_Ymax - m_Ymin;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Get the center point.
/**
@return The center point.
*/
const Point Rect::center() const
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
	the X() and Y() methods will return positive values.
*/
void Rect::normalize()
{
	if (m_Xmax < m_Xmin)
		std::swap(m_Xmin, m_Xmax);

	if (m_Ymax < m_Ymin)
		std::swap(m_Ymin, m_Ymax);
}


//////////////////////////////////////////////////////////////////////////
/// @brief Is rectangle empty?
/**
		The rectangle is empty if the Xsize() or Ysize() returns zero or negative value.

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
@param[in] pt The point of interest.
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
@param[in] rc The rectangle of interest.
@return @b true if the rectangle is inside, @b false otherwise.
*/
bool Rect::contains(const Rect &rc) const
{
	return m_Xmin <= rc.Xmin() && rc.Xmax() <= m_Xmax
		&& m_Ymin <= rc.Ymin() && rc.Ymax() <= m_Ymax;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Inflate rect.
/**
		The minimum point will be shifted by (-dx,-dy)
	and the maximum point will be shifted by (+dx,+dy).

@param[in] dx The X increasing size.
@param[in] dy The Y increasing size.
*/
void Rect::inflate(Real dx, Real dy)
{
	m_Xmin -= dx;
	m_Xmax += dx;
	m_Ymin -= dy;
	m_Ymax += dy;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Inflate rect.
/**
		The minimum point will be shifted by (-sz.X(),-sz.Y())
	and the maximum point will be shifted by (+sz.X(),+sz.Y()).

@param[in] sz The increasing size.
*/
void Rect::inflate(const Size &sz)
{
	m_Xmin -= sz.X();
	m_Xmax += sz.X();
	m_Ymin -= sz.Y();
	m_Ymax += sz.Y();
}


//////////////////////////////////////////////////////////////////////////
/// @brief Inflate rect.
/**
		The minimum point will be shifted by (-dx_min,-dy_min)
	and the maximum point will be shifted by (+dx_max,+dy_max).

@param[in] dx_min The minimum X increasing size.
@param[in] dy_min The minimum Y increasing size.
@param[in] dx_max The maximum X increasing size.
@param[in] dy_max The maximum Y increasing size.
*/
void Rect::inflate(Real dx_min, Real dy_min, Real dx_max, Real dy_max)
{
	m_Xmin -= dx_min;
	m_Xmax += dx_max;
	m_Ymin -= dy_min;
	m_Ymax += dy_max;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Inflate rect.
/**
		The minimum point will be shifted by (-rc.Xmin(),-rc.Ymin())
	and the maximum point will be shifted by (+rc.Xmax(),+rc.Ymax()).

@param[in] rc The increasing rectangular.
*/
void Rect::inflate(const Rect &rc)
{
	m_Xmin -= rc.Xmin();
	m_Xmax += rc.Xmax();
	m_Ymin -= rc.Ymin();
	m_Ymax += rc.Ymax();
}


//////////////////////////////////////////////////////////////////////////
/// @brief Deflate rect.
/**
		The minimum point will be shifted by (+dx,+dy)
	and the maximum point will be shifted by (-dx,-dy).

@param[in] dx The X increasing size.
@param[in] dy The Y increasing size.
*/
void Rect::deflate(Real dx, Real dy)
{
	m_Xmin += dx;
	m_Xmax -= dx;
	m_Ymin += dy;
	m_Ymax -= dy;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Deflate rect.
/**
		The minimum point will be shifted by (+sz.X(),+sz.Y())
	and the maximum point will be shifted by (-sz.X(),-sz.Y()).

@param[in] sz The decreasing size.
*/
void Rect::deflate(const Size &sz)
{
	m_Xmin += sz.X();
	m_Xmax -= sz.X();
	m_Ymin += sz.Y();
	m_Ymax -= sz.Y();
}


//////////////////////////////////////////////////////////////////////////
/// @brief Deflate rect.
/**
		The minimum point will be shifted by (+dx_min,+dy_min)
	and the maximum point will be shifted by (-dx_max,-dy_max).

@param[in] dx_min The minimum X decreasing size.
@param[in] dy_min The minimum Y decreasing size.
@param[in] dx_max The maximum X decreasing size.
@param[in] dy_max The maximum Y decreasing size.
*/
void Rect::deflate(Real dx_min, Real dy_min, Real dx_max, Real dy_max)
{
	m_Xmin += dx_min;
	m_Xmax -= dx_max;
	m_Ymin += dy_min;
	m_Ymax -= dy_max;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Deflate rect.
/**
		The minimum point will be shifted by (+rc.Xmin(),+rc.Ymin())
	and the maximum point will be shifted by (-rc.Xmax(),-rc.Ymax()).

@param[in] rc The decreasing rectangular.
*/
void Rect::deflate(const Rect &rc)
{
	m_Xmin += rc.Xmin();
	m_Xmax -= rc.Xmax();
	m_Ymin += rc.Ymin();
	m_Ymax -= rc.Ymax();
}


//////////////////////////////////////////////////////////////////////////
/// @brief Offset rect.
/**
		The minimum and the maximum points will be shifted by (+dx,+dy).

@param[in] dx The X increasing size.
@param[in] dy The Y increasing size.
*/
void Rect::offset(Real dx, Real dy)
{
	m_Xmin += dx;
	m_Xmax += dx;
	m_Ymin += dy;
	m_Ymax += dy;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Offset rect.
/**
		The minimum and the maximum points will be shifted by (+sz.X(),+sz.Y()).

@param[in] sz The increasing size.
*/
void Rect::offset(const Size &sz)
{
	m_Xmin += sz.X();
	m_Xmax += sz.X();
	m_Ymin += sz.Y();
	m_Ymax += sz.Y();
}


//////////////////////////////////////////////////////////////////////////
/// @brief Union.
/**
@param[in] rc The second rectangle.
@return The self reference.
*/
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


//////////////////////////////////////////////////////////////////////////
/// @brief Union.
/**
@param[in] pt The point.
@return The self reference.
*/
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


//////////////////////////////////////////////////////////////////////////
/// @brief Intersection.
/**
@param[in] rc The second rectangle.
@return The self reference.
*/
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


//////////////////////////////////////////////////////////////////////////
/// @brief Positive offset.
/**
@param[in] sz The offset size.
@return The self reference.
*/
Rect& Rect::operator+=(const Size &sz)
{
	offset(sz.X(), sz.Y());
	return *this;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Negative offset.
/**
@param[in] sz The offset size.
@return The self reference.
*/
Rect& Rect::operator-=(const Size &sz)
{
	offset(-sz.X(), -sz.Y());
	return *this;
}

	} // Rect


	// Object
	namespace plot
	{

//////////////////////////////////////////////////////////////////////////
/// @brief The default constructor.
Object::Object()
{}


//////////////////////////////////////////////////////////////////////////
/// @brief The destructor.
/**
		Detaches from the all plotters.
*/
Object::~Object()
{
	// detach all plotters
	while (!m_plotters.empty())
		detach(*m_plotters.back());
}


//////////////////////////////////////////////////////////////////////////
/// @brief Attach to the plotter.
/**
@param[in] plotter The plotter.
@param[in] level The draw level.
	Should be in range [0..64).
@return @b true if plotter is attached,
	@b false if plotter is already attached.
*/
bool Object::attach(Plotter &plotter, size_t level)
{
	return plotter.attach(*this, level);
}


//////////////////////////////////////////////////////////////////////////
/// @brief Detach from the plotter.
/**
@param[in] plotter The plotter.
@return @b true if object is detached,
	@b false if object not found.
*/
bool Object::detach(Plotter &plotter)
{
	return plotter.detach(*this);
}


//////////////////////////////////////////////////////////////////////////
/// @brief Draw the object.
/**
@param[in] plotter The plotter.
@param[in] canvas The canvas.
*/
void Object::draw(const Plotter &plotter, Canvas &canvas) const
{
	// DO NOTHING
}


//////////////////////////////////////////////////////////////////////////
/// @brief Hit test.
/**
@param[in] pt The hit point.
@param[in] eps The epsilon.
@return @b true if point inside object, otherwise @b false.
*/
bool Object::hit_test(const Point &pt, Real eps) const
{
	return false;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Get the "full" rectangle.
/**
@return The "full" rectangle.
*/
const Rect Object::full() const
{
	return Rect();
}

	} // Object


	// Plotter
	namespace plot
	{

//////////////////////////////////////////////////////////////////////////
/// @brief The default constructor.
Plotter::Plotter()
	: m_fixed_aspect_ratio(true),
	  m_color(Color::Black)
{}


//////////////////////////////////////////////////////////////////////////
/// @brief The destructor.
/**
	Detaches all graphic objects.
*/
Plotter::~Plotter()
{
	{ // detach all objects
		LayerList::iterator i = m_layers.begin();
		LayerList::iterator ie = m_layers.end();

		for (; i != ie; ++i)
		{
			Layer &layer = *i;

			while (!layer.empty())
				detach(*layer.back());
		}
	}
}


//////////////////////////////////////////////////////////////////////////
/// @brief Attach the graphic object.
/**
@note If graphic object is attached at different draw level
	the draw level will not be changed!

@param[in] obj The graphic object.
@param[in] level The draw level.
	Should be in range [0..64).
@return @b true if object is attached,
	@b false if object is already attached.
*/
bool Plotter::attach(Object &obj, size_t level)
{
	// saturate draw level
	if (63 < level)
		level = 63;

	bool is_attached = false;
	{ // find the object at each layer
		LayerList::iterator i = m_layers.begin();
		LayerList::iterator ie = m_layers.end();

		for (; i != ie; ++i)
		{
			Layer &layer = *i;

			Layer::iterator found = std::find(
				layer.begin(), layer.end(), &obj);
			if (layer.end() != found)
			{
				is_attached = true;
				break;
			}
		}
	}

	if (!is_attached)
	{
		if (m_layers.size() <= level)
			m_layers.resize(level+1); // ensure level is valid
		Layer &layer = m_layers[level];

		obj.m_plotters.push_back(this);
		layer.push_back(&obj);

		return true;
	}

	return false; // already attached
}


//////////////////////////////////////////////////////////////////////////
/// @brief Detach the graphic object.
/**
@param[in] obj The graphic object.
@return @b true if object is detached,
	@b false if object not found.
*/
bool Plotter::detach(Object &obj)
{
	LayerList::reverse_iterator i = m_layers.rbegin();
	LayerList::reverse_iterator ie = m_layers.rend();
	int N = 0;

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

			N += 1;
		}
	}

	return (0 < N);
}


//////////////////////////////////////////////////////////////////////////
/// @brief Find the object at point.
/**
@param[in] pt The point of interest.
@param[in] eps The epsilon.
@return The graphic object at that point or null.
*/
Object* Plotter::hit_test(const Point &pt, Real eps)
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
/// @brief Get the current "world" rectangle.
/**
@return The current "world" rectangle.
*/
const Rect& Plotter::world() const
{
	return m_world;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Get the current "wish" rectangle.
/**
@return The current "wish" rectangle.
*/
const Rect& Plotter::wish() const
{
	return m_wish;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Get the current "full" rectangle.
/**
@return The current "full" rectangle.
*/
const Rect& Plotter::full() const
{
	return m_full;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Get the current "view" rectangle.
/**
@return The current "view" rectangle.
*/
const Rect& Plotter::view() const
{
	return m_view;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Convert point from "view" to "world".
/**
@param[in] pt The point in "view" co-system.
@return The same point in "world" co-system.
*/
const Point Plotter::v2w(const Point &pt) const
{
	const Rect &W = world();
	const Rect &V = view();

	if (!V.empty())
	{
		const Real x = (pt.X() - V.Xmin()) / V.Xsize();
		const Real y = (V.Ymax() - pt.Y()) / V.Ysize();

		return Point(
			x*W.Xsize() + W.Xmin(),
			y*W.Ysize() + W.Ymin()
		);
	}
	else
		return Point();
}


//////////////////////////////////////////////////////////////////////////
/// @brief Convert point from "world" to "view".
/**
@param[in] pt The point in "world" co-system.
@return The same point in "view" co-system.
*/
const Point Plotter::w2v(const Point &pt) const
{
	const Rect &W = world();
	const Rect &V = view();

	if (!W.empty())
	{
		const Real x = (pt.X() - W.Xmin()) / W.Xsize();
		const Real y = (W.Ymax() - pt.Y()) / W.Ysize();

		return Point(
			x*V.Xsize() + V.Xmin(),
			y*V.Ysize() + V.Ymin());
	}
	else
		return Point();
}


//////////////////////////////////////////////////////////////////////////
/// @brief Convert size from "view" to "world".
/**
@param[in] sz The size in "view" co-system.
@return The same size in "world" co-system.
*/
const Size Plotter::v2w(const Size &sz) const
{
	const Rect &W = world();
	const Rect &V = view();

	if (!V.empty())
	{
		return Size(
			sz.X() * W.Xsize() / V.Xsize(),
			sz.Y() * W.Ysize() / V.Ysize());
	}
	else
		return Size();
}


//////////////////////////////////////////////////////////////////////////
/// @brief Convert size from "world" to "view".
/**
@param[in] sz The size in "world" co-system.
@return The same size in "view" co-system.
*/
const Size Plotter::w2v(const Size &sz) const
{
	const Rect &W = world();
	const Rect &V = view();

	if (!W.empty())
	{
		return Size(
			sz.X() * V.Xsize() / W.Xsize(),
			sz.Y() * V.Ysize() / W.Ysize());
	}
	else
		return Size();
}


//////////////////////////////////////////////////////////////////////////
/// @brief Convert rectangle from "view" to "world".
/**
@param[in] rc The rectangle in "view" co-system.
@return The same rectangle in "world" co-system.
*/
const Rect Plotter::v2w(const Rect &rc) const
{
	const Rect &W = world();
	const Rect &V = view();

	if (!V.empty())
	{
		const Real x_min = (rc.Xmin() - V.Xmin()) / V.Xsize();
		const Real y_min = (V.Ymax() - rc.Ymax()) / V.Ysize();
		const Real x_max = (rc.Xmax() - V.Xmin()) / V.Xsize();
		const Real y_max = (V.Ymax() - rc.Ymin()) / V.Ysize();

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
/// @brief Convert rectangle from "world" to "view".
/**
@param[in] rc The rectangle in "world" co-system.
@return The same rectangle in "view" co-system.
*/
const Rect Plotter::w2v(const Rect &rc) const
{
	const Rect &W = world();
	const Rect &V = view();

	if (!W.empty())
	{
		const Real x_min = (rc.Xmin() - W.Xmin()) / W.Xsize();
		const Real y_min = (W.Ymax() - rc.Ymax()) / W.Ysize();
		const Real x_max = (rc.Xmax() - W.Xmin()) / W.Xsize();
		const Real y_max = (W.Ymax() - rc.Ymin()) / W.Ysize();

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
/// @brief Set the main color.
/**
@param[in] color The main color.
*/
void Plotter::setColor(const Color &color)
{
	m_color = color;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Get the main color.
/**
@return The main color.
*/
const Color& Plotter::getColor() const
{
	return m_color;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Set the "fixed aspect ratio" flag.
/**
@param[in] fixed_AR The "fixed aspect ratio" flag.
*/
void Plotter::setFixedAspectRatio(bool fixed_AR)
{
	m_fixed_aspect_ratio = fixed_AR;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Get the "fixed aspect ratio" flag.
/**
@return The "fixed aspect ratio" flag.
*/
bool Plotter::getFixedAspectRatio() const
{
	return m_fixed_aspect_ratio;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Set the new "world" rectangle.
/**
		This method updates the "wish" and "world" rectangles.

@param[in] wr The new "world" rectangle.
*/
void Plotter::show(const Rect &wr)
{
	m_wish = wr;
	update_world();
}


//////////////////////////////////////////////////////////////////////////
/// @brief Set the new "view" rectangle.
/**
		This method updates the "view" rectangle.

@param[in] x The X coordinate.
@param[in] y The Y coordinate.
@param[in] w The width.
@param[in] h The height.
*/
void Plotter::setViewport(Real x, Real y, Real w, Real h)
{
	on_view_changing(x, y, w, h);

	m_view = Rect(Point(x, y), Size(w, h));
	update_world();

	on_view_changed();
}


//////////////////////////////////////////////////////////////////////////
/// @brief Draw the plotter content to the canvas.
/**
@param[in] canvas The canvas.
*/
void Plotter::draw(Canvas &canvas) const
{
	// clear background
	canvas.Clear(m_color);

	if (!m_world.empty() && !m_view.empty())
	{
		// start drawing
		on_draw_start(canvas);

		{ // draw all objects on all layers
			LayerList::const_iterator i = m_layers.begin();
			LayerList::const_iterator ie = m_layers.end();

			for (; i != ie; ++i)
			{
				const Layer &layer = *i;

				Layer::const_iterator i = layer.begin();
				Layer::const_iterator ie = layer.end();

				for (; i != ie; ++i)
					(*i)->draw(*this, canvas);
			}
		}

		if (0)
		{ // test draw
			Gdiplus::SolidBrush brush(Color::Red);
			Gdiplus::Pen pen(Color::White, 1.0f);

			Rect r1 = w2v(Rect(Point(-0.4f, -0.4f), Point(-0.6f, -0.6f)));
			Rect r2 = w2v(Rect(Point(-0.9f, -0.9f), Point(+0.9f, +0.9f)));

			canvas.FillEllipse(&brush, r1.Xmin(), r1.Ymin(), r1.Xsize(), r1.Ysize());
			canvas.DrawEllipse(&pen, r1.Xmin(), r1.Ymin(), r1.Xsize(), r1.Ysize());

			pen.SetWidth(5.0f);
			canvas.DrawRectangle(&pen, r2.Xmin(), r2.Ymin(), r2.Xsize(), r2.Ysize());
		}

		// stop drawing
		on_draw_stop(canvas);
	}
}


//////////////////////////////////////////////////////////////////////////
/// @brief Update plotter.
/**
	This method updates the "full" rectangle.
*/
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
/// @brief The "world changed" event.
void Plotter::on_world_changed()
{
	// DO NOTHING
}


//////////////////////////////////////////////////////////////////////////
/// @brief The "full changed" event.
void Plotter::on_full_changed()
{
	// DO NOTHING
}


//////////////////////////////////////////////////////////////////////////
/// @brief The "view changing" event.
void Plotter::on_view_changing(Real &x, Real &y, Real &w, Real &h)
{
	// DO NOTHING
}


//////////////////////////////////////////////////////////////////////////
/// @brief The "view changed" event.
void Plotter::on_view_changed()
{
	// DO NOTHING
}


//////////////////////////////////////////////////////////////////////////
/// @brief The "before drawing" event.
void Plotter::on_draw_start(Canvas &canvas) const
{
	canvas.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
}


//////////////////////////////////////////////////////////////////////////
/// @brief The "after drawing" event.
void Plotter::on_draw_stop(Canvas &canvas) const
{
	// DO NOTHING
}


//////////////////////////////////////////////////////////////////////////
/// @brief Update the "world" rectangle.
/**
		This method updates "world" rectangle based on current
	"wish" and "view" rectangles and the "fixed aspect ratio" flag.
*/
void Plotter::update_world()
{
	m_world = m_wish;

	if (m_fixed_aspect_ratio && !m_view.empty())
	{
		const Real X_ratio = m_wish.Xsize() / m_view.Xsize();
		const Real Y_ratio = m_wish.Ysize() / m_view.Ysize();

		m_world.inflate(
			(X_ratio < Y_ratio) ? (m_view.Xsize()*Y_ratio - m_world.Xsize())/2 : 0,
			(X_ratio > Y_ratio) ? (m_view.Ysize()*X_ratio - m_world.Ysize())/2 : 0
		);
	}

	on_world_changed();
}

	} // Plotter


	// Window
	namespace plot
	{

//////////////////////////////////////////////////////////////////////////
/// @brief The window class.
class Window::Class
{
public:

	/// @brief Register window class.
	static void attach_class()
	{
		if (1 == ::InterlockedIncrement(&N_windows))
		{
			// initialize window class
			wnd_class.cbSize = sizeof(wnd_class);
			wnd_class.style = CS_DBLCLKS|CS_HREDRAW|CS_VREDRAW|CS_OWNDC;
			wnd_class.lpfnWndProc = wnd_proc;
			wnd_class.hInstance = ::GetModuleHandle(NULL);
			wnd_class.hCursor = ::LoadCursor(NULL, IDC_ARROW);
			wnd_class.lpszClassName = _T("omni::plot::Window");

			ATOM atom = ::RegisterClassEx(&wnd_class);
			if (NULL == atom)
				throw std::runtime_error("Can't register plotter's window class");
		}
	}

	/// @brief Unregister window class.
	static void detach_class()
	{
		if (0 == ::InterlockedDecrement(&N_windows))
		{
			BOOL ret = ::UnregisterClass(
				wnd_class.lpszClassName,
				wnd_class.hInstance);
			assert(ret != FALSE
				&& "Can't unregister plotter's window class");
		}
	}

	/// @brief Create window instance.
	/**
	@param[in] parent The parent window handle.
	@param[in] wnd The corresponding Window.
	@param[in] style The window style.
	@param[in] ex_style The extended window style.
	*/
	static HWND create_window(HWND parent, Window *wnd, DWORD style, DWORD ex_style)
	{
		return ::CreateWindowEx(ex_style,
			wnd_class.lpszClassName, NULL, style,
			CW_USEDEFAULT, CW_USEDEFAULT,
			CW_USEDEFAULT, CW_USEDEFAULT,
			parent, NULL, wnd_class.hInstance, wnd);
	}

public:

	/// @brief Convert HWND to Window pointer.
	/**
	@param[in] hWnd The window handle.
	@return The corresponding Window pointer.
	*/
	static Window* get_Window(HWND hWnd)
	{
		LONG_PTR X = ::GetWindowLongPtr(hWnd, GWLP_USERDATA);
		return reinterpret_cast<Window*>(X);
	}

	/// @brief Set Window pointer to HWND.
	/**
	@param[in] hWnd The window handle.
	@param[in] wnd The corresponding window pointer.
	*/
	static void set_Window(HWND hWnd, Window *wnd)
	{
		LONG_PTR X = reinterpret_cast<LONG_PTR>(wnd);
		::SetWindowLongPtr(hWnd, GWLP_USERDATA, X);
	}

public:

	/// @brief The window procedure.
	/**
	@param[in] hWnd The window handle.
	@param[in] uMsg The message type.
	@param[in] wParam The first parameter.
	@param[in] lParam The second parameter.
	@return The result code.
	*/
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
	static volatile long N_windows;
};


//////////////////////////////////////////////////////////////////////////
/// @brief The window class info.
WNDCLASSEX Window::Class::wnd_class = {0};

//////////////////////////////////////////////////////////////////////////
/// @brief The number of active windows.
volatile long Window::Class::N_windows = 0;


//////////////////////////////////////////////////////////////////////////
/// @brief The main constructor.
/**
@param[in] parent The parent window handle.
@param[in] style The window style.
@param[in] ex_style The extended window style.
*/
Window::Window(HWND parent, DWORD style, DWORD ex_style)
	: m_handle(0),
	  m_moveEnabled(false), m_moveActive(false),
	  m_zoomEnabled(false), m_zoomActive(false)
{
	Class::attach_class();

	show(Rect( // by default
		Point(-1, -1),
		Point(+1, +1)
	));

	m_handle = Class::create_window(
		parent, this, style, ex_style);
	if (NULL == m_handle)
		throw std::runtime_error("Can't create plotter's window");
}


//////////////////////////////////////////////////////////////////////////
/// @brief The destructor.
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
/// @brief Get the window handle.
/**
@return The window handle.
*/
HWND Window::handle() const
{
	return m_handle;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Invalidate and repaint the whole window.
/**
@param[in] force The "force" flag. If set to true the window will be repainted now,
	otherwise the window will be repainted as soon as possible.
*/
void Window::redraw(bool force) const
{
	::InvalidateRect(handle(), NULL, FALSE);
	if (force) ::UpdateWindow(handle());
}


//////////////////////////////////////////////////////////////////////////
/// @brief The "world changed" event.
void Window::on_world_changed()
{
	update_hscroll();
	update_vscroll();
}


//////////////////////////////////////////////////////////////////////////
/// @brief The "full changed" event.
void Window::on_full_changed()
{
	update_hscroll();
	update_vscroll();
}


//////////////////////////////////////////////////////////////////////////
/// @brief Get the full and wish rectangle.
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
/// @brief Update horizontal scrolling.
void Window::update_hscroll()
{
	if (isHScroll())
	{
		SCROLLINFO info = { 0 };
		info.cbSize = sizeof(info);
		info.fMask = SIF_PAGE|SIF_POS|SIF_RANGE;
		info.nMin = 0; info.nMax = 1000000;

		const Rect FW = full_and_wish();
		if (0.0 < FW.Xsize())
		{
			const Real SCALE = (info.nMax - info.nMin) / FW.Xsize();
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
/// @brief Update vertical scrolling.
void Window::update_vscroll()
{
	if (isVScroll())
	{
		SCROLLINFO info = { 0 };
		info.cbSize = sizeof(info);
		info.fMask = SIF_PAGE|SIF_POS|SIF_RANGE;
		info.nMin = 0; info.nMax = 1000000;

		const Rect FW = full_and_wish();
		if (0.0 < FW.Ysize())
		{
			Real SCALE = (info.nMax - info.nMin) / FW.Ysize();
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
/// @brief Enable/disable horizontal and/or vertical scrolling.
/**
@param[in] horz Enable the horizontal scrolling flag.
@param[in] vert Enable the vertical scrolling flag.
*/
void Window::setScroll(bool horz, bool vert)
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
/// @brief Is horizontal scrolling enabled?
bool Window::isHScroll() const
{
	LONG style = ::GetWindowLong(handle(), GWL_STYLE);
	return (style & WS_HSCROLL) != 0;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Is vertical scrolling enabled?
bool Window::isVScroll() const
{
	LONG style = ::GetWindowLong(handle(), GWL_STYLE);
	return (style & WS_VSCROLL) != 0;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Enable moving tool.
/**
@param[in] enabled The enabled flag.
*/
void Window::enableMoving(bool enabled)
{
	m_moveEnabled = enabled;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Is moving tool enabled?
/**
@return The mooving enabled flag.
*/
bool Window::isMovingEnabled() const
{
	return m_moveEnabled;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Enable zooming tool.
/**
@param[in] enabled The enabled flag.
*/
void Window::enableZooming(bool enabled)
{
	m_zoomEnabled = enabled;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Is zooming tool enabled?
/**
@return The zooming enabled flag.
*/
bool Window::isZoomingEnabled() const
{
	return m_zoomEnabled;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Offset the "world" rectangle.
/**
@param[in] dx The offset in the @b X axis.
@param[in] dy The offset in the @b Y axis.
*/
void Window::offsetWorld(Real dx, Real dy)
{
	Rect W = wish();
	W.offset(dx, dy);
	show(W);
}


//////////////////////////////////////////////////////////////////////////
/// @brief Scale the "world" area.
/**
@param[in] dx The size increment in the @b X axis.
@param[in] dy The size increment in the @b Y axis.
@param[in] ref_w The reference point in "world" space.
*/
void Window::scaleWorld(Real dx, Real dy, const Point &ref_w)
{
	const Rect &W1 = wish();

	const Real w = W1.Xsize() + dx;
	const Real h = W1.Ysize() + dy;

	const float x = W1.Xmin() + w*(ref_w.X() - W1.Xmin())/W1.Xsize();
	const float y = W1.Ymin() + h*(ref_w.Y() - W1.Ymin())/W1.Ysize();

	show( Rect(Point(
		W1.Xmin() - x + ref_w.X(),
		W1.Ymin() - y + ref_w.Y()),
		Size(w, h)) );
}


//////////////////////////////////////////////////////////////////////////
/// @brief Scale the "world" area.
/**
		The reference point is the center of the "world" area.

@param[in] dx The size increment in the @b X axis.
@param[in] dy The size increment in the @b Y axis.
*/
void Window::scaleWorld(Real dx, Real dy)
{
	scaleWorld(dx, dy, wish().center());
}


//////////////////////////////////////////////////////////////////////////
/// @brief Window message processing.
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
			on_hscroll(GET_WM_HSCROLL_CODE(wparam, lparam));
			return true;

		case WM_VSCROLL:
			on_vscroll(GET_WM_VSCROLL_CODE(wparam, lparam));
			return true;

		case WM_MOUSEMOVE:
			return on_mouse_move(
				GET_KEYSTATE_WPARAM(wparam),
				GET_X_LPARAM(lparam),
				GET_Y_LPARAM(lparam));

		case WM_MOUSEWHEEL:
		{
			result = TRUE;
			return on_mouse_wheel(
				GET_WHEEL_DELTA_WPARAM(wparam),
				GET_KEYSTATE_WPARAM(wparam),
				GET_X_LPARAM(lparam),
				GET_Y_LPARAM(lparam));
		}

		case WM_LBUTTONDOWN:
			return on_Lbtn_down(
				GET_KEYSTATE_WPARAM(wparam),
				GET_X_LPARAM(lparam),
				GET_Y_LPARAM(lparam));

		case WM_LBUTTONUP:
			return on_Lbtn_up(
				GET_KEYSTATE_WPARAM(wparam),
				GET_X_LPARAM(lparam),
				GET_Y_LPARAM(lparam));

		case WM_LBUTTONDBLCLK:
			return on_Lbtn_2click(
				GET_KEYSTATE_WPARAM(wparam),
				GET_X_LPARAM(lparam),
				GET_Y_LPARAM(lparam));

		case WM_RBUTTONDOWN:
			return on_Rbtn_down(
				GET_KEYSTATE_WPARAM(wparam),
				GET_X_LPARAM(lparam),
				GET_Y_LPARAM(lparam));

		case WM_RBUTTONUP:
			return on_Rbtn_up(
				GET_KEYSTATE_WPARAM(wparam),
				GET_X_LPARAM(lparam),
				GET_Y_LPARAM(lparam));

		case WM_RBUTTONDBLCLK:
			return on_Rbtn_2click(
				GET_KEYSTATE_WPARAM(wparam),
				GET_X_LPARAM(lparam),
				GET_Y_LPARAM(lparam));
	}

	return false;
}


//////////////////////////////////////////////////////////////////////////
/// @brief The "WM_CREATE" message handler.
/**
@param[in] hWnd The window handle.
*/
void Window::on_create(HWND hWnd)
{
	m_handle = hWnd;
}


//////////////////////////////////////////////////////////////////////////
/// @brief The "WM_DESTROY" message handler.
void Window::on_destroy()
{
	m_pbmp.reset(0); // destroy bitmap
	m_handle = NULL;
}


//////////////////////////////////////////////////////////////////////////
/// @brief The "WM_SIZE" message handler.
/**
@param[in] cx The window width.
@param[in] cy The window height.
*/
void Window::on_size(int cx, int cy)
{
	inherited::setViewport(0, 0,
		(Real)cx, (Real)cy);
}


//////////////////////////////////////////////////////////////////////////
/// @brief The "WM_PAINT" message handler.
void Window::on_paint()
{
	RECT rc = {0};
	::GetClientRect(handle(), &rc);
	const int w = rc.right - rc.left;
	const int h = rc.bottom - rc.top;

	PAINTSTRUCT ps = {0};
	HDC dc = ::BeginPaint(handle(), &ps);
	Gdiplus::Graphics g(ps.hdc);

	// update bitmap
	if (!m_pbmp.get() || (int)m_pbmp->GetWidth() < w || (int)m_pbmp->GetHeight() < h)
	{
		m_pbmp.reset(new Gdiplus::Bitmap(w, h, &g));
	}

	// draw to "back" buffer
	Gdiplus::Graphics gg(&*m_pbmp);
	inherited::draw(gg);

	// copy "back" buffer to the screen
	g.DrawImage(&*m_pbmp, 0, 0);
	::EndPaint(handle(), &ps);
}


//////////////////////////////////////////////////////////////////////////
/// @brief The "WM_HSCROLL" message handler.
/**
@param[in] request The request type.
*/
void Window::on_hscroll(int request)
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
		Real SCALE = FW.Xsize() / (info.nMax - info.nMin);
		Real pagew = wish().Xsize();

		Real WX = wish().Xmin();
		switch (request)
		{
			case SB_LEFT:      WX = FW.Xmin(); break;
			case SB_RIGHT:     WX = FW.Xmax()-pagew; break;
			case SB_LINELEFT:  WX -= 0.1f*pagew; break;
			case SB_PAGELEFT:  WX -= 0.9f*pagew; break;
			case SB_LINERIGHT: WX += 0.1f*pagew; break;
			case SB_PAGERIGHT: WX += 0.9f*pagew; break;
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
/// @brief The "WM_VSCROLL" message handler.
/**
@param[in] request The request type.
*/
void Window::on_vscroll(int request)
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
		Real SCALE = FW.Ysize() / (info.nMax - info.nMin);
		Real pageh = wish().Ysize();

		Real WY = wish().Ymin();
		switch (request)
		{
			case SB_TOP:       WY = FW.Ymax()-pageh; break;
			case SB_BOTTOM:    WY = FW.Ymin(); break;
			case SB_LINEUP:    WY += 0.1f*pageh; break;
			case SB_PAGEUP:    WY += 0.9f*pageh; break;
			case SB_LINEDOWN:  WY -= 0.1f*pageh; break;
			case SB_PAGEDOWN:  WY -= 0.9f*pageh; break;
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


//////////////////////////////////////////////////////////////////////////
/// @brief The "WM_MOUSEMOVE" message handler.
/**
@param[in] vkeys The virtual keys that are down.
@param[in] x The cursor's X coordinate.
@param[in] y The cursor's Y coordinate.
@return @b true if message is handled.
*/
bool Window::on_mouse_move(int vkeys, int x, int y)
{
	if (m_moveEnabled && m_moveActive)
	{
		Rect W = m_moveWish;
		Size sz = v2w(Point(Real(x), Real(y)) - m_movePoint);
		W.offset(+sz.X(), -sz.Y());

		show(W); // (!)

		redraw(false);
		return true;
	}

	return false;
}


//////////////////////////////////////////////////////////////////////////
/// @brief The "WM_MOUSEWHEEL" message handler.
/**
@param[in] delta The wheel delta.
@param[in] vkeys The virtual keys that are down.
@param[in] x The cursor's X coordinate.
@param[in] y The cursor's Y coordinate.
@return @b true if message is handled.
*/
bool Window::on_mouse_wheel(int delta, int vkeys, int x, int y)
{
	{ // screen -> client
		POINT t;
		t.x = x;
		t.y = y;

		::ScreenToClient(handle(), &t);

		x = t.x;
		y = t.y;
	}

	if (m_zoomEnabled)
	{
		const Real scale = Real(delta) / WHEEL_DELTA / -10;
		const Point pt = v2w(Point(Real(x), Real(y)));
		const Rect &W = wish();

		scaleWorld(W.Xsize()*scale,
			W.Ysize()*scale, pt);

		redraw(true);
		return true;
	}

	return false;
}


//////////////////////////////////////////////////////////////////////////
/// @brief The "WM_LBUTTONDOWN" message handler.
/**
@param[in] vkeys The virtual keys that are down.
@param[in] x The cursor's X coordinate.
@param[in] y The cursor's Y coordinate.
@return @b true if message is handled.
*/
bool Window::on_Lbtn_down(int vkeys, int x, int y)
{
	if (m_moveEnabled)
	{
		m_moveActive = true;
		m_movePoint = Point(Real(x), Real(y));
		m_moveWish = wish();

		::SetCapture(handle());
		return true;
	}

	return false;
}


//////////////////////////////////////////////////////////////////////////
/// @brief The "WM_LBUTTONUP" message handler.
/**
@param[in] vkeys The virtual keys that are down.
@param[in] x The cursor's X coordinate.
@param[in] y The cursor's Y coordinate.
@return @b true if message is handled.
*/
bool Window::on_Lbtn_up(int vkeys, int x, int y)
{
	if (m_moveEnabled && m_moveActive)
	{
		m_moveActive = false;
		::ReleaseCapture();

		redraw(true);
		return true;
	}

	return false;
}



//////////////////////////////////////////////////////////////////////////
/// @brief The "WM_LBUTTONDBLCLK" message handler.
/**
@param[in] vkeys The virtual keys that are down.
@param[in] x The cursor's X coordinate.
@param[in] y The cursor's Y coordinate.
@return @b true if message is handled.
*/
bool Window::on_Lbtn_2click(int vkeys, int x, int y)
{
	if (m_moveEnabled || m_zoomEnabled)
	{
		show(full());
		redraw(true);
	}

	return false;
}


//////////////////////////////////////////////////////////////////////////
/// @brief The "WM_RBUTTONDOWN" message handler.
/**
@param[in] vkeys The virtual keys that are down.
@param[in] x The cursor's X coordinate.
@param[in] y The cursor's Y coordinate.
@return @b true if message is handled.
*/
bool Window::on_Rbtn_down(int vkeys, int x, int y)
{
	return false;
}


//////////////////////////////////////////////////////////////////////////
/// @brief The "WM_RBUTTONUP" message handler.
/**
@param[in] vkeys The virtual keys that are down.
@param[in] x The cursor's X coordinate.
@param[in] y The cursor's Y coordinate.
@return @b true if message is handled.
*/
bool Window::on_Rbtn_up(int vkeys, int x, int y)
{
	return false;
}


//////////////////////////////////////////////////////////////////////////
/// @brief The "WM_RBUTTONDBLCLK" message handler.
/**
@param[in] vkeys The virtual keys that are down.
@param[in] x The cursor's X coordinate.
@param[in] y The cursor's Y coordinate.
@return @b true if message is handled.
*/
bool Window::on_Rbtn_2click(int vkeys, int x, int y)
{
	return false;
}

	} // Window


	// Axis
	namespace plot
	{

//////////////////////////////////////////////////////////////////////////
/// @brief The default constructor.
Axis::Axis()
	: m_majorLine(new Gdiplus::Pen(Color::DarkGray, 1.0f)),
	  m_minorLine(new Gdiplus::Pen(Color::Gray, 1.0f)),
	  m_majorStep(0), m_minorStep(0),
	  m_majorAutoStep(true), m_majorAutoStepHint(70),
	  m_minorAutoStep(true), m_minorAutoStepHint(0.25f),
	  m_backColor(Color::DarkGreen),
	  m_textColor(Color::WhiteSmoke),
	  m_axisName(L""), m_textSize(14),
	  m_textFormat(L"%+g"),
	  m_textOffset(0),
	  m_textScale(1)
{
	m_majorLine->SetDashStyle(Gdiplus::DashStyleSolid);
	m_minorLine->SetDashStyle(Gdiplus::DashStyleDash);
}


//////////////////////////////////////////////////////////////////////////
/// @brief The destructor.
Axis::~Axis()
{}


//////////////////////////////////////////////////////////////////////////
/// @brief Set the minor line's pen.
/**
@param[in] pen The minor line's pen.
*/
void Axis::setMinorPen(const Gdiplus::Pen &pen)
{
	m_minorLine.reset(pen.Clone());
}


//////////////////////////////////////////////////////////////////////////
/// @brief Get the minor line's pen.
/**
@return The minor line's pen.
*/
const Gdiplus::Pen* Axis::getMinorPen() const
{
	return m_minorLine.get();
}


//////////////////////////////////////////////////////////////////////////
/// @brief Get the minor line's pen.
/**
@return The minor line's pen.
*/
Gdiplus::Pen* Axis::getMinorPen()
{
	return m_minorLine.get();
}


//////////////////////////////////////////////////////////////////////////
/// @brief Set the minor line step.
/**
@param[in] step The minor line step.
*/
void Axis::setMinorStep(Real step)
{
	setMinorAutoStep(false); // (!)
	m_minorStep = step;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Get the minor line step.
/**
@return The minor line step.
*/
Real Axis::getMinorStep() const
{
	return m_minorStep;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Set the minor "auto step" flag.
/**
@param[in] enabled The minor "auto step" flag.
*/
void Axis::setMinorAutoStep(bool enabled)
{
	m_minorAutoStep = enabled;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Get the minor "auto step" flag.
/**
@return The minor "auto step" flag.
*/
bool Axis::getMinorAutoStep() const
{
	return m_minorAutoStep;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Set the minor auto step hint.
/**
	This hint is the fractional part of the major step.

@param[in] hint The minor auto step hint.
*/
void Axis::setMinorAutoStepHint(Real hint)
{
	m_minorAutoStepHint = hint;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Get the minor auto step hint.
Real Axis::getMinorAutoStepHint() const
{
	return m_minorAutoStepHint;
}


///////////////////////////////////////////////////////////////////////////
/// @brief Set the major line's pen.
/**
@param[in] pen The major line's pen.
*/
void Axis::setMajorPen(const Gdiplus::Pen &pen)
{
	m_majorLine.reset(pen.Clone());
}


///////////////////////////////////////////////////////////////////////////
/// @brief Get the major line's pen.
/**
@return The major line's pen.
*/
const Gdiplus::Pen* Axis::getMajorPen() const
{
	return m_majorLine.get();
}


///////////////////////////////////////////////////////////////////////////
/// @brief Get the major line's pen.
/**
@return The major line's pen.
*/
Gdiplus::Pen* Axis::getMajorPen()
{
	return m_majorLine.get();
}


//////////////////////////////////////////////////////////////////////////
/// @brief Set the major line step.
/**
@param[in] step The major line step.
*/
void Axis::setMajorStep(Real step)
{
	setMajorAutoStep(false); // (!)
	m_majorStep = step;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Get the major line step.
/**
@return The major line step.
*/
Real Axis::getMajorStep() const
{
	return m_majorStep;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Set the major "auto step" flag.
/**
@param[in] The major "auto step" flag.
*/
void Axis::setMajorAutoStep(bool enabled)
{
	m_majorAutoStep = enabled;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Get the major "auto step" flag.
/**
@return The major "auto step" flag.
*/
bool Axis::getMajorAutoStep() const
{
	return m_majorAutoStep;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Set the major auto step hint.
/**
@param[in] hint The auto step hint.
*/
void Axis::setMajorAutoStepHint(Real step_hint)
{
	m_majorAutoStepHint = step_hint;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Get the major auto step hint.
/**
@return The auto step hint.
*/
Real Axis::getMajorAutoStepHint() const
{
	return m_majorAutoStepHint;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Set the background color.
/**
@param[in] color The background color.
*/
void Axis::setBackColor(const Color &color)
{
	m_backColor = color;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Get the background color.
/**
@return The background color.
*/
const Color& Axis::getBackColor() const
{
	return m_backColor;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Set the text color.
/**
@param[in] color The text color.
*/
void Axis::setTextColor(const Color &color)
{
	m_textColor = color;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Get the text color.
/**
@return The text color.
*/
const Color& Axis::getTextColor() const
{
	return m_textColor;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Set the axis name.
/**
@param[in] name The axis name.
*/
void Axis::setAxisName(const wchar_t *name)
{
	m_axisName = name;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Get the axis name.
/**
@return The axis name.
*/
const wchar_t* Axis::getAxisName() const
{
	return m_axisName.c_str();
}


//////////////////////////////////////////////////////////////////////////
/// @brief Set the text size.
/**
@param[in] size The text size in pixels.
*/
void Axis::setTextSize(Real size)
{
	m_textSize = size;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Get the text size.
/**
@return The text size in pixels.
*/
Real Axis::getTextSize() const
{
	return m_textSize;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Set the text format string.
/**
@param[in] format The text format string.
*/
void Axis::setTextFormat(const wchar_t *format)
{
	m_textFormat = format;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Get the text format string.
/**
@return The text format string.
*/
const wchar_t* Axis::getTextFormat() const
{
	return m_textFormat.c_str();
}


//////////////////////////////////////////////////////////////////////////
/// @brief Set the transformation offset.
/**
@param[in] offset The transformation offset.
*/
void Axis::setTextOffset(Real offset)
{
	m_textOffset = offset;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Get the transformation offset.
/**
@return The transformation offset.
*/
Real Axis::getTextOffset() const
{
	return m_textOffset;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Set the transformation scale.
/**
@param[in] scale The transformation scale.
*/
void Axis::setTextScale(Real scale)
{
	m_textScale = scale;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Get the transformation scale.
/**
@return The transformation scale.
*/
Real Axis::getTextScale() const
{
	return m_textScale;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Get transformed value.
/**
@param[in] x The "world" value.
@return The transformed value.
*/
Real Axis::transform(Real x) const
{
	return x*m_textScale
		+ m_textOffset;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Update major and minor steps.
/**
@param[in] wrange The "world" range.
@param[in] vrange The "view" range.
*/
void Axis::update(Real wrange, Real vrange)
{
	// major step
	if (m_majorAutoStep)
	{
		m_majorStep = auto_step(wrange,
			vrange, m_majorAutoStepHint);
	}

	// minor step
	if (m_minorAutoStep)
	{
		m_minorStep = m_majorStep
			* m_minorAutoStepHint;
	}
}


//////////////////////////////////////////////////////////////////////////
/// @brief Calculate auto step.
/**
@param[in] wrange The "world" range.
@param[in] vrange The "view" range.
@param[in] vstep The approximate "view" step.
*/
Real Axis::auto_step(Real wrange, Real vrange, Real vstep)
{
	int N_steps = int(vrange / vstep);
	if (N_steps <= 0) N_steps = 1;  // (?) min=1
	if (10 < N_steps) N_steps = 10; // (?) max=10

	Real step = wrange / N_steps;
	Real order = powf(10.0f, floorf(log10f(step)));

	step /= order;
	if (step < 1.00f) step = 1.00f;
	else if (step < 1.25f) step = 1.25f;
	else if (step < 2.00f) step = 2.00f;
	else if (step < 2.50f) step = 2.50f;
	else if (step < 5.00f) step = 5.00f;
	else                   step = 10.0f;

	return step * order;
}

	} // Axis


	// XYPlotter
	namespace plot
	{

//////////////////////////////////////////////////////////////////////////
/// @brief The X axis.
class XYPlotter::XAxis:
	public Axis
{
	typedef Axis inherited;
public:

	/// @brief The default constructor.
	XAxis()
	{
		m_minor_lines.set_axis(this);
		m_major_lines.set_axis(this);
		m_text.set_axis(this);
		setAxisName(L"X");
	}

public:

	///////////////////////////////////////////////////////////////////////////
	/// @brief The minor lines painter.
	class MinorLines:
		public Object
	{
	public:

		/// @brief The default constructor.
		MinorLines()
			: m_axis(0)
		{}

		/// @brief Set the axis object.
		/**
		@param[in] axis The axis.
		*/
		void set_axis(Axis *axis)
		{
			m_axis = axis;
		}

	public:

		/// @brief Draw the lines.
		/**
		@param[in] plotter The plotter.
		@param[in] canvas The canvas.
		*/
		virtual void draw(const Plotter &plotter, Canvas &canvas) const
		{
			const Real step = m_axis->getMinorStep();
			const Rect &W = plotter.world();

			// draw grid lines
			if (0.0 < step)
			{
				const Gdiplus::Pen *ppen = m_axis->getMinorPen();
				const Real Xstart = ceilf(W.Xmin()/step) * step;
				const Real Ymin = W.Ymin();
				const Real Ymax = W.Ymax();

				for (Real x = Xstart; x <= W.Xmax(); x += step)
				{
					const Point p1 = plotter.w2v(Point(x, Ymin));
					const Point p2 = plotter.w2v(Point(x, Ymax));

					canvas.DrawLine(ppen, p1, p2);
				}
			}
		}

	private:
		Axis *m_axis; ///< @brief The corresponding axis.
	};


	///////////////////////////////////////////////////////////////////////////
	/// @brief The major lines painter.
	class MajorLines:
		public Object
	{
	public:

		/// @brief The default constructor.
		MajorLines()
			: m_axis(0)
		{}

		/// @brief Set the axis object.
		/**
		@param[in] axis The axis.
		*/
		void set_axis(Axis *axis)
		{
			m_axis = axis;
		}

	public:

		/// @brief Draw the lines.
		/**
		@param[in] plotter The plotter.
		@param[in] canvas The canvas.
		*/
		virtual void draw(const Plotter &plotter, Canvas &canvas) const
		{
			const Real step = m_axis->getMajorStep();
			const Rect &W = plotter.world();

			// draw grid lines
			if (0.0 < step)
			{
				const Gdiplus::Pen *ppen = m_axis->getMajorPen();
				const Real Xstart = ceilf(W.Xmin()/step) * step;
				const Real Ymin = W.Ymin();
				const Real Ymax = W.Ymax();

				for (Real x = Xstart; x <= W.Xmax(); x += step)
				{
					const Point p1 = plotter.w2v(Point(x, Ymin));
					const Point p2 = plotter.w2v(Point(x, Ymax));

					canvas.DrawLine(ppen, p1, p2);
				}
			}
		}

	private:
		Axis *m_axis; ///< @brief The corresponding axis.
	};


	///////////////////////////////////////////////////////////////////////////
	/// @brief Text painter.
	class Text:
		public Object
	{
	public:

		/// @brief The default constructor.
		Text()
			: m_axis(0)
		{}

		/// @brief Set the axis object.
		/**
		@param[in] axis The axis.
		*/
		void set_axis(Axis *axis)
		{
			m_axis = axis;
		}

	public:
		enum { MARGIN = 2 };

	public:

		/// @brief Draw the text.
		/**
		@param[in] plotter The plotter.
		@param[in] canvas The canvas.
		*/
		virtual void draw(const Plotter &plotter, Canvas &canvas) const
		{
			const Real step = m_axis->getMajorStep();
			const Rect &W = plotter.world();
			const Rect &V = plotter.view();

			Gdiplus::Font font(L"Courier", m_axis->getTextSize(),
				Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
			Gdiplus::SolidBrush brush(m_axis->getTextColor());
			Gdiplus::StringFormat format;

			Gdiplus::RectF name_rc;
			{ // draw axis name
				const Rect vrc(Point(V.Xmin(), V.Ymax()),
					Size(V.Xsize(), m_axis->getTextSize() + 2*MARGIN));

				format.SetAlignment(Gdiplus::StringAlignmentFar);
				format.SetLineAlignment(Gdiplus::StringAlignmentCenter);

				canvas.MeasureString(m_axis->getAxisName(),
					-1, &font, vrc, &format, &name_rc);
				canvas.DrawString(m_axis->getAxisName(),
					-1, &font, vrc, &format, &brush);
			}

			// draw axis text
			if (0.0 < step)
			{
				const int BUF_SZ = 1024;
				wchar_t buf[BUF_SZ];

				const Real Xstart = ceilf(W.Xmin()/step) * step;
				for (Real x = Xstart; x <= W.Xmax(); x += step)
				{
					const Point vpt = plotter.w2v(Point(x, W.Ymin()));

					format.SetAlignment(Gdiplus::StringAlignmentCenter);
					format.SetLineAlignment(Gdiplus::StringAlignmentNear);

					Gdiplus::RectF hint_rc;

					_snwprintf(buf, BUF_SZ,
						m_axis->getTextFormat(),
						roundto(m_axis->transform(x), step/10.0f));

					canvas.MeasureString(buf, -1, &font,
						vpt, &format, &hint_rc);

					if (!hint_rc.IntersectsWith(name_rc))
						canvas.DrawString(buf, -1, &font,
							vpt, &format, &brush);
				}
			}
		}

	private:
		Axis *m_axis; ///< @brief The corresponding axis.
	};

public:

	/// @brief Update axis based on plotter.
	/**
	@param[in] plotter The plotter.
	*/
	void update(const Plotter &plotter)
	{
		inherited::update(
			plotter.world().Xsize(),
			plotter.view().Xsize()
		);
	}

public:

	/// @brief Get the minor lines painter.
	Object& minorLines()
	{
		return m_minor_lines;
	}

	/// @brief Get the major lines painter.
	Object& majorLines()
	{
		return m_major_lines;
	}

	/// @brief Get the text painter.
	Object& text()
	{
		return m_text;
	}

private:
	MinorLines m_minor_lines; ///< @brief The minor lines painter.
	MajorLines m_major_lines; ///< @brief The major lines painter.
	Text m_text;              ///< @brief The text painter.
};


//////////////////////////////////////////////////////////////////////////
/// @brief The Y axis.
class XYPlotter::YAxis:
	public Axis
{
	typedef Axis inherited;
public:

	/// @brief The default constructor.
	YAxis()
	{
		m_minor_lines.set_axis(this);
		m_major_lines.set_axis(this);
		m_text.set_axis(this);
		setAxisName(L"Y");
	}

public:

	//////////////////////////////////////////////////////////////////////////
	/// @brief The minor lines painter.
	class MinorLines
		: public Object
	{
	public:

		/// @brief The default constructor.
		MinorLines()
			: m_axis(0)
		{}

		/// @brief Set the axis object.
		/**
		@param[in] axis The axis.
		*/
		void set_axis(Axis *axis)
		{
			m_axis = axis;
		}

	public:

		/// @brief Draw the lines.
		/**
		@param[in] plotter The plotter.
		@param[in] canvas The canvas.
		*/
		virtual void draw(const Plotter &plotter, Canvas &canvas) const
		{
			const Real step = m_axis->getMinorStep();
			const Rect &W = plotter.world();

			// draw grid lines
			if (0.0 < step)
			{
				const Gdiplus::Pen *ppen = m_axis->getMinorPen();
				const Real Ystart = ceilf(W.Ymin()/step) * step;
				const Real Xmin = W.Xmin();
				const Real Xmax = W.Xmax();

				for (Real y = Ystart; y <= W.Ymax(); y += step)
				{
					const Point p1 = plotter.w2v(Point(Xmin, y));
					const Point p2 = plotter.w2v(Point(Xmax, y));

					canvas.DrawLine(ppen, p1, p2);
				}
			}
		}

	private:
		Axis *m_axis; ///< @brief The corresponding axis.
	};


	///////////////////////////////////////////////////////////////////////////
	/// @brief The major lines painter.
	class MajorLines:
		public Object
	{
	public:

		/// @brief The default constructor.
		MajorLines()
			: m_axis(0)
		{}

		/// @brief Set the axis object.
		/**
		@param[in] axis The axis.
		*/
		void set_axis(Axis *axis)
		{
			m_axis = axis;
		}

	public:

		/// @brief Draw the lines.
		/**
		@param[in] plotter The plotter.
		@param[in] canvas The canvas.
		*/
		virtual void draw(const Plotter &plotter, Canvas &canvas) const
		{
			const Real step = m_axis->getMajorStep();
			const Rect &W = plotter.world();

			// draw grid lines
			if (0.0 < step)
			{
				const Gdiplus::Pen *ppen = m_axis->getMajorPen();
				const Real Ystart = ceilf(W.Ymin()/step) * step;
				const Real Xmin = W.Xmin();
				const Real Xmax = W.Xmax();

				for (Real y = Ystart; y <= W.Ymax(); y += step)
				{
					const Point p1 = plotter.w2v(Point(Xmin, y));
					const Point p2 = plotter.w2v(Point(Xmax, y));

					canvas.DrawLine(ppen, p1, p2);
				}
			}
		}

	private:
		Axis *m_axis; ///< @brief The corresponding axis.
	};


	///////////////////////////////////////////////////////////////////////////
	/// @brief Text painter.
	class Text:
		public Object
	{
	public:

		/// @brief The default constructor.
		Text()
			: m_axis(0)
		{}

		/// @brief Set the axis object.
		/**
		@param[in] axis The axis.
		*/
		void set_axis(Axis *axis)
		{
			m_axis = axis;
		}

	public:
		enum { MARGIN = 2 };

	public:

		/// @brief Draw the text.
		/**
		@param[in] plotter The plotter.
		@param[in] canvas The canvas.
		*/
		virtual void draw(const Plotter &plotter, Canvas &canvas) const
		{
			const Real step = m_axis->getMajorStep();
			const Rect &W = plotter.world();
			const Rect &V = plotter.view();

			Gdiplus::Font font(L"Courier", m_axis->getTextSize(),
				Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
			Gdiplus::SolidBrush brush(m_axis->getTextColor());
			Gdiplus::StringFormat format;

			Gdiplus::GraphicsState state = canvas.Save();
			canvas.TranslateTransform(0.0f, V.Ysize());
			canvas.RotateTransform(-90.0f);

			Gdiplus::RectF name_rc;
			{ // draw axis name
				const Rect vrc(Point(0, 0),
					Size(V.Ysize(), m_axis->getTextSize() + 2*MARGIN));

				format.SetAlignment(Gdiplus::StringAlignmentFar);
				format.SetLineAlignment(Gdiplus::StringAlignmentCenter);

				canvas.MeasureString(m_axis->getAxisName(),
					-1, &font, vrc, &format, &name_rc);
				canvas.DrawString(m_axis->getAxisName(),
					-1, &font, vrc, &format, &brush);
			}

			// draw axis text
			if (0.0 < step)
			{
				const int BUF_SZ = 1024;
				wchar_t buf[BUF_SZ];

				const Real Ystart = ceilf(W.Ymin()/step) * step;
				for (Real y = Ystart; y <= W.Ymax(); y += step)
				{
					const Point vpt = Point(V.Ymax() - plotter.w2v(Point(0, y)).Y(), (Real)MARGIN);

					format.SetAlignment(Gdiplus::StringAlignmentCenter);
					format.SetLineAlignment(Gdiplus::StringAlignmentNear);

					Gdiplus::RectF hint_rc;

					_snwprintf(buf, BUF_SZ,
						m_axis->getTextFormat(),
						roundto(m_axis->transform(y), step/10.0f));

					canvas.MeasureString(buf, -1, &font,
						vpt, &format, &hint_rc);

					if (!hint_rc.IntersectsWith(name_rc))
						canvas.DrawString(buf, -1, &font,
							vpt, &format, &brush);
				}
			}

			canvas.Restore(state);
		}

	private:
		Axis *m_axis; ///< @brief The corresponding axis.
	};

public:

	/// @brief Update axis based on plotter.
	/**
	@param[in] plotter The plotter.
	*/
	void update(const Plotter &plotter)
	{
		inherited::update(
			plotter.world().Ysize(),
			plotter.view().Ysize());
	}


	/// @brief Get the minor lines painter.
	Object& minorLines()
	{
		return m_minor_lines;
	}

	/// @brief Get the major lines painter.
	Object& majorLines()
	{
		return m_major_lines;
	}

	/// @brief Get the text painter.
	Object& text()
	{
		return m_text;
	}

private:
	MinorLines m_minor_lines; ///< @brief The minor lines painter.
	MajorLines m_major_lines; ///< @brief The major lines painter.
	Text m_text;              ///< @brief The text painter.
};


//////////////////////////////////////////////////////////////////////////
/// @brief The main constructor.
/**
@param[in] parent The parent window handle.
@param[in] style The window style.
@param[in] ex_style The extended window style.
*/
XYPlotter::XYPlotter(HWND parent, DWORD style, DWORD ex_style)
	: inherited(parent, style, ex_style),
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
/// @brief The destructor.
XYPlotter::~XYPlotter()
{}


//////////////////////////////////////////////////////////////////////////
/// @brief Attach the graphic object.
/**
@param[in] obj The graphic object.
*/
void XYPlotter::attach(Object &obj)
{
	inherited::attach(obj, LEVEL_GRAPH);
}


//////////////////////////////////////////////////////////////////////////
/// @brief Attach the graphic object at specified level.
/**
@param[in] obj The graphic object.
@param[in] level The draw level.
*/
void XYPlotter::attach(Object &obj, Level level)
{
	inherited::attach(obj, level);
}


//////////////////////////////////////////////////////////////////////////
/// @brief Detach the graphic object.
/**
@param[in] obj The graphic object.
*/
void XYPlotter::detach(Object &obj)
{
	inherited::detach(obj);
}


//////////////////////////////////////////////////////////////////////////
/// @brief Get the X axis (read-only).
/**
@return The X axis.
*/
const Axis& XYPlotter::Xaxis() const
{
	return *m_Xaxis;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Get the X axis.
/**
@return The X axis.
*/
Axis& XYPlotter::Xaxis()
{
	return *m_Xaxis;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Get the Y axis (read-only).
/**
@return The Y axis.
*/
const Axis& XYPlotter::Yaxis() const
{
	return *m_Yaxis;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Get the Y axis.
/**
@return The Y axis.
*/
Axis& XYPlotter::Yaxis()
{
	return *m_Yaxis;
}


//////////////////////////////////////////////////////////////////////////
/// @brief The "world changed" event.
void XYPlotter::on_world_changed()
{
	inherited::on_world_changed();

	//const Real dx = m_Xaxis->textSize() + 2*XAxis::Text::MARGIN;
	//const Real dy = m_Yaxis->textSize() + 2*YAxis::Text::MARGIN;

	//const Real Xscale = dx / view().Xsize();
	//const Real Yscale = dy / view().Ysize();

	//const Real Xmin = (W.Xmin() - W.Xmax()*Xscale) / (1.0 - Xscale);
	//const Real Ymin = (W.Ymin() - W.Ymax()*Yscale) / (1.0 - Yscale);

	//inherited::show(Rect(Point(Xmin, Ymin),
	//	Point(W.Xmax(), W.Ymax())));

	m_Xaxis->update(*this);
	m_Yaxis->update(*this);
}


//////////////////////////////////////////////////////////////////////////
/// @brief The "full changed" event.
void XYPlotter::on_full_changed()
{
	inherited::on_full_changed();
}


//////////////////////////////////////////////////////////////////////////
/// @brief The "view changing" event.
/**
@param[in,out] x The X coordinate of "view" rectangle.
@param[in,out] y The Y coordinate of "view" rectangle.
@param[in,out] w The width of "view" rectangle.
@param[in,out] h The height of "view" rectangle.
*/
void XYPlotter::on_view_changing(Real &x, Real &y, Real &w, Real &h)
{
	const Real dx = m_Xaxis->getTextSize() + YAxis::Text::MARGIN*2;
	const Real dy = m_Yaxis->getTextSize() + XAxis::Text::MARGIN*2;

	// TODO: axis position?
	x += dx;
	w -= dx;
	h -= dy;
}

	} // XYPlotter


	// PolarPlotter
	namespace plot
	{

//////////////////////////////////////////////////////////////////////////
/// @brief The radial axis.
class PolarPlotter::RadialAxis:
	public Axis
{
	typedef Axis inherited;
public:

	/// @brief The default constructor.
	RadialAxis()
	{
		m_minor_lines.set_axis(this);
		m_major_lines.set_axis(this);
		m_text.set_axis(this);
		setAxisName(L"R");
	}

public:

	//////////////////////////////////////////////////////////////////////////
	/// @brief The minor lines painter.
	class MinorLines:
		public Object
	{
	public:

		/// @brief The default constructor.
		MinorLines()
			: m_axis(0)
		{}

		/// @brief Set the axis object.
		/**
		@param[in] axis The axis.
		*/
		void set_axis(Axis *axis)
		{
			m_axis = axis;
		}

	public:

		/// @brief Draw the lines.
		/**
		@param[in] plotter The plotter.
		@param[in] canvas The canvas.
		*/
		virtual void draw(const Plotter &plotter, Canvas &canvas) const
		{
			const Real step = m_axis->getMinorStep();
			const Rect &W = plotter.world();

			// draw grid lines
			if (0.0 < step)
			{
				const Gdiplus::Pen *ppen = m_axis->getMajorPen();
				const Real ONE = __max(W.Xsize(), W.Ysize())/2;
				const Point p0(0, 0);// = W.center();

				for (Real r = step; r <= ONE; r += step)
				{
					Rect rc(p0, Size(2*r, 2*r));
					rc.offset(-r, -r);

					canvas.DrawEllipse(ppen,
						plotter.w2v(rc));
				}
			}
		}

	private:
		Axis *m_axis; ///< @brief The corresponding axis.
	};


	//////////////////////////////////////////////////////////////////////////
	/// @brief The major lines painter.
	class MajorLines:
		public Object
	{
	public:

		/// @brief The default constructor.
		MajorLines()
			: m_axis(0)
		{}

		/// @brief Set the axis object.
		/**
		@param[in] axis The axis.
		*/
		void set_axis(Axis *axis)
		{
			m_axis = axis;
		}

	public:

		/// @brief Draw the lines.
		/**
		@param[in] plotter The plotter.
		@param[in] canvas The canvas.
		*/
		virtual void draw(const Plotter &plotter, Canvas &canvas) const
		{
			const Real step = m_axis->getMajorStep();
			const Rect &W = plotter.world();

			// draw grid lines
			if (0.0 < step)
			{
				const Gdiplus::Pen *ppen = m_axis->getMajorPen();
				const Real ONE = __max(W.Xsize(), W.Ysize())/2;
				const Point p0(0, 0);// = W.center();

				for (Real r = step; r <= ONE; r += step)
				{
					Rect rc(p0, Size(2*r, 2*r));
					rc.offset(-r, -r);

					canvas.DrawEllipse(ppen,
						plotter.w2v(rc));
				}
			}
		}

	private:
		Axis *m_axis; ///< @brief The corresponding axis.
	};


	//////////////////////////////////////////////////////////////////////////
	/// @brief Text painter.
	class Text:
		public Object
	{
	public:

		/// @brief The default constructor.
		Text()
			: m_axis(0)
		{}

		/// @brief Set the axis object.
		/**
		@param[in] axis The axis.
		*/
		void set_axis(Axis *axis)
		{
			m_axis = axis;
		}

	public:
		enum { MARGIN = 2 };

	public:

		/// @brief Draw the text.
		/**
		@param[in] plotter The plotter.
		@param[in] canvas The canvas.
		*/
		virtual void draw(const Plotter &plotter, Canvas &canvas) const
		{
			const Real step = m_axis->getMajorStep();
			const Rect &W = plotter.world();

			const Real ONE = __max(W.Xsize(), W.Ysize())/2;
			const Point p0(0, 0);// = W.center();

			Gdiplus::Font font(L"Courier", m_axis->getTextSize(),
				Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
			Gdiplus::SolidBrush brush(m_axis->getTextColor());
			Gdiplus::StringFormat format;

			Gdiplus::RectF name_rc;
			{ // draw axis name
				const Point vp = plotter.w2v(p0 + Size(ONE, 0));

				format.SetAlignment(Gdiplus::StringAlignmentFar);
				format.SetLineAlignment(Gdiplus::StringAlignmentFar);

				canvas.MeasureString(m_axis->getAxisName(),
					-1, &font, vp, &format, &name_rc);
				canvas.DrawString(m_axis->getAxisName(),
					-1, &font, vp, &format, &brush);
			}

			// draw grid lines
			if (0.0 < step)
			{
				const int BUF_SZ = 1024;
				wchar_t buf[BUF_SZ];

				for (Real r = step; r <= ONE; r += step)
				{
					const Point vp = plotter.w2v(p0 + Size(r, 0));

					format.SetAlignment(Gdiplus::StringAlignmentFar);
					format.SetLineAlignment(Gdiplus::StringAlignmentNear);

					_snwprintf(buf, BUF_SZ,
						m_axis->getTextFormat(),
						roundto(m_axis->transform(r), step/10.0f));

					canvas.DrawString(buf, -1, &font,
						vp, &format, &brush);
				}
			}
		}

	private:
		Axis *m_axis; ///< @brief The corresponding axis.
	};

public:

	/// @brief Update axis based on plotter.
	/**
	@param[in] plotter The plotter.
	*/
	void update(const Plotter &plotter)
	{
		inherited::update(
			plotter.world().Xsize(),
			plotter.view().Xsize()
		);
	}

	/// @brief Get the minor lines painter.
	Object& minorLines()
	{
		return m_minor_lines;
	}

	/// @brief Get the major lines painter.
	Object& majorLines()
	{
		return m_major_lines;
	}

	/// @brief Get the text painter.
	Object& text()
	{
		return m_text;
	}

private:
	MinorLines m_minor_lines; ///< @brief The minor lines painter.
	MajorLines m_major_lines; ///< @brief The major lines painter.
	Text m_text;              ///< @brief The text painter.
};


//////////////////////////////////////////////////////////////////////////
/// @brief The angular axis.
class PolarPlotter::AngularAxis:
	public Axis
{
	typedef Axis inherited;
public:

	/// @brief The default constructor.
	AngularAxis()
	{
		m_minor_lines.set_axis(this);
		m_major_lines.set_axis(this);
		m_text.set_axis(this);
		setAxisName(L"");
	}

public:

	//////////////////////////////////////////////////////////////////////////
	/// @brief The minor lines painter.
	class MinorLines
		: public Object
	{
	public:

		/// @brief The default constructor.
		MinorLines()
			: m_axis(0)
		{}

		/// @brief Set the axis object.
		/**
		@param[in] axis The axis.
		*/
		void set_axis(Axis *axis)
		{
			m_axis = axis;
		}

	public:

		/// @brief Draw the lines.
		/**
		@param[in] plotter The plotter.
		@param[in] canvas The canvas.
		*/
		virtual void draw(const Plotter &plotter, Canvas &canvas) const
		{
			const Real step = m_axis->getMinorStep();
			const Rect &W = plotter.world();

			// draw grid lines
			if (0.0 < step)
			{
			}
		}

	private:
		Axis *m_axis; ///< @brief The corresponding axis.
	};


	///////////////////////////////////////////////////////////////////////////
	/// @brief The major lines painter.
	class MajorLines:
		public Object
	{
	public:

		/// @brief The default constructor.
		MajorLines()
			: m_axis(0)
		{}

		/// @brief Set the axis object.
		/**
		@param[in] axis The axis.
		*/
		void set_axis(Axis *axis)
		{
			m_axis = axis;
		}

	public:

		/// @brief Draw the lines.
		/**
		@param[in] plotter The plotter.
		@param[in] canvas The canvas.
		*/
		virtual void draw(const Plotter &plotter, Canvas &canvas) const
		{
			const Real step = m_axis->getMajorStep();
			const Rect &W = plotter.world();

			// draw major lines
			if (0.0 < step)
			{
				const Gdiplus::Pen *ppen = m_axis->getMajorPen();
				const Real ONE = __max(W.Xsize(), W.Ysize())/2;
				const Point p0(0, 0);// = W.center();

				for (Real phi = 0.0; phi < 2*M_PI; phi += step)
				{
					const Real x = ONE*cosf(phi);
					const Real y = ONE*sinf(phi);

					const Point p1 = plotter.w2v(p0);
					const Point p2 = plotter.w2v(p0 + Size(x, y));

					canvas.DrawLine(ppen, p1, p2);
				}
			}
		}

	private:
		Axis *m_axis; ///< @brief The corresponding axis.
	};


	///////////////////////////////////////////////////////////////////////////
	/// @brief Text painter.
	class Text:
		public Object
	{
	public:

		/// @brief The default constructor.
		Text()
			: m_axis(0)
		{}

		/// @brief Set the axis object.
		/**
		@param[in] axis The axis.
		*/
		void set_axis(Axis *axis)
		{
			m_axis = axis;
		}

	public:
		enum { MARGIN = 2 };

	public:

		/// @brief Draw the text.
		/**
		@param[in] plotter The plotter.
		@param[in] canvas The canvas.
		*/
		virtual void draw(const Plotter &plotter, Canvas &canvas) const
		{
			const Real step = m_axis->getMajorStep();
			const Rect &W = plotter.world();

			const Real ONE = __max(W.Xsize(), W.Ysize())/2;
			const Point p0(0, 0);// = W.center();

			Gdiplus::Font font(L"Courier", m_axis->getTextSize(),
				Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
			Gdiplus::SolidBrush brush(m_axis->getTextColor());
			Gdiplus::StringFormat format(Gdiplus::StringFormatFlagsDirectionVertical);

			Gdiplus::RectF name_rc;
			if (0)
			{ // draw axis name
				const Point vp = plotter.w2v(p0 + Size(ONE, 0));

				format.SetAlignment(Gdiplus::StringAlignmentFar);
				format.SetLineAlignment(Gdiplus::StringAlignmentFar);

				canvas.MeasureString(m_axis->getAxisName(),
					-1, &font, vp, &format, &name_rc);
				canvas.DrawString(m_axis->getAxisName(),
					-1, &font, vp, &format, &brush);
			}

			// draw grid lines
			if (0.0 < step)
			{
				const Point vp0 = plotter.w2v(p0);
				const Point vp1 = Point() + plotter.w2v(Size(ONE, 0));

				const int BUF_SZ = 1024;
				wchar_t buf[BUF_SZ];

				for (Real phi = 0.0; phi < 2*M_PI - step/2; phi += step)
				{
					Gdiplus::GraphicsState state = canvas.Save();
					canvas.ResetTransform();
					canvas.TranslateTransform(vp0.X(), vp0.Y());
					canvas.RotateTransform((Real)omni::util::rad2deg(-phi));

					format.SetAlignment(Gdiplus::StringAlignmentCenter);
					format.SetLineAlignment(Gdiplus::StringAlignmentNear);

					_snwprintf(buf, BUF_SZ,
						m_axis->getTextFormat(),
						omni::util::rad2deg(roundto(m_axis->transform(phi), step/10.0f)));

					canvas.DrawString(buf, -1, &font,
						vp1, &format, &brush);

					canvas.Restore(state);
				}
			}
		}

	private:
		Axis *m_axis; ///< @brief The corresponding axis.
	};

public:

	/// @brief Update axis based on plotter.
	/**
	@param[in] plotter The plotter.
	*/
	void update(const Plotter &plotter)
	{
		inherited::update(
			plotter.world().Ysize(),
			plotter.view().Ysize());
	}

	/// @brief Get the minor lines painter.
	Object& minorLines()
	{
		return m_minor_lines;
	}

	/// @brief Get the major lines painter.
	Object& majorLines()
	{
		return m_major_lines;
	}

	/// @brief Get the text painter.
	Object& text()
	{
		return m_text;
	}

private:
	MinorLines m_minor_lines; ///< @brief The minor lines painter.
	MajorLines m_major_lines; ///< @brief The major lines painter.
	Text m_text;              ///< @brief The text painter.
};


//////////////////////////////////////////////////////////////////////////
/// @brief The main constructor.
PolarPlotter::PolarPlotter(HWND parent, DWORD style, DWORD ex_style)
	: inherited(parent, style, ex_style),
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
/// @brief The destructor.
PolarPlotter::~PolarPlotter()
{}


//////////////////////////////////////////////////////////////////////////
/// @brief Attach the graphic object.
/**
@param[in] obj The graphic object.
*/
void PolarPlotter::attach(Object &obj)
{
	inherited::attach(obj, LEVEL_GRAPH);
}


//////////////////////////////////////////////////////////////////////////
/// @brief Attach the graphic object at specified level.
/**
@param[in] obj The graphic object.
@param[in] level The draw level.
*/
void PolarPlotter::attach(Object &obj, Level level)
{
	inherited::attach(obj, level);
}


//////////////////////////////////////////////////////////////////////////
/// @brief Detach the graphic object.
/**
@param[in] obj The graphic object.
*/
void PolarPlotter::detach(Object &obj)
{
	inherited::detach(obj);
}


//////////////////////////////////////////////////////////////////////////
/// @brief Get the radial axis (read-only).
/**
@return The radial axis.
*/
const Axis& PolarPlotter::radialAxis() const
{
	return *m_radialAxis;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Get the radial axis.
/**
@return The radial axis.
*/
Axis& PolarPlotter::radialAxis()
{
	return *m_radialAxis;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Get the angular axis (read-only).
/**
@return The angular axis.
*/
const Axis& PolarPlotter::angularAxis() const
{
	return *m_angularAxis;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Get the angular axis.
/**
@return The angular axis.
*/
Axis& PolarPlotter::angularAxis()
{
	return *m_angularAxis;
}


//////////////////////////////////////////////////////////////////////////
/// @brief The "world changed" event.
void PolarPlotter::on_world_changed()
{
	inherited::on_world_changed();

	//const Real dx = m_Xaxis->textSize() + 2*XAxis::Text::MARGIN;
	//const Real dy = m_Yaxis->textSize() + 2*YAxis::Text::MARGIN;

	//const Real Xscale = dx / view().X();
	//const Real Yscale = dy / view().Y();

	//const Real Xmin = (W.Xmin() - W.Xmax()*Xscale) / (1.0 - Xscale);
	//const Real Ymin = (W.Ymin() - W.Ymax()*Yscale) / (1.0 - Yscale);

	//inherited::show(Rect(Point(Xmin, Ymin),
	//	Point(W.Xmax(), W.Ymax())));

	m_angularAxis->update(*this);
	m_radialAxis->update(*this);
}


//////////////////////////////////////////////////////////////////////////
/// @brief The "full changed" event.
void PolarPlotter::on_full_changed()
{
	inherited::on_full_changed();
}


//////////////////////////////////////////////////////////////////////////
/// @brief The "view changing" event.
/**
@param[in,out] x The X coordinate of "view" rectangle.
@param[in,out] y The Y coordinate of "view" rectangle.
@param[in,out] w The width of "view" rectangle.
@param[in,out] h The height of "view" rectangle.
*/
void PolarPlotter::on_view_changing(Real &x, Real &y, Real &w, Real &h)
{
	const Real dx = m_angularAxis->getTextSize() + AngularAxis::Text::MARGIN*2;
	const Real dy = m_radialAxis->getTextSize() + RadialAxis::Text::MARGIN*2;

	// TODO: axis position?
	x += dx;
	w -= dx;
	h -= dy;
}

	} // PolarPlotter


	// Marker
	namespace plot
	{

//////////////////////////////////////////////////////////////////////////
/// @brief The default constructor.
/**
	By default the size is 6.
*/
Marker::Marker()
	: m_pen(new Gdiplus::Pen(Color::White)),
	  m_brush(new Gdiplus::SolidBrush(Color::Black)),
	  m_size(6.0f)
{}


//////////////////////////////////////////////////////////////////////////
/// @brief The copy constructor.
/**
@param[in] other The marker to copy.
*/
Marker::Marker(const Marker &other)
	: m_pen(other.getPen()->Clone()),
	  m_brush(other.getBrush()->Clone()),
	  m_size(other.getSize())
{}


//////////////////////////////////////////////////////////////////////////
/// @brief Set the new pen.
/**
@param[in] pen The new pen.
*/
void Marker::setPen(const Gdiplus::Pen &pen)
{
	m_pen.reset(pen.Clone());
}


//////////////////////////////////////////////////////////////////////////
/// @brief Get the pen.
/**
@return The pen.
*/
const Gdiplus::Pen* Marker::getPen() const
{
	return m_pen.get();
}


//////////////////////////////////////////////////////////////////////////
/// @brief Get the pen.
/**
@return The pen.
*/
Gdiplus::Pen* Marker::getPen()
{
	return m_pen.get();
}


//////////////////////////////////////////////////////////////////////////
/// @brief Set the new brush.
/**
@return The new brush.
*/
void Marker::setBrush(const Gdiplus::Brush &brush)
{
	m_brush.reset(brush.Clone());
}


//////////////////////////////////////////////////////////////////////////
/// @brief Get the brush.
/**
@return The brush.
*/
const Gdiplus::Brush* Marker::getBrush() const
{
	return m_brush.get();
}


//////////////////////////////////////////////////////////////////////////
/// @brief Get the brush.
/**
@return The brush.
*/
Gdiplus::Brush* Marker::getBrush()
{
	return m_brush.get();
}


//////////////////////////////////////////////////////////////////////////
/// @brief Set the new size.
/**
@return The new size.
*/
void Marker::setSize(Real size)
{
	m_size = size;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Get the size.
/**
@return The size.
*/
Real Marker::getSize() const
{
	return m_size;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Start drawing.
/**
@param[in] plotter The plotter.
@param[in] canvas The canvas.
*/
void Marker::start(const Plotter &plotter, Canvas &canvas) const
{
	// DO NOTHING
}


//////////////////////////////////////////////////////////////////////////
/// @brief Draw the marker.
/**
@param[in] plotter The plotter.
@param[in] canvas The canvas.
@param[in] x The X coordinate.
@param[in] y The Y coordinate.
*/
void Marker::draw(const Plotter &plotter, Canvas &canvas, Real x, Real y) const
{
	// DO NOTHING
}


//////////////////////////////////////////////////////////////////////////
/// @brief Stop drawing.
/**
@param[in] plotter The plotter.
@param[in] canvas The canvas.
*/
void Marker::finish(const Plotter &plotter, Canvas &canvas) const
{
	// DO NOTHING
}


//////////////////////////////////////////////////////////////////////////
/// @brief Clone the marker.
/**
@return The marker's clone.
*/
Marker* Marker::clone() const
{
	return 0;
}

	} // Marker


	// SquareMarker
	namespace plot
	{

//////////////////////////////////////////////////////////////////////////
/// @brief The default constructor.
SquareMarker::SquareMarker()
{}


//////////////////////////////////////////////////////////////////////////
/// @brief The copy constructor.
/**
@param[in] other The marker to copy.
*/
SquareMarker::SquareMarker(const SquareMarker &other)
	: inherited(other)
{}


//////////////////////////////////////////////////////////////////////////
/// @brief Start drawing.
/**
@param[in] plotter The plotter.
@param[in] canvas The canvas.
*/
void SquareMarker::start(const Plotter &plotter, Canvas &canvas) const
{}


//////////////////////////////////////////////////////////////////////////
/// @brief Draw the marker.
/**
@param[in] plotter The plotter.
@param[in] canvas The canvas.
@param[in] x The X coordinate.
@param[in] y The Y coordinate.
*/
void SquareMarker::draw(const Plotter &plotter, Canvas &canvas, Real x, Real y) const
{
	const Real S = getSize();
	Gdiplus::RectF rc(-S/2, -S/2, S, S);

	Gdiplus::GraphicsState state = canvas.Save();
	canvas.ResetTransform();
	canvas.TranslateTransform(x, y);

	canvas.FillRectangle(getBrush(), rc);
	canvas.DrawRectangle(getPen(), rc);

	canvas.Restore(state);
}


//////////////////////////////////////////////////////////////////////////
/// @brief Stop drawing.
/**
@param[in] plotter The plotter.
@param[in] canvas The canvas.
*/
void SquareMarker::finish(const Plotter &plotter, Canvas &canvas) const
{}


//////////////////////////////////////////////////////////////////////////
/// @brief Clone the marker.
/**
@return The marker's clone.
*/
Marker* SquareMarker::clone() const
{
	return new SquareMarker(*this);
}

	} // SquareMarker


	// CircleMarker
	namespace plot
	{

//////////////////////////////////////////////////////////////////////////
/// @brief The default constructor.
CircleMarker::CircleMarker()
{}


//////////////////////////////////////////////////////////////////////////
/// @brief The copy constructor.
/**
@param[in] other The marker to copy.
*/
CircleMarker::CircleMarker(const CircleMarker &other)
	: inherited(other)
{}


//////////////////////////////////////////////////////////////////////////
/// @brief Start drawing.
/**
@param[in] plotter The plotter.
@param[in] canvas The canvas.
*/
void CircleMarker::start(const Plotter &plotter, Canvas &canvas) const
{}


//////////////////////////////////////////////////////////////////////////
/// @brief Draw the marker.
/**
@param[in] plotter The plotter.
@param[in] canvas The canvas.
@param[in] x The X coordinate.
@param[in] y The Y coordinate.
*/
void CircleMarker::draw(const Plotter &plotter, Canvas &canvas, Real x, Real y) const
{
	const Real S = getSize();
	Gdiplus::RectF rc(-S/2, -S/2, S, S);

	Gdiplus::GraphicsState state = canvas.Save();
	canvas.ResetTransform();
	canvas.TranslateTransform(x, y);

	canvas.FillEllipse(getBrush(), rc);
	canvas.DrawEllipse(getPen(), rc);

	canvas.Restore(state);
}


//////////////////////////////////////////////////////////////////////////
/// @brief Stop drawing.
/**
@param[in] plotter The plotter.
@param[in] canvas The canvas.
*/
void CircleMarker::finish(const Plotter &plotter, Canvas &canvas) const
{}


//////////////////////////////////////////////////////////////////////////
/// @brief Clone the marker.
/**
@return The marker's clone.
*/
Marker* CircleMarker::clone() const
{
	return new CircleMarker(*this);
}

	} // CircleMarker


	// StarMarker
	namespace plot
	{


//////////////////////////////////////////////////////////////////////////
/// @brief The main constructor.
/**
@param[in] Nangles The number of angles.
	By default is 5.
*/
StarMarker::StarMarker(size_t Nanlges)
	: m_Nangles(Nanlges),
	  m_figureSize(0) // will be updated during first draw
{
	assert(3 <= Nanlges && "number of angles is too small");
	if (m_Nangles < 3)
		m_Nangles = 3;
}


//////////////////////////////////////////////////////////////////////////
/// @brief The copy constructor.
/**
@param[in] other The marker to copy.
*/
StarMarker::StarMarker(const StarMarker &other)
	: inherited(other),
	  m_Nangles(other.m_Nangles),
	  m_figureSize(0) // will be updated during first draw
{}


//////////////////////////////////////////////////////////////////////////
/// @brief Start drawing.
/**
@param[in] plotter The plotter.
@param[in] canvas The canvas.
*/
void StarMarker::start(const Plotter &plotter, Canvas &canvas) const
{}


//////////////////////////////////////////////////////////////////////////
/// @brief Draw the marker.
/**
@param[in] plotter The plotter.
@param[in] canvas The canvas.
@param[in] x The X coordinate.
@param[in] y The Y coordinate.
*/
void StarMarker::draw(const Plotter &plotter, Canvas &canvas, Real x, Real y) const
{
	const Real S = getSize();
	if (fabsf(S - m_figureSize) > FLT_EPSILON)
	{
		m_figureSize = S;
		updateFigure();
	}

	Gdiplus::GraphicsState state = canvas.Save();
	canvas.ResetTransform();
	canvas.TranslateTransform(x, y);

	canvas.FillPath(getBrush(), &m_figure);
	canvas.DrawPath(getPen(), &m_figure);

	canvas.Restore(state);
}


//////////////////////////////////////////////////////////////////////////
/// @brief Stop drawing.
/**
@param[in] plotter The plotter.
@param[in] canvas The canvas.
*/
void StarMarker::finish(const Plotter &plotter, Canvas &canvas) const
{}


//////////////////////////////////////////////////////////////////////////
/// @brief Clone the marker.
/**
@return The marker's clone.
*/
Marker* StarMarker::clone() const
{
	return new StarMarker(*this);
}


//////////////////////////////////////////////////////////////////////////
/// @brief Update figure.
void StarMarker::updateFigure() const
{
	const Real S = getSize();

	m_figure.Reset();

	const int N = m_Nangles;
	const Real step = Real(2*M_PI / N);
	const Real Rmin = 0.5f*S;
	const Real Rmax = 1.0f*S;

	m_figure.StartFigure();
	for (int i = 0; i < N; ++i)
	{
		const Real A = Real(i*step - M_PI/2);

		const Point p1(Rmin*cosf(A-step/2), Rmin*sinf(A-step/2));
		const Point p2(Rmax*cosf(A), Rmax*sinf(A));
		const Point p3(Rmin*cosf(A+step/2), Rmin*sinf(A+step/2));

		m_figure.AddLine(p1, p2);
		m_figure.AddLine(p2, p3);
	}
	m_figure.CloseFigure();
}

	} // StarMarker


	// LineGraph
	namespace plot
	{

//////////////////////////////////////////////////////////////////////////
/// @brief The default constructor.
LineGraph::LineGraph()
	: m_pen(new Gdiplus::Pen(Color::White, 3.0f)),
	  m_smoothCurve(true)
{}


//////////////////////////////////////////////////////////////////////////
/// @brief The destructor.
LineGraph::~LineGraph()
{}


//////////////////////////////////////////////////////////////////////////
/// @brief Set the new pen.
/**
@param[in] pen The new pen.
*/
void LineGraph::setPen(const Gdiplus::Pen &pen)
{
	m_pen.reset(pen.Clone());
}


//////////////////////////////////////////////////////////////////////////
/// @brief Get the pen.
/**
@return The pen.
*/
const Gdiplus::Pen* LineGraph::getPen() const
{
	return m_pen.get();
}


//////////////////////////////////////////////////////////////////////////
/// @brief Get the pen.
/**
@return The pen.
*/
Gdiplus::Pen* LineGraph::getPen()
{
	return m_pen.get();
}


//////////////////////////////////////////////////////////////////////////
/// @brief Set the new marker
void LineGraph::setMarker(const Marker &marker)
{
	m_marker.reset(marker.clone());
}


//////////////////////////////////////////////////////////////////////////
/// @brief Get the current marker.
/**
@return The current marker or null.
*/
const Marker* LineGraph::getMarker() const
{
	return m_marker.get();
}


//////////////////////////////////////////////////////////////////////////
/// @brief Get the current marker.
/**
@return The current marker or null.
*/
Marker* LineGraph::getMarker()
{
	return m_marker.get();
}


//////////////////////////////////////////////////////////////////////////
/// @brief Set the "smooth curve" flag.
/**
@param[in] smooth The "smooth curve" flag.
*/
void LineGraph::setSmoothCurve(bool smooth)
{
	m_smoothCurve = smooth;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Get the "smooth curve" flag.
/**
@return The "smooth curve" flag.
*/
bool LineGraph::isSmoothCurve() const
{
	return m_smoothCurve;
}



//////////////////////////////////////////////////////////////////////////
/// @brief Assign new points.
/**
@param[in] Npoints The number of points.
@param[in] XYs The points.
*/
void LineGraph::assign(size_t Npoints, const Point *XYs)
{
	m_wpoints.assign(XYs,
		XYs + Npoints);

	update();
}


//////////////////////////////////////////////////////////////////////////
/// @brief Assign new points.
/**
@param[in] Npoints The number of points.
@param[in] Ys The Y values.
@param[in] Xs The X values.
*/
void LineGraph::assign(size_t Npoints, const Real *Ys, const Real *Xs)
{
	m_wpoints.resize(Npoints);
	for (size_t i = 0; i < Npoints; ++i)
		m_wpoints[i] = Point(Xs[i], Ys[i]);

	update();
}


//////////////////////////////////////////////////////////////////////////
/// @brief Assign new points.
/**
@param[in] Npoints The number of points.
@param[in] Ys The Y values.
@param[in] Xstart The first X value.
@param[in] Xstep The X step.
*/
void LineGraph::assign(size_t Npoints, const Real *Ys, Real Xstart, Real Xstep)
{
	m_wpoints.resize(Npoints);
	for (size_t i = 0; i < Npoints; ++i)
		m_wpoints[i] = Point(Xstart + i*Xstep, Ys[i]);

	update();
}


//////////////////////////////////////////////////////////////////////////
/// @brief Add one point.
/**
@param[in] pt The point.
*/
void LineGraph::push_back(const Point &pt)
{
	// update "full" area
	if (m_wpoints.empty())
		m_full = Rect(pt, Size());
	else
		m_full |= pt;

	m_wpoints.push_back(pt);
}


//////////////////////////////////////////////////////////////////////////
/// @brief Add one point.
/**
@param[in] x The X value.
@param[in] y The Y value.
*/
void LineGraph::push_back(Real x, Real y)
{
	push_back(Point(x, y));
}


//////////////////////////////////////////////////////////////////////////
/// @brief Clear all points.
void LineGraph::clear()
{
	m_wpoints.clear();
	m_vpoints.clear();
	m_full = Rect();
}


//////////////////////////////////////////////////////////////////////////
/// @brief Draw the line graph.
/**
@param[in] plotter The plotter.
@param[in] canvas The canvas.
*/
void LineGraph::draw(const Plotter &plotter, Canvas &canvas) const
{
	const size_t Npoints = m_wpoints.size();

	{ // draw line
		m_vpoints.resize(Npoints);
		for (size_t i = 0; i < Npoints; ++i)
			m_vpoints[i] = plotter.w2v(m_wpoints[i]);

		if (isSmoothCurve()) // smooth curve
		{
			canvas.DrawCurve(m_pen.get(),
				&m_vpoints[0], Npoints);
		}
		else // lines
		{
			canvas.DrawLines(m_pen.get(),
				&m_vpoints[0], Npoints);
		}
	}

	// draw markers
	if (const Marker *M = getMarker())
	{
		const Size SCALE = plotter.v2w(Size(1.0, 1.0));

		M->start(plotter, canvas);
		for (size_t i = 0; i < Npoints; ++i)
		{
			const Gdiplus::PointF &pt = m_vpoints[i];
			M->draw(plotter, canvas, pt.X, pt.Y);
		}
		M->finish(plotter, canvas);
	}
}


//////////////////////////////////////////////////////////////////////////
/// @brief Hit testing.
/**
@param[in] pt The point of interest.
@param[in] eps The epsilon.
@return @b true if point is inside the line graph.
*/
bool LineGraph::hit_test(const Point &pt, Real eps) const
{
	// TODO: hit testing for LineGraph
	return false;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Get the "full" rectangle.
/**
@return The "full" rectangle.
*/
const Rect LineGraph::full() const
{
	return m_full;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Update the "full" rectangle.
void LineGraph::update()
{
	const size_t N = m_wpoints.size();

	if (0 < N)
	{
		const Point &pt = m_wpoints.front();
		m_full = Rect(pt, Size());

		for (size_t i = 1; i < N; ++i)
			m_full |= m_wpoints[i];
	}
	else
		m_full = Rect();
}

	} // LineGraph


	// HistGraph
	namespace plot
	{

//////////////////////////////////////////////////////////////////////////
/// @brief The default constructor.
HistGraph::HistGraph()
	: m_zeroLevel(0), m_barWidth(1),
	  m_pen(new Gdiplus::Pen(Color::White, 2.0f)),
	  m_brush(new Gdiplus::SolidBrush(Color::Gray))
{}


//////////////////////////////////////////////////////////////////////////
/// @brief The destructor.
HistGraph::~HistGraph()
{}


//////////////////////////////////////////////////////////////////////////
/// @brief Set the new pen.
/**
@param[in] pen The new pen.
*/
void HistGraph::setPen(const Gdiplus::Pen &pen)
{
	m_pen.reset(pen.Clone());
}


//////////////////////////////////////////////////////////////////////////
/// @brief Get the pen.
/**
@return The pen.
*/
const Gdiplus::Pen* HistGraph::getPen() const
{
	return m_pen.get();
}


//////////////////////////////////////////////////////////////////////////
/// @brief Get the pen.
/**
@return The pen.
*/
Gdiplus::Pen* HistGraph::getPen()
{
	return m_pen.get();
}


//////////////////////////////////////////////////////////////////////////
/// @brief Set the new brush.
/**
@param[in] brush The new brush.
*/
void HistGraph::setBrush(const Gdiplus::Brush &brush)
{
	m_brush.reset(brush.Clone());
}


//////////////////////////////////////////////////////////////////////////
/// @brief Get the brush.
/**
@return The brush.
*/
const Gdiplus::Brush* HistGraph::getBrush() const
{
	return m_brush.get();
}


//////////////////////////////////////////////////////////////////////////
/// @brief Get the brush.
/**
@return The brush.
*/
Gdiplus::Brush* HistGraph::getBrush()
{
	return m_brush.get();
}


//////////////////////////////////////////////////////////////////////////
/// @brief Set the zero level.
/**
@param[in] zeroLevel The new zero level.
*/
void HistGraph::setZeroLevel(Real zeroLevel)
{
	m_zeroLevel = zeroLevel;
	update();
}


//////////////////////////////////////////////////////////////////////////
/// @brief Get the zero level.
/**
@return The zero level.
*/
Real HistGraph::getZeroLevel() const
{
	return m_zeroLevel;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Set the bar width.
/**
@param[in] barWidth The new bar width.
*/
void HistGraph::setBarWidth(Real barWidth)
{
	m_barWidth = barWidth;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Get the bar width.
Real HistGraph::getBarWidth() const
{
	return m_barWidth;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Assign new points.
/**
@param[in] Npoints The number of points.
@param[in] XYs The points.
*/
void HistGraph::assign(size_t Npoints, const Point *XYs)
{
	m_wpoints.assign(XYs,
		XYs + Npoints);

	update();
}


//////////////////////////////////////////////////////////////////////////
/// @brief Assign new points.
/**
@param[in] Npoints The number of points.
@param[in] Ys The Y values.
@param[in] Xs The X values.
*/
void HistGraph::assign(size_t Npoints, const Real *Ys, const Real *Xs)
{
	m_wpoints.resize(Npoints);
	for (size_t i = 0; i < Npoints; ++i)
		m_wpoints[i] = Point(Xs[i], Ys[i]);

	update();
}


//////////////////////////////////////////////////////////////////////////
/// @brief Assign new points.
/**
@param[in] Npoints The number of points.
@param[in] Ys The Y values.
@param[in] Xstart The first X value.
@param[in] Xstep The X step.
*/
void HistGraph::assign(size_t Npoints, const Real *Ys, Real Xstart, Real Xstep)
{
	m_wpoints.resize(Npoints);
	for (size_t i = 0; i < Npoints; ++i)
		m_wpoints[i] = Point(Xstart + i*Xstep, Ys[i]);

	update();
}


//////////////////////////////////////////////////////////////////////////
/// @brief Add one point.
/**
@param[in] pt The point.
*/
void HistGraph::push_back(const Point &pt)
{
	// update "full" area
	if (m_wpoints.empty())
	{
		m_full = Rect(pt, Size());
		m_full |= Point(pt.X(), m_zeroLevel);
	}
	else
		m_full |= pt;

	m_wpoints.push_back(pt);
}


//////////////////////////////////////////////////////////////////////////
/// @brief Add one point.
/**
@param[in] x The X value.
@param[in] y The Y value.
*/
void HistGraph::push_back(Real x, Real y)
{
	push_back(Point(x, y));
}


//////////////////////////////////////////////////////////////////////////
/// @brief Clear all points.
void HistGraph::clear()
{
	m_wpoints.clear();
	m_full = Rect();
}


//////////////////////////////////////////////////////////////////////////
/// @brief Draw the histogram graph.
/**
@param[in] plotter The plotter.
@param[in] canvas The canvas.
*/
void HistGraph::draw(const Plotter &plotter, Canvas &canvas) const
{
	const size_t Npoints = m_wpoints.size();

	{ // draw line
		const Gdiplus::Brush *pbrush = getBrush();
		const Gdiplus::Pen *ppen = getPen();

		const Real L = m_zeroLevel;
		const Real W = m_barWidth;
		for (size_t i = 0; i < Npoints; ++i)
		{
			const Point &pt = m_wpoints[i];
			Rect rc(Point(pt.X() - W/2, L),
				Point(pt.X() + W/2, pt.Y()));
			rc.normalize();

			const Rect vrc = plotter.w2v(rc);

			canvas.FillRectangle(pbrush, vrc);
			canvas.DrawRectangle(ppen, vrc);
		}
	}
}


//////////////////////////////////////////////////////////////////////////
/// @brief Hit testing.
/**
@param[in] pt The point of interest.
@param[in] eps The epsilon.
@return @b true if point is inside the histogram graph.
*/
bool HistGraph::hit_test(const Point &pt, Real eps) const
{
	// TODO: hit testing for HistGraph
	return false;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Get the "full" rectangle.
/**
@return The "full" rectangle.
*/
const Rect HistGraph::full() const
{
	return m_full;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Update the "full" rectangle.
void HistGraph::update()
{
	const size_t N = m_wpoints.size();

	if (0 < N)
	{
		const Point &pt = m_wpoints.front();
		m_full = Rect(pt, Size());

		for (size_t i = 1; i < N; ++i)
			m_full |= m_wpoints[i];

		m_full |= Point(m_full.Xmin(), m_zeroLevel);
	}
	else
		m_full = Rect();
}

	} // HistGraph

} // omni namespace
