#ifndef __MYMATRIX_H__
#define __MYMATRIX_H__

#include <Eigen/Core>
#include <Eigen/Cholesky>

/**
 * Represents a polynomial as an internal set of coeffients and the order.
 * Can be used to evaluate the polynomial or convert it to a string.
 */
class Polynomial
{
public:
	/// Constructor
	Polynomial(const unsigned int & = 3) noexcept;

	/// Destructor
	virtual ~Polynomial() noexcept;

	/// Fit data from CSV filename
	const double &fitData(const std::vector<double> &_x,
						  const std::vector<double> &_y) noexcept;

	/// Define an operator to evaluate the polynomial given a value for x
	double operator()(const double &) noexcept;

	/// Duplicate of the operator above to account to allow functor to be bound
	double eval(const double &_x) noexcept
	{
		// Use existing function
		return (*this)(_x);
	}

	const double &getLastFittingError() const noexcept { return m_lastFittingError; }

	/// Set the order of this polynomial
	void setOrder(const unsigned int &_order) noexcept;

private:
	/// Coefficient vectors
	Eigen::VectorXd m_coeffs;

	/// Store the last error from performing the fitting
	double m_lastFittingError = 0.0;
};

#endif //__MYMATRIX_H__
