/* 
 *      Copyright (c) 2020 Robert Shaw
 *		This file is a part of Libecpint.
 *
 *      Permission is hereby granted, free of charge, to any person obtaining
 *      a copy of this software and associated documentation files (the
 *      "Software"), to deal in the Software without restriction, including
 *      without limitation the rights to use, copy, modify, merge, publish,
 *      distribute, sublicense, and/or sell copies of the Software, and to
 *      permit persons to whom the Software is furnished to do so, subject to
 *      the following conditions:
 *
 *      The above copyright notice and this permission notice shall be
 *      included in all copies or substantial portions of the Software.
 *
 *      THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *      EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *      MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *      NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 *      LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 *      OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 *      WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef API_HEAD
#define API_HEAD

#include <vector>
#include <array>
#include <string>
#include <memory>
#include "ecpint.hpp"
#include "multiarr.hpp"

namespace libecpint {
	
#define H_START(i, j, N) (9*j + 3*(3*N-1)*i - (9*i*(i+1))/2 - 3)
	
	struct ECPIntegrator {
		std::vector<GaussianShell> shells;
		ECPBasis ecps; 
		std::shared_ptr<ECPIntegral> ecpint; 
		int maxLB, deriv, ncart, natoms;
		
		bool ecp_is_set, basis_is_set;
		
		TwoIndex<double> integrals; 
		std::vector<TwoIndex<double>> first_derivs, second_derivs;
		
		ECPIntegrator() { ecp_is_set = basis_is_set = false; maxLB = ncart = 0; }
		
		void set_gaussian_basis(int nshells, double* coords, double* exponents, double* coefs, int* ams, int* shell_lengths);
		void set_ecp_basis(int necps, double* coords, double* exponents, double* coefs, int* ams, int* ns, int* shell_lengths);
		void set_ecp_basis_from_library(int necps, double* coords, int* charges, std::vector<std::string> names, std::string share_dir);
		void update_gaussian_basis_coords(int nshells, double* coords);
		void update_ecp_basis_coords(int necps, double* coords);
		
		void init(int deriv_ = 0);
		void compute_integrals();
		void compute_first_derivs();
		void compute_second_derivs();
		
		std::shared_ptr<std::vector<double>> get_integrals() { return std::make_shared<std::vector<double>>(integrals.data); }
		
		std::vector<std::shared_ptr<std::vector<double>>> get_first_derivs() {
			std::vector<std::shared_ptr<std::vector<double>>> results;
			for (auto& v : first_derivs) results.push_back(std::make_shared<std::vector<double>>(v.data));
			return results;
		}
		
		std::vector<std::shared_ptr<std::vector<double>>> get_second_derivs() {
			std::vector<std::shared_ptr<std::vector<double>>> results;
			for (auto& v : second_derivs) results.push_back(std::make_shared<std::vector<double>>(v.data));
			return results;
		}
	};
	
}

#endif
