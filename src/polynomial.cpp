#include <polynomial.h>
#include <csvparse.h>
#include <gtest/gtest.h>

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

      // Indicate that the polynomial has to be recalculated
      m_dirty = true;
   }
}

/**
 * @brief Given a CSV file load up
 *
 * @param csv
 */
void Polynomial::fitData(const std::string &csv) noexcept
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
   // Debugging
   std::copy(_x.begin(), _x.end(), std::ostream_iterator<double>(std::cout, " "));
   std::copy(_y.begin(), _y.end(), std::ostream_iterator<double>(std::cout, " "));

   // Initialise the matrix from the _x vector

   // Copy the answer vector into _y
}
