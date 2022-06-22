#include <polynomial.h>
#include <gtest/gtest.h>
#include <Eigen/Dense>

/**
 * @brief Construct a new Polynomial:: Polynomial object
 *
 * @param _order The order of the newly constructed polynomial (default 3)
 */
Polynomial::Polynomial(const unsigned int &_order) noexcept
{
   setOrder(_order);
}

/**
 * @brief Destroy the Polynomial:: Polynomial object
 *
 */
Polynomial::~Polynomial() noexcept
{
}

/**
 * @brief Sets the order of the polynomial - can be done after initialisation. Note this class could be templated for efficiency if dynamic resizing is not required.
 *
 * @param _order The new order of ths polynomial.
 */
void Polynomial::setOrder(const unsigned int &_order) noexcept
{
   // Don't waste time setting order to the same value
   if (m_coeffs.size() != _order + 1)
   {
      // The number of coefficients is the poly order + 1 (for the constant term)
      m_coeffs.resize(_order + 1);

      // Zero them out so it makes sense
      m_coeffs.setZero();
   }
}

/**
 * @brief Fit a polynomial (as represented by coeffients) to input x and y data pairs.
 *
 * @param _x x data
 * @param _y y data
 * @return const double& The RMS error of the fitting (also retrievable using getLastFittingError())
 */
const double &Polynomial::fitData(const std::vector<double> &_x,
                                  const std::vector<double> &_y) noexcept
{
   // Initialise the matrix from the _x vector. Order determines value in matrix, so values are 1, x, x^2, x^3, . . .
   Eigen::MatrixXd A;
   A.resize(_x.size(), m_coeffs.size());

   // Keep track on the row number
   unsigned int row = 0;
   for (auto xit = _x.begin(); xit != _x.end(); ++xit, ++row)
   {
      double val = 1.0;
      for (int col = 0; col < m_coeffs.size(); ++col)
      {
         // I'm assured direct access on Eigen matrices is fast
         A(row, col) = val;

         // Increase the dosage!
         val *= *xit;
      }
   }
   // Debug
   // Eigen::IOFormat CommaInitFmt(Eigen::StreamPrecision, Eigen::DontAlignCols, ", ", ", ", "", "", " << ", ";");
   // std::cout << "A" << A.format(CommaInitFmt) << "\n";

   // Map the answer vector into Y
   const Eigen::VectorXd b = Eigen::Map<const Eigen::VectorXd, Eigen::Unaligned>(_y.data(), _y.size());
   // std::cout << "b" << b.format(CommaInitFmt) << "\n";

   // This QR solver is from https://eigen.tuxfamily.org/dox/group__LeastSquares.html
   m_coeffs = A.colPivHouseholderQr().solve(b);

   // Debug
   // std::cout << "x" << m_coeffs.format(CommaInitFmt) << "\n";

   // Now evaluate the values with our polynomial to assess the RMS error
   double RMS_tot = 0.0;
   unsigned int i = 0;
   double tmp = 0.0;
   for (auto xit = _x.begin(); xit != _x.end(); ++xit, ++i)
   {
      // Evaluate the difference between the evaluated function and the sample point
      tmp = (*this)(*xit) - _y[i];

      // Sum this into the RMS
      RMS_tot += tmp * tmp;
   }
   // Note often RMS isn't sqrt'ed for performance reasons
   m_lastFittingError = sqrt(RMS_tot);
   return m_lastFittingError;
}

/**
 * @brief Basic functor operator to evaluate this polynomial function given an input x.
 *
 * @param _x Value to evaluate the function
 * @return double Evaluation of the function - e.g. f(x)
 */
double Polynomial::operator()(const double &_x) noexcept
{
   double ret_val = 0.0;
   double val = 1.0;
   for (int col = 0; col < m_coeffs.size(); ++col)
   {
      // Evaluate the term
      ret_val += m_coeffs(col) * val;

      // Increase the dosage!
      val *= _x;
   }

   return ret_val;
}