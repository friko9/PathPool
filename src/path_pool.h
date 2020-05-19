#ifndef PATH_POOL_H
#define PATH_POOL_H

#include "./field_ref_iterator.h"

#include <boost/flyweight.hpp>
#include <unordered_map>
#include <string>
#include <vector>

class PathPoolHashmap
{
public:
  using key_t = size_t;
  using tag_t = boost::flyweight<std::string>;
private:
  struct node_t : public std::unordered_map<tag_t,key_t>
  {
    node_t(const std::unordered_map<tag_t,key_t>& children, tag_t name, key_t parent):
      std::unordered_map<tag_t,key_t>{children},
      m_parent{parent},
      m_name{name}
    {}
    key_t m_parent;
    tag_t m_name;
  };
public:
  key_t get_subnode(key_t path,tag_t subnode)
  {
    auto result = nodes[path].emplace(subnode,nodes.size());
    if(result.second)
      nodes.push_back(node_t{{},subnode,path});
    return result.first->second;
  }
  key_t get_parent(key_t path)
  {
    return nodes[path].m_parent;
  }
  std::vector<key_t> get_siblings(key_t path)
  {
    std::vector<key_t> result;
    auto value_t_ref = &std::decay<decltype(*nodes[path].begin())>::type::second;
    std::copy(field_ref(nodes[path].begin(),value_t_ref),
	      field_ref(nodes[path].end(),value_t_ref),
	      std::back_inserter(result));
    return result;
  }
  tag_t get_tagname(key_t path)
  {
    return nodes[path].m_name;
  }
  std::vector<tag_t> get_pathname(key_t path)
  {
    std::vector<tag_t> result;  
    for(; path != 0; path = nodes[path].m_parent)
      result.push_back(get_tagname(path));
    return result;
  }
private:
  std::vector<node_t> nodes {{node_t{{},tag_t{""},key_t{0}}}};
  tag_t x;
};

#endif /* PATH_POOL_H */
