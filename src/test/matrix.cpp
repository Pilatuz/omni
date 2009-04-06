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
/** @file
	@brief The unit-test of "matrix.hpp".

@author Sergey Polichnoy
*/
#include <omni/matrix.hpp>
#include <omni/matmath.hpp>
#include <test/test.hpp>

#include <iterator>
#include <iomanip>
#include <sstream>
#include <complex>

#include <tchar.h>

template class omni::mx::Scalar<int>;
template class omni::mx::Scalar<float>;
template class omni::mx::Scalar<double>;
template class omni::mx::Scalar< std::complex<float> >;
template class omni::mx::Scalar< std::complex<double> >;

template class omni::mx::Vector<int>;
template class omni::mx::Vector<float>;
template class omni::mx::Vector<double>;
template class omni::mx::Vector< std::complex<float> >;
template class omni::mx::Vector< std::complex<double> >;

template class omni::mx::Matrix<int>;
template class omni::mx::Matrix<float>;
template class omni::mx::Matrix<double>;
template class omni::mx::Matrix< std::complex<float> >;
template class omni::mx::Matrix< std::complex<double> >;

template<typename T, typename A>
void print(std::ostream &os, const omni::mx::Matrix<T,A> &x)
{
	os << x.Nrows() << "x" << x.Ncols() << ":\n";

	for (size_t i = 0; i < x.Nrows(); ++i)
	{
		for (size_t j = 0; j < x.Ncols(); ++j)
			os << x[i][j] << "\t";
		os << "\n";
	}
}


