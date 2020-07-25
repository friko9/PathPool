#ifndef IMPL_PATH_UTILS_H
#define IMPL_PATH_UTILS_H

// wardens prevent cyclic inclusion
#include "../path.h"

template<typename PoolT,int poolno>
inline
Path<PoolT,poolno> common_path(const Path<PoolT,poolno>& l, const Path<PoolT,poolno>& r)
{
  return Path<PoolT,poolno>{ get_common_path(l.m_path,r.m_path,l.s_pool)[0] };
}

template<typename PoolT,int poolno>
inline
bool operator ==(const Path<PoolT,poolno>& l, const Path<PoolT,poolno>& r) noexcept
{
  return l.m_path == r.m_path;
}

template<typename PoolT>
bool operator_less_impl(typename PoolT::pathid_t l, typename PoolT::pathid_t r, PoolT const& pool)
{
  auto split = get_common_path(l, r, pool);
  if(split[1] == split[2]) return false;
  if(split[0] == split[1]) return true;
  if(split[0] == split[2]) return false;
  return pool.get_tag(split[1]) < pool.get_tag(split[2]);
}

template<typename PoolT,int poolno>
inline
bool operator <(const Path<PoolT,poolno>& l, const Path<PoolT,poolno>& r)
{
  return operator_less_impl(l.m_path, r.m_path, Path<PoolT,poolno>::s_pool);
}

#endif /* IMPL_PATH_UTILS_H */
