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
	Polynomial(const unsigned int& =3);

	/// Destructor
	virtual ~Polynomial() {};

private:
	/// Set the order of this polynomial
	void setOrder(const unsigned int &_order) noexcept;

	/// Order of this polynomial
	unsigned int m_order = 0;

	/// Coefficient vectors
	Eigen::VectorXd m_coeffs;

	/// Tracks if this needs recalculating
	bool m_dirty = true;
};

#endif //__MYMATRIX_H__