// test function
bool test_matrix(std::ostream &os)
{
	using namespace omni::mx;

	if (0)
	{ // QRD testing
		typedef std::complex<double> T;
		Matrix<T> A1(5,3);
		for (size_t i = 0; i < A1.Nrows(); ++i)
			for (size_t j = 0; j < A1.Ncols(); ++j)
				A1[i][j] = T(::rand() / double(RAND_MAX) - 0.5, ::rand() / double(RAND_MAX) - 0.5);

		Matrix<T> Q;
		Matrix<T> R;
		Q = qrd(A1, R);

		os << std::setprecision(4);
		os << std::fixed;

		os << "\nA ";
		print(os, A1);

		os << "\nQ ";
		print(os, Q);

		os << "\nR ";
		print(os, R);

		os << "\nQ*R-A ";
		print(os, Matrix<double>(norm(Matrix<T>(Q*R - A1))));

		os << "\nherm(Q)*Q ";
		print(os, Matrix<double>(norm(Matrix<T>(herm(Q)*Q))));
	}

	if (0)
	{ // Cholesky testing
		typedef std::complex<double> T;
		Matrix<T> A(3,3);
		for (size_t i = 0; i < A.Nrows(); ++i)
			for (size_t j = 0; j < A.Ncols(); ++j)
				A[i][j] = T(::rand() / double(RAND_MAX) - 0.5, ::rand() / double(RAND_MAX) - 0.5);

		A = A*herm(A);

		Matrix<T> L, U;
		L = choll(L = A);
		U = cholu(U = A);

		os << std::setprecision(4);
		os << std::fixed;

		os << "\nA ";
		print(os, A);

		os << "\nL ";
		print(os, L);

		os << "\nU ";
		print(os, U);

		os << "\nL*herm(L)-A ";
		print(os, Matrix<double>(norm(Matrix<T>(L*herm(L) - A))));

		os << "\nherm(U)*U-A ";
		print(os, Matrix<double>(norm(Matrix<T>(herm(U)*U - A))));
	}

	if (0) // check just compilation errors
	{

	{ // Scalar
		make_scalar(1);
		make_scalar(1.0f);
		make_scalar(1.0);
	}

	{ // Vector
		Vector<int> v0;
		Vector<int> v1(10);
		Vector<double> v2(10, 1.0);
		Vector<double> v3(v2);
		Vector<double> v4(v1); v4 = v1;

		Matrix<int> m1(1, 100, 555);
		Vector<double> v5(m1); v5 = m1;
		Vector<double> v6(v1.begin(), v1.end());
	}

	{ // Matrix
		Matrix<int> m0;
		Matrix<int> m1(10, 10);
		Matrix<double> m2(5, 5, 1.0);
		Matrix<double> m3(m2); m3 = m2;
		Matrix<double> m4(m1); m4 = m1;

		Vector<int> v1(10, 555);
		Matrix<double> m5(v1); m5 = v1;

		m5.assign(10, 10, m1.begin(), m1.end());
		m5.assign(m1.begin(), m1.end());

		swap(m3,m4);
		m5 == m1;
		m5 != m1;

		is_empty(m0);
		is_square(m1);

		Matrix<double>::row_const_iterator cr = m5.row_begin(0);
		Matrix<double>::row_iterator r = m5.row_begin(1);
		cr = r; cr == r; cr != r; cr < r; cr <= r; cr > r; cr >= r;
		r + 5; (5 + r) - 5; (cr+5) - cr;

		Matrix<double>::col_const_iterator cc = m5.col_begin(0);
		Matrix<double>::col_iterator c = m5.col_begin(1);
		cc = c; cc == c; cc != c; cc < c; cc <= c; cc > c; cc >= c;
		c + 5; (5 + c) - 5; (cc+5) - cc;
	}

	{ // Scalar operators
		Scalar<int> s0(1);
		Scalar<double> s1(1.0);

		++s0; s0++; --s0; s0--; ~s0; !s0; -s0; +s0; s0*s0; s0/s0; s0%s0; s0+s0; s0-s0;
		s0==s0; s0!=s0; s0<s0; s0<=s0; s0>s0; s0>=s0;
		s0*=s0; s0/=s0; s0%=s0; s0+=s0; s0-=s0;

		++s1; s1++; --s1; s1--; !s1; -s1; +s1; s1*s1; s1/s1; s1+s1; s1-s1;
		s1==s1; s1!=s1; s1<s1; s1<=s1; s1>s1; s1>=s1;
		s1*=s1; s1/=s1; s1+=s1; s1-=s1;
	}

	{ // matrix math
		Matrix<int> m1(10, 10, 555);
		Matrix<double> m2(10, 10, 666.666);
		Vector<double> v1(10, 11.22);

		is_symmetric(m1); is_symmetric(m2);
		is_hermitian(m1); is_hermitian(m2);

		row_swap(m1,0,1);   col_swap(m2,0,1);
		row_mul(m1,0,1);    col_mul(m2,0,1);
		row_fadd(m1,0,1,0); col_fadd(m2,0,1,0);

		det(m2);
		norme(v1);
		norme(m2);

		// eye(5); eye(10);
		m1 = eye(5);
		m2 = eye(10);

		Vector<double> v2(-v1);
		v2.clear();
		v2 = -(-v1);
		v2 = -(-(-v1));
		m2 = -m1;
		m2 = -(-m1);

		m2 = trans(m1);
		m2 = trans(trans(m1));
		m2 = trans(trans(trans(m1)));

		v2 = conj(v1);
		v2 = conj(conj(v1));
		v2 = conj(conj(conj(v1)));

		m2 = conj(m1);
		m2 = conj(conj(m1));
		m2 = conj(conj(conj(m1)));

		m2 = herm(m1);
		m2 = herm(herm(m1));
		m2 = herm(herm(herm(m1)));

		{
			Matrix<double> A(10,10);
			for (size_t i = 0; i < A.Nrows(); ++i)
				for (size_t j = 0; j < A.Ncols(); ++j)
					A[i][j] = rand() / double(RAND_MAX);

			v2 = solve(A, v1);
			v2 = solve(A, -v1);
			v2 = solve(A, v1+v1);
			v2 = solve(-A, v1);
			v2 = solve(-A, -v1);
			v2 = solve(-A, v1+v1);
			v2 = solve(A+A, v1);
			v2 = solve(A+A, -v1);
			v2 = solve(A+A, v1+v1);
			m2 = solve(A, m1);
			m2 = solve(A, -m1);
			m2 = solve(A, m1+m1);
			m2 = solve(-A, m1);
			m2 = solve(-A, -m1);
			m2 = solve(-A, m1+m1);
			m2 = solve(A+A, m1);
			m2 = solve(A+A, -m1);
			m2 = solve(A+A, m1+m1);

			m2 = inv(A);
			m2 = inv(-A);
			m2 = inv(A+A);

			m2 = choll(A);
			m2 = choll(-A);
			m2 = choll(A+A);
			m2 = cholu(A);
			m2 = cholu(-A);
			m2 = cholu(A+A);

			m2 = qrd(A,m2);
			m2 = qrd(-A,m2);
			m2 = qrd(A+A,m2);
		}

		// +
		Scalar<int> s(0);
		Vector<double> v3;
		v3 = s + v1;
		v3 = s + (-v1);
		v3 = s + (v1+v2);
		v3 = v1 + s;
		v3 = v1 + v2;
		v3 = v1 + (-v2);
		v3 = v1 + (v2+v2);
		v3 = (-v1) + s;
		v3 = (-v1) + v2;
		v3 = (-v1) + (-v2);
		v3 = (-v1) + (v2+v2);
		v3 = (v1+v1) + s;
		v3 = (v1+v1) + v2;
		v3 = (v1+v1) + (-v2);
		v3 = (v1+v1) + (v2+v2);
		v3 += s;
		v3 += v1;
		v3 += (-v1);
		v3 += v1+v2;

		// +
		Matrix<double> m3;
		m3 = s + m1;
		m3 = s + (-m1);
		m3 = s + (m1+m2);
		m3 = m1 + s;
		m3 = m1 + m2;
		m3 = m1 + (-m2);
		m3 = m1 + (m2+m2);
		m3 = (-m1) + s;
		m3 = (-m1) + m2;
		m3 = (-m1) + (-m2);
		m3 = (-m1) + (m2+m2);
		m3 = (m1+m1) + s;
		m3 = (m1+m1) + m2;
		m3 = (m1+m1) + (-m2);
		m3 = (m1+m1) + (m2+m2);
		m3 += s;
		m3 += m1;
		m3 += (-m1);
		m3 += m1+m1;

		// -
		v3 = s - v1;
		v3 = s - (-v1);
		v3 = s - (v1+v2);
		v3 = v1 - s;
		v3 = v1 - v2;
		v3 = v1 - (-v2);
		v3 = v1 - (v2+v2);
		v3 = (-v1) - s;
		v3 = (-v1) - v2;
		v3 = (-v1) - (-v2);
		v3 = (-v1) - (v2+v2);
		v3 = (v1+v1) - s;
		v3 = (v1+v1) - v2;
		v3 = (v1+v1) - (-v2);
		v3 = (v1+v1) - (v2+v2);
		v3 -= s;
		v3 -= v1;
		v3 -= (-v1);
		v3 -= v1+v2;

		// -
		m3 = s - m1;
		m3 = s - (-m1);
		m3 = s - (m1+m2);
		m3 = m1 - s;
		m3 = m1 - m2;
		m3 = m1 - (-m2);
		m3 = m1 - (m2+m2);
		m3 = (-m1) - s;
		m3 = (-m1) - m2;
		m3 = (-m1) - (-m2);
		m3 = (-m1) - (m2+m2);
		m3 = (m1+m1) - s;
		m3 = (m1+m1) - m2;
		m3 = (m1+m1) - (-m2);
		m3 = (m1+m1) - (m2+m2);
		m3 -= s;
		m3 -= m1;
		m3 -= (-m1);
		m3 -= m1+m1;

		// .*
		s = make_scalar(1);
		v3 = s * v1;
		v3 = s * (-v1);
		v3 = s * (v1+v1);
		v3 = v1 * s;
		v3 = -v1 * s;
		v3 = (v1+v1) * s;
		v3 *= s;
		m3 = s * m1;
		m3 = s * (-m1);
		m3 = s * (m1+m1);
		m3 = m1 * s;
		m3 = -m1 * s;
		m3 = (m1+m1) * s;
		m3 *= s;

		// ./
		s = make_scalar(1);
		v3 = v1 / s;
		v3 = -v1 / s;
		v3 = (v1+v1) / s;
		v3 /= s;
		m3 = m1 / s;
		m3 = -m1 / s;
		m3 = (m1+m1) / s;
		m3 /= s;

		// *
		m3 = m1 * m2;
		m3 = m1 * (-m2);
		m3 = m1 * (m2+m2);
		m3 = (-m1) * m2;
		m3 = (-m1) * (-m2);
		m3 = (-m1) * (m2+m2);
		m3 = (m1-m1) * m2;
		m3 = (m1-m1) * (-m2);
		m3 = (m1-m1) * (m2+m2);
		m3 *= m1;
		m3 *= -m1;
		m3 *= m1+m1;

		v3 = m1 * v2;
		v3 = m1 * (-v2);
		v3 = m1 * (v2+v2);
		v3 = (-m1) * v2;
		v3 = (-m1) * (-v2);
		v3 = (-m1) * (v2+v2);
		v3 = (m1-m1) * v2;
		v3 = (m1-m1) * (-v2);
		v3 = (m1-m1) * (v2+v2);

		v3 = v2 * m1;
		v3 = v2 * (-m1);
		v3 = v2 * (m1-m1);
		v3 = (-v2) * m1;
		v3 = (-v2) * (-m1);
		v3 = (-v2) * (m1-m1);
		v3 = (v2+v2) * m1;
		v3 = (v2+v2) * (-m1);
		v3 = (v2+v2) * (m1-m1);
		v3 *= m1;
		v3 *= -m1;
		v3 *= m1+m1;
	}

	} // if (0)

	if (1) // iterators check
	{
		Matrix<int> m(3,4);

		std::reverse(m.begin(), m.end());
		for (size_t i = 0; i < m.Nrows(); ++i)
			std::reverse(m.row_begin(i), m.row_end(i));
		for (size_t j = 0; j < m.Ncols(); ++j)
			std::reverse(m.col_begin(j), m.col_end(j));
	}

	return true;
}

namespace {

	// Test1 class
	class Test1: public omni::test::UnitTest {
		// test title
		virtual const char* title() const
		{
			return "omni::matrix";
		}

		// test function
		virtual bool do_test(std::ostream &os) const
		{
			return test_matrix(os);
		}
	} test1;

} // namespace
