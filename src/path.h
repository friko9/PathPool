#ifndef PATH_H
#define PATH_H

#include <algorithm>
#include <vector>
#include <functional>
#include <type_traits>

template<typename PoolT,int poolno>
class Path;

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

template<typename PoolT,int poolno>
bool operator <(const Path<PoolT,poolno>& l, const Path<PoolT,poolno>& r)
{
  using PathT = Path<PoolT,poolno>;
  auto split = get_common_path(l.m_path,r.m_path,PathT::s_pool);
  if(split[1] == split[2]) return false;
  if(split[0] == split[1]) return true;
  if(split[0] == split[2]) return false;
  return PathT::s_pool.get_tag(split[1]) < PathT::s_pool.get_tag(split[2]);
}

template<typename PoolT,int poolno = 0>
class Path
{
  friend
  Path<PoolT,poolno>
  common_path<>(const Path<PoolT,poolno>& l, const Path<PoolT,poolno>& r);
  friend
  bool operator == <>(const Path<PoolT,poolno>& l, const Path<PoolT,poolno>& r);
  friend
  bool operator < <>(const Path<PoolT,poolno>& l, const Path<PoolT,poolno>& r);
private:
  using pathid_t = typename PoolT::pathid_t;
public:
  class horizontal_iterator;
  class vertical_iterator;
  using tag_t = typename PoolT::tag_t;
public:
  Path():
    m_path{s_pool.get_root()}
  {}
  Path(const Path& parent,const tag_t& name):
    m_path{ s_pool.get_subnode(parent.m_path, name)}
  {}
  Path operator /(const tag_t& t) const
  {
    return Path { *this, t };
  }
  std::pair<horizontal_iterator,horizontal_iterator>
  get_subpaths() const 
  {
    auto subpaths = s_pool.get_subnodes(m_path);
    return std::make_pair(horizontal_iterator{subpaths.first}, horizontal_iterator{subpaths.second});
  }
  Path get_parent() const
  {
    return Path{ s_pool.get_parent(m_path) };
  }
  tag_t get_tag() const
  {
    return s_pool.get_tag(m_path);
  }
  vertical_iterator begin()
  {
    return vertical_iterator {*this};
  }
  vertical_iterator end()
  {
    return vertical_iterator {};
  }
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
class Path<PoolT,poolno>::horizontal_iterator
{
  friend Path;
  using it_t = typename PoolT::iterator_t;
public:
  using iterator_category = std::forward_iterator_tag;
  using value_type = Path;
  using difference_type = size_t;
  using pointer = value_type*;
  using reference = value_type&;
public:
  horizontal_iterator()
  {}
  value_type operator *(){return Path{*m_it};}
  bool operator ==(horizontal_iterator const& r) const
  {
    return m_it == r.m_it;
  }
  bool operator !=(horizontal_iterator const& r) const
  {
    return !(*this == r);
  }
  horizontal_iterator& operator ++()
  {
    ++m_it;
    return *this;
  }
  horizontal_iterator operator ++(int)
  {
    return horizontal_iterator{m_it++};
  }
private:
  explicit
  horizontal_iterator(it_t it):
    m_it{it}
  {}
private:
  typename PoolT::iterator_t m_it {{},s_pool};
};

template<typename PoolT,int poolno>
class Path<PoolT,poolno>::vertical_iterator
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
  vertical_iterator(Path path):
    m_it{path}
  {}
  vertical_iterator()
  {}
  value_type operator *(){return m_it;}
  bool operator ==(vertical_iterator const& r) const
  {
    return m_it == r.m_it;
  }
  bool operator !=(vertical_iterator const& r) const
  {
    return !(*this == r);
  }
  vertical_iterator& operator ++()
  {
    this->m_it = this->m_it.get_parent();
    return *this;
  }
  vertical_iterator operator ++(int)
  {
    vertical_iterator result = *this;
    ++*this;
    return result;
  }
protected:
  Path m_it {};
};

template<typename PoolT,int poolno>
PoolT Path<PoolT,poolno>::s_pool {};

#endif /* PATH_H */
