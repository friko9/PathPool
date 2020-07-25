#ifndef PATH_H
#define PATH_H

#include <algorithm>
#include <vector>
#include <functional>
#include <type_traits>

//////////////////////
// Global functions //
//////////////////////
template<typename PoolT,int poolno>
class Path;

template<typename PoolT,int poolno>
inline
Path<PoolT,poolno> common_path(Path<PoolT,poolno> const& l, Path<PoolT,poolno> const& r);

template<typename PoolT,int poolno>
inline
bool operator ==(Path<PoolT,poolno> const& l, Path<PoolT,poolno> const& r) noexcept;

template<typename PoolT,int poolno>
bool operator <(Path<PoolT,poolno> const& l, Path<PoolT,poolno> const& r);

////////////////
// Path class //
////////////////
template<typename PoolT,int poolno = 0>
class Path
{
public:
  class horizontal_iterator;
  class vertical_iterator;
  using tag_t = typename PoolT::tag_t;
public:
  Path();
  Path(Path const& parent,tag_t const& name);
  auto operator /(tag_t const& t) const -> Path;
  auto get_subpaths() const -> std::pair<horizontal_iterator,horizontal_iterator>;
  auto get_parent() const -> Path;
  auto get_tag() const -> tag_t;
  auto begin() -> vertical_iterator;
  auto end() -> vertical_iterator;
private:
  using pathid_t = typename PoolT::pathid_t;
private:
  explicit
  Path(pathid_t p);
private:
  static PoolT s_pool;
  pathid_t m_path;
private:
  friend
  Path<PoolT,poolno>
  common_path<>(Path<PoolT,poolno> const& l, Path<PoolT,poolno> const& r);
  friend
  bool operator == <>(Path<PoolT,poolno> const& l, Path<PoolT,poolno> const& r);
  friend
  bool operator < <>(Path<PoolT,poolno> const& l, Path<PoolT,poolno> const& r);
};

////////////////////
// Implementation //
////////////////////
#include "impl/impl_path_utils.h"
#include "impl/impl_path.h"

#endif /* PATH_H */
