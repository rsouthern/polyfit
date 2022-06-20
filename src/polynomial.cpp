#include <polynomial.h>

Polynomial::Polynomial(const unsigned int &_order) {
	setOrder(_order);
}

void Polynomial::setOrder(const unsigned int &_order) noexcept {
      // Don't waste time setting order to the same value
      if (m_order != _order) {

                // Set the order
                m_order = _order;
                // The number of coefficients is the poly order + 1 (for the constant term)
                m_coeffs.resize(m_order+1);

                // Indicate that the polynomial has to be recalculated
                m_dirty = true;
      }
}

