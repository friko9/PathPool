#ifndef PATH_H
#define PATH_H

#include <algorithm>
#include <vector>
#include <functional>
#include <type_traits>

template<typename PoolT,int poolno>
class Path;

template<typename PoolT,int poolno>
Path<PoolT,poolno> common_path(const Path<PoolT,poolno> l, const Path<PoolT,poolno> r)
{
  return l.s_pool.get_common(l.m_path,r.m_path)[0];
}

template<typename PoolT,int poolno = 0>
class Path
{
  friend
  Path<PoolT,poolno>
  common_path<>(const Path<PoolT,poolno> l, const Path<PoolT,poolno> r);
private:
  using pathid_t = typename PoolT::pathid_t;
public:
  class iterator_t;
  using tag_t = typename PoolT::tag_t;
public:
  Path():
    m_path{s_pool.get_root()}
  {}
  Path(const Path& parent, tag_t name):
    m_path{ s_pool.get_subnode(parent.m_path, name)}
  {}
  Path operator /(const tag_t& t)
  {
    return Path { *this, t };
  }
  bool operator ==(const Path& r) const noexcept
  {
    return m_path == r.m_path;
  }
  bool operator <(const Path& r) const
  {
    auto split = get_common_path(m_path,r.m_path,s_pool);
    if(split[1] == split[2]) return false;
    if(split[0] == split[1]) return true;
    if(split[0] == split[2]) return false;
    return s_pool.get_tag(split[1]) < s_pool.get_tag(split[2]);
  }
  // this requires iterator
  template<template<typename...> class ResultT = std::vector>
  ResultT<Path> get_subpaths() const 
  {
    ResultT<Path> result;
    auto subpaths = s_pool.get_subnodes(m_path);
    std::for_each(subpaths.first,subpaths.second,
		  [&result](pathid_t p){ result.push_back(Path{p});});
    return result;
  }
  Path get_parent() const
  {
    return Path{ s_pool.get_parent(m_path) };
  }
  tag_t get_tag() const
  {
    return s_pool.get_tag(m_path);
  }
  iterator_t begin()
  {
    return iterator_t {*this};
  }
  iterator_t end()
  {
    return {};
  }
public:
  class iterator_t
  {
    using it_t = typename PoolT::iterator_t;
  public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = Path;
    using difference_type = size_t;
    using pointer = value_type*;
    using reference = value_type&;
  public:
    explicit
    iterator_t(Path it):
      m_it{it}
    {}
    iterator_t()
    {}
    ~iterator_t()
    {}
    value_type operator *(){return m_it;}
    iterator_t& operator ++(){ m_it = m_it.get_parent(); return *this; };
    iterator_t operator ++(int){ iterator_t result = *this; ++*this; return result; }
    bool operator ==(iterator_t const& r) const
    {
      return m_it == r.m_it;
    }
    bool operator !=(iterator_t const& r) const
    {
      return !(*this == r);
    }
  private:
    Path m_it {};
  };
private:
  explicit
  Path(pathid_t p):
    m_path{ p }
  {}
private:
  static PoolT s_pool;
  pathid_t m_path;
};

template<typename PoolT,int poolno>
PoolT Path<PoolT,poolno>::s_pool {};

#endif /* PATH_H */
