/********************************************************************************
 * HOMMEXX 1.0: Copyright of Sandia Corporation
 * This software is released under the BSD license
 * See the file 'COPYRIGHT' in the HOMMEXX/src/share/cxx directory
 *******************************************************************************/

#ifndef HOMMEXX_ELEMENTS_STATE_HPP
#define HOMMEXX_ELEMENTS_STATE_HPP

#include "Types.hpp"

namespace Homme {

class HybridVCoord;

struct StateStorage {
  ExecViewManaged<Scalar * [NUM_TIME_LEVELS][2][NP][NP][NUM_LEV  ]> m_v;          // Horizontal velocity
  ExecViewManaged<Scalar * [NUM_TIME_LEVELS]   [NP][NP][NUM_LEV_P]> m_w_i;        // Vertical velocity at interfaces
  ExecViewManaged<Scalar * [NUM_TIME_LEVELS]   [NP][NP][NUM_LEV  ]> m_vtheta_dp;  // Virtual potential temperature (mass)
  ExecViewManaged<Scalar * [NUM_TIME_LEVELS]   [NP][NP][NUM_LEV_P]> m_phinh_i;    // Geopotential used by NH model at interfaces
  ExecViewManaged<Scalar * [NUM_TIME_LEVELS]   [NP][NP][NUM_LEV  ]> m_dp3d;       // Delta p on levels

  ExecViewManaged<Real   * [NUM_TIME_LEVELS]   [NP][NP]           > m_ps_v;       // Surface pressure

  void init_storage(const int num_elems);

  void copy_state (const StateStorage& src);
};

/* Per element data - specific velocity, temperature, pressure, etc. */
class ElementsState : public StateStorage {
public:

  // A copy of this state. We will use it to store the initial state
  StateStorage m_state0;

  ElementsState() : m_num_elems(0) {}

  void init(const int num_elems);

  void randomize(const int seed);
  void randomize(const int seed, const Real max_pressure);
  void randomize(const int seed, const Real max_pressure, const Real ps0);

  void randomize(const int seed, const Real max_pressure, const Real ps0,
                 const ExecViewUnmanaged<const Real*[NP][NP]>& phis);

  // Copies current state into m_state0
  void save_state();

  KOKKOS_INLINE_FUNCTION
  int num_elems() const { return m_num_elems; }

  // Fill the exec space views with data coming from F90 pointers
  void pull_from_f90_pointers(CF90Ptr& state_v,         CF90Ptr& state_w_i,
                              CF90Ptr& state_vtheta_dp, CF90Ptr& state_phinh_i,
                              CF90Ptr& state_dp3d,      CF90Ptr& state_ps_v);

  // Push the results from the exec space views to the F90 pointers
  void push_to_f90_pointers(F90Ptr& state_v, F90Ptr& state_w_i, F90Ptr& state_vtheta_dp,
                            F90Ptr& state_phinh_i, F90Ptr& state_dp) const;

private:
  int m_num_elems;

};

} // Homme

#endif // HOMMEXX_ELEMENTS_STATE_HPP
