#ifndef PATH_POOL_H
#define PATH_POOL_H

#include <boost/flyweight.hpp>
#include <unordered_map>
#include <string>
#include <vector>


template <typename T,typename... tail>
inline
T make(tail... args)
{
  return T{args...};
}

template<>
class std::hash<boost::flyweight<std::string>>
{
public:
  size_t operator()(boost::flyweight<std::string> arg) const noexcept
  {
    using hashed_t = std::decay<decltype(arg.get_key())>::type;
    return std::hash<hashed_t>{}(arg.get_key());
  }
};


class PathPoolHashmap
{
public:
  using index_t = size_t;
  using key_t = boost::flyweight<std::string>;
private:
  struct node_t
  {
    node_t() = default;
    node_t(const node_t&) = default;
    node_t(const std::unordered_map<key_t,index_t>& children,index_t parent):
      m_children{children},m_parent{parent}
    {}
    std::unordered_map<key_t,index_t> m_children;
    index_t m_parent;
  };
public:
  void f()
  {
    x.get_key();
  }
private:
  std::unordered_map<key_t,node_t> nodes {{key_t{"/"},node_t{{},index_t{0}}}};
  key_t x;
};

#endif /* PATH_POOL_H */
