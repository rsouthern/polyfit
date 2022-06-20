#ifndef __MYMATRIX_H__
#define __MYMATRIX_H__

#include <Eigen/Core>
#include <Eigen/Cholesky>

/**
 * Represents a polynomial as an internal set of coeffients and the order.
 * Can be used to evaluate the polynomial or convert it to a string.
 */
class Polynomial {
public:
	/// Constructor
	Polynomial(const unsigned int& =3) noexcept;

	/// Destructor
	virtual ~Polynomial() noexcept;

	/// Fit data from CSV filename
	void fitData(const std::string &csv) noexcept;

private:
	/// Set the order of this polynomial
	void setOrder(const unsigned int &_order) noexcept;

	/// Coefficient vectors
	Eigen::VectorXd m_coeffs;

	/// Tracks if this needs recalculating
	bool m_dirty = true;
};

#endif //__MYMATRIX_H__

