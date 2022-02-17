#pragma once
#include "stdafx.h"

namespace weights {
	/*
		The precision for comparing two float numbers.
		It also decides the precision of weights stored in unique_table.
	*/
	extern double EPS;

	/// <summary>
	/// Check whether two weights a and b are equal with respect to eps.
	/// </summary>
	/// <param name="a"></param>
	/// <param name="b"></param>
	/// <param name="eps"></param>
	/// <returns>Return true if they are equal.</returns>
	inline bool is_equal(wcomplex a, wcomplex b, double eps) {
		return abs(a.real() - b.real()) < eps && abs(a.imag() - b.imag()) < eps;
	}

	// This function takes in the weight (tensor) and generates the integer key for unique_table.
	inline int get_int_key(double weight) {
		return (int)round(weight / EPS);
	}


}