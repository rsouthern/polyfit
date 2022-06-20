#include <polynomial.h>
#include <csvparse.h>
#include <gtest/gtest.h>
#include <Eigen/Dense>

/**
 * @brief Construct a new Polynomial:: Polynomial object
 *
 * @param _order
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
 * @brief
 *
 * @param _order
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
 * @brief Given a CSV file load up
 *
 * @param csv
 */
const double &Polynomial::fitData(const std::string &csv) noexcept
{
   // Initialise two vectors to hold the (x,y) pairs
   std::vector<double> _x, _y;

   // test for doubles
   ifstream fin(csv);
   if (fin)
   {
      auto cit = csv_istream_iterator<double>(fin);
      for (bool isx = true; fin; ++cit, isx = !isx)
      {
         // Note we can't initialise Eigen from vectors of unknown length
         // (might be better to parse and create std::vectors externally?)
         (isx) ? _x.push_back(*cit) : _y.push_back(*cit);
      }
      fin.close();
   }

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
   Eigen::IOFormat CommaInitFmt(Eigen::StreamPrecision, Eigen::DontAlignCols, ", ", ", ", "", "", " << ", ";");
   std::cout << "A" << A.format(CommaInitFmt) << "\n";

   // Map the answer vector into Y
   Eigen::VectorXd b = Eigen::Map<Eigen::VectorXd, Eigen::Unaligned>(_y.data(), _y.size());
   std::cout << "b" << b.format(CommaInitFmt) << "\n";

   // This QR solver is from https://eigen.tuxfamily.org/dox/group__LeastSquares.html
   m_coeffs = A.colPivHouseholderQr().solve(b);

   // Debug
   std::cout << "x" << m_coeffs.format(CommaInitFmt) << "\n";

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
 * @brief
 *
 * @param _x
 * @return double
 */
double Polynomial::operator()(const double &_x) const noexcept
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