/**********************************************************************************
**
** Copyright (C) 2013 Narvik University College
** Contact: GMlib Online Portal at http://episteme.hin.no
**
** This file is part of the Geometric Modeling Library, GMlib.
**
** GMlib is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** GMlib is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with GMlib.  If not, see <http://www.gnu.org/licenses/>.
**
**********************************************************************************/



/*! \file gmviennaclmatmult.c
 *
 *  Implementation of ViennaCL-specific matrix multiplication operators.
 */

#include <viennacl/ocl/backend.hpp>
#include <viennacl/linalg/prod.hpp>
#include <viennacl/matrix_proxy.hpp>

namespace GMlib
{

/*!	const DMatrix<float>&	operator*(const DMatrix<float>& m, const DMatrix<float>& b)
 *	\brief Multiply two matrices
 *
 *	ViennaCL-specific implementation of the multiplication of two matrices
 *	of data type float.
 */
inline
const DMatrix<float>&  operator*(const DMatrix<float>& m, const DMatrix<float>& b)
{
	static DMatrix<float> r;
	r.setDim(m.getDim1(), b.getDim2());

	float* work_A = new float[m.getDim1() * m.getDim2()];
	float* work_B = new float[b.getDim1() * b.getDim2()];
	float* work_C = new float[r.getDim1() * r.getDim2()];

	for(int i=0; i<m.getDim1(); i++)
		memcpy(&work_A[i*m.getDim2()], &m(i)(0), m.getDim2()*sizeof(float));
	for(int i=0; i<b.getDim1(); i++)
		memcpy(&work_B[i*b.getDim2()], &b(i)(0), b.getDim2()*sizeof(float));

#ifdef VIENNACL_WITH_OPENCL
	viennacl::ocl::set_context_device_type(0, viennacl::ocl::gpu_tag());
#endif

	viennacl::matrix<float> vcl_A(m.getDim1(), m.getDim2());
	viennacl::matrix<float> vcl_B(b.getDim1(), b.getDim2());
	viennacl::matrix<float> vcl_C(m.getDim1(), b.getDim2());

#ifdef VIENNACL_WITH_OPENCL
	std::vector<viennacl::ocl::device> devices = viennacl::ocl::current_context().devices();
#else
	std::vector<long> devices(1);
#endif

#ifdef VIENNACL_WITH_OPENCL
	viennacl::ocl::current_context().switch_device(devices[0]);
#endif

	viennacl::fast_copy(&work_A[0], &work_A[0] + m.getDim1() * m.getDim2(), vcl_A);
	viennacl::fast_copy(&work_B[0], &work_B[0] + b.getDim1() * b.getDim2(), vcl_B);

	vcl_C = viennacl::linalg::prod(vcl_A, vcl_B);
	viennacl::backend::finish();

	viennacl::fast_copy(vcl_C, work_C);

	for(int i=0; i<r.getDim1(); i++)
		memcpy(&r[i][0], &work_C[i*r.getDim2()], r.getDim2()*sizeof(float));

	delete[] work_A;
	delete[] work_B;
	delete[] work_C;

	return r;
}

/*!	const DMatrix<double>&	operator*(const DMatrix<double>& m, const DMatrix<double>& b)
 *	\brief Multiply two matrices
 *
 *	ViennaCL-specific implementation of the multiplication of two matrices
 *	of data type double.
 */
inline
const DMatrix<double>&  operator*(const DMatrix<double>& m, const DMatrix<double>& b)
{
	static DMatrix<double> r;
	r.setDim(m.getDim1(), b.getDim2());

	double* work_A = new double[m.getDim1() * m.getDim2()];
	double* work_B = new double[b.getDim1() * b.getDim2()];
	double* work_C = new double[r.getDim1() * r.getDim2()];

	for(int i=0; i<m.getDim1(); i++)
		memcpy(&work_A[i*m.getDim2()], &m(i)(0), m.getDim2()*sizeof(double));
	for(int i=0; i<b.getDim1(); i++)
		memcpy(&work_B[i*b.getDim2()], &b(i)(0), b.getDim2()*sizeof(double));

#ifdef VIENNACL_WITH_OPENCL
	viennacl::ocl::set_context_device_type(0, viennacl::ocl::gpu_tag());
#endif

	viennacl::matrix<double> vcl_A(m.getDim1(), m.getDim2());
	viennacl::matrix<double> vcl_B(b.getDim1(), b.getDim2());
	viennacl::matrix<double> vcl_C(m.getDim1(), b.getDim2());

#ifdef VIENNACL_WITH_OPENCL
	std::vector<viennacl::ocl::device> devices = viennacl::ocl::current_context().devices();
#else
	std::vector<long> devices(1);
#endif

#ifdef VIENNACL_WITH_OPENCL
	viennacl::ocl::current_context().switch_device(devices[0]);
#endif

	viennacl::fast_copy(&work_A[0], &work_A[0] + m.getDim1() * m.getDim2(), vcl_A);
	viennacl::fast_copy(&work_B[0], &work_B[0] + b.getDim1() * b.getDim2(), vcl_B);

	vcl_C = viennacl::linalg::prod(vcl_A, vcl_B);
	viennacl::backend::finish();

	viennacl::fast_copy(vcl_C, work_C);

	for(int i=0; i<r.getDim1(); i++)
		memcpy(&r[i][0], &work_C[i*r.getDim2()], r.getDim2()*sizeof(double));
	
	delete[] work_A;
	delete[] work_B;
	delete[] work_C;

	return r;
}

/*!	const DMatrix<std::complex<float> >&	operator*(const DMatrix<std::complex<float> >& m, const DMatrix<std::complex<float> >& b)
 *	\brief Multiply two matrices
 *
 *	ViennaCL-specific implementation of the multiplication of two matrices
 *	of data type std::complex<float>.
 */
inline
const DMatrix<std::complex<float> >&  operator*(const DMatrix<std::complex<float> >& m, const DMatrix<std::complex<float> >& b)
{
	static DMatrix<std::complex<float> > r;
	r.setDim(m.getDim1(), b.getDim2());

	float* work_A_r = new float[m.getDim1() * m.getDim2()];
	float* work_B_r = new float[b.getDim1() * b.getDim2()];
	float* work_C_r = new float[r.getDim1() * r.getDim2()];
	float* work_A_i = new float[m.getDim1() * m.getDim2()];
	float* work_B_i = new float[b.getDim1() * b.getDim2()];
	float* work_C_i = new float[r.getDim1() * r.getDim2()];

	const float * m_it;
	float * work_r_it = work_A_r;
	float * work_i_it = work_A_i;
	for(int i=0; i<m.getDim1(); i++)
	{
		m_it = reinterpret_cast<const float*>(&m(i)(0));
		for(int j=0; j<m.getDim2(); j++)
		{
			*work_r_it++ = *m_it++;
			*work_i_it++ = *m_it++;
		}
	}
	work_r_it = work_B_r;
	work_i_it = work_B_i;
	for(int i=0; i<b.getDim1(); i++)
	{
		m_it = reinterpret_cast<const float*>(&b(i)(0));
		for(int j=0; j<b.getDim2(); j++)
		{
			*work_r_it++ = *m_it++;
			*work_i_it++ = *m_it++;
		}
	}
	
#ifdef VIENNACL_WITH_OPENCL
	viennacl::ocl::set_context_device_type(0, viennacl::ocl::gpu_tag());
#endif
	
	viennacl::matrix<float> vcl_A_r(m.getDim1(), m.getDim2());
	viennacl::matrix<float> vcl_B_r(b.getDim1(), b.getDim2());
	viennacl::matrix<float> vcl_C_r(m.getDim1(), b.getDim2());
	viennacl::matrix<float> vcl_A_i(m.getDim1(), m.getDim2());
	viennacl::matrix<float> vcl_B_i(b.getDim1(), b.getDim2());
	viennacl::matrix<float> vcl_C_i(m.getDim1(), b.getDim2());
	
#ifdef VIENNACL_WITH_OPENCL
	std::vector<viennacl::ocl::device> devices = viennacl::ocl::current_context().devices();
#else
	std::vector<long> devices(1);
#endif

#ifdef VIENNACL_WITH_OPENCL
	viennacl::ocl::current_context().switch_device(devices[0]);
#endif
	
	viennacl::fast_copy(work_A_r, work_A_r + m.getDim1() * m.getDim2(), vcl_A_r);
	viennacl::fast_copy(work_B_r, work_B_r + b.getDim1() * b.getDim2(), vcl_B_r);
	viennacl::fast_copy(work_A_i, work_A_i + m.getDim1() * m.getDim2(), vcl_A_i);
	viennacl::fast_copy(work_B_i, work_B_i + b.getDim1() * b.getDim2(), vcl_B_i);

	vcl_C_r = viennacl::linalg::prod(vcl_A_r, vcl_B_r) - viennacl::linalg::prod(vcl_A_i, vcl_B_i);
	vcl_C_i = viennacl::linalg::prod(vcl_A_r, vcl_B_i) + viennacl::linalg::prod(vcl_A_i, vcl_B_r);
	viennacl::backend::finish();

	viennacl::fast_copy(vcl_C_r, work_C_r);
	viennacl::fast_copy(vcl_C_i, work_C_i);
	
	float * r_it;
	work_r_it = work_C_r;
	work_i_it = work_C_i;
	for(int i=0; i<r.getDim1(); i++)
	{
		r_it = reinterpret_cast<float*>(&r[i][0]);
		for(int j=0; j<r.getDim2(); j++)
		{
			*r_it++ = *work_r_it++;
			*r_it++ = *work_i_it++;
		}
	}

	delete[] work_A_r;
	delete[] work_B_r;
	delete[] work_C_r;
	delete[] work_A_i;
	delete[] work_B_i;
	delete[] work_C_i;

	return r;
}

/*!	const DMatrix<std::complex<double> >&	operator*(const DMatrix<std::complex<double> >& m, const DMatrix<std::complex<double> >& b)
 *	\brief Multiply two matrices
 *
 *	ViennaCL-specific implementation of the multiplication of two matrices
 *	of data type std::complex<double>.
 */
inline
const DMatrix<std::complex<double> >&  operator*(const DMatrix<std::complex<double> >& m, const DMatrix<std::complex<double> >& b)
{
	static DMatrix<std::complex<double> > r;
	r.setDim(m.getDim1(), b.getDim2());

	double* work_A_r = new double[m.getDim1() * m.getDim2()];
	double* work_B_r = new double[b.getDim1() * b.getDim2()];
	double* work_C_r = new double[r.getDim1() * r.getDim2()];
	double* work_A_i = new double[m.getDim1() * m.getDim2()];
	double* work_B_i = new double[b.getDim1() * b.getDim2()];
	double* work_C_i = new double[r.getDim1() * r.getDim2()];
	
	const double * m_it;
	double * work_r_it = &work_A_r[0];
	double * work_i_it = &work_A_i[0];
	for(int i=0; i<m.getDim1(); i++)
	{
		m_it = reinterpret_cast<const double*>(&m(i)(0));
		for(int j=0; j<m.getDim2(); j++)
		{
			*work_r_it++ = *m_it++;
			*work_i_it++ = *m_it++;
		}
	}
	work_r_it = &work_B_r[0];
	work_i_it = &work_B_i[0];
	for(int i=0; i<b.getDim1(); i++)
	{
		m_it = reinterpret_cast<const double*>(&b(i)(0));
		for(int j=0; j<b.getDim2(); j++)
		{
			*work_r_it++ = *m_it++;
			*work_i_it++ = *m_it++;
		}
	}
	
#ifdef VIENNACL_WITH_OPENCL
	viennacl::ocl::set_context_device_type(0, viennacl::ocl::gpu_tag());
#endif
	
	viennacl::matrix<double> vcl_A_r(m.getDim1(), m.getDim2());
	viennacl::matrix<double> vcl_B_r(b.getDim1(), b.getDim2());
	viennacl::matrix<double> vcl_C_r(m.getDim1(), b.getDim2());
	viennacl::matrix<double> vcl_A_i(m.getDim1(), m.getDim2());
	viennacl::matrix<double> vcl_B_i(b.getDim1(), b.getDim2());
	viennacl::matrix<double> vcl_C_i(m.getDim1(), b.getDim2());
	
#ifdef VIENNACL_WITH_OPENCL
	std::vector<viennacl::ocl::device> devices = viennacl::ocl::current_context().devices();
#else
	std::vector<long> devices(1);
#endif

#ifdef VIENNACL_WITH_OPENCL
	viennacl::ocl::current_context().switch_device(devices[0]);
#endif
	
	viennacl::fast_copy(&work_A_r[0], &work_A_r[0] + m.getDim1() * m.getDim2(), vcl_A_r);
	viennacl::fast_copy(&work_B_r[0], &work_B_r[0] + b.getDim1() * b.getDim2(), vcl_B_r);
	viennacl::fast_copy(&work_A_i[0], &work_A_i[0] + m.getDim1() * m.getDim2(), vcl_A_i);
	viennacl::fast_copy(&work_B_i[0], &work_B_i[0] + b.getDim1() * b.getDim2(), vcl_B_i);

	vcl_C_r = viennacl::linalg::prod(vcl_A_r, vcl_B_r) - viennacl::linalg::prod(vcl_A_i, vcl_B_i);
	vcl_C_i = viennacl::linalg::prod(vcl_A_r, vcl_B_i) + viennacl::linalg::prod(vcl_A_i, vcl_B_r);
	viennacl::backend::finish();

	viennacl::fast_copy(vcl_C_r, &work_C_r[0]);
	viennacl::fast_copy(vcl_C_i, &work_C_i[0]);
	
	double * r_it;
	work_r_it = &work_C_r[0];
	work_i_it = &work_C_i[0];
	for(int i=0; i<r.getDim1(); i++)
	{
		r_it = reinterpret_cast<double*>(&r[i][0]);
		for(int j=0; j<r.getDim2(); j++)
		{
			*r_it++ = *work_r_it++;
			*r_it++ = *work_i_it++;
		}
	}
	
	delete[] work_A_r;
	delete[] work_B_r;
	delete[] work_C_r;
	delete[] work_A_i;
	delete[] work_B_i;
	delete[] work_C_i;

	return r;
}

} // namespace GMlib
