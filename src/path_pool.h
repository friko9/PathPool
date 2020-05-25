#ifndef PATH_POOL_H
#define PATH_POOL_H

#include "./field_ref_iterator.h"

#include <boost/flyweight.hpp>
#include <unordered_map>
#include <string>
#include <vector>

struct HashPathPool;
struct ListPathPool;

struct HashPathPool
{
  using key_t = size_t;
  using tag_t = boost::flyweight<std::string>;
private:
  struct node_t : public std::unordered_map<tag_t,key_t>
  {
    node_t(tag_t name, key_t parent,const std::unordered_map<tag_t,key_t>& children):
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
      nodes.push_back(node_t{subnode,path,{}});
    return result.first->second;
  }
  std::vector<key_t> get_siblings(key_t path) const
  {
    std::vector<key_t> result;
    auto value_t_ref = &std::decay<decltype(*nodes[path].begin())>::type::second;
    path = get_parent(path);
    std::copy(field_ref(nodes[path].begin(),value_t_ref),
	      field_ref(nodes[path].end(),value_t_ref),
	      std::back_inserter(result));
    return result;
  }
  key_t get_parent(key_t path) const
  {
    return nodes[path].m_parent;
  }
  tag_t get_tagname(key_t path) const
  {
    return nodes[path].m_name;
  }
  std::vector<tag_t> get_pathname(key_t path) const
  {
    std::vector<tag_t> result;  
    for(; path != 0; path = nodes[path].m_parent)
      result.push_back(get_tagname(path));
    return result;
  }
private:
  std::vector<node_t> nodes {{node_t{tag_t{""},key_t{0},{}}}};
};


struct ListPathPool
{
  using key_t = size_t;
  using tag_t = boost::flyweight<std::string>;
private:
  struct node_t
  {
    node_t(tag_t name, key_t parent,key_t sibling, key_t child):
      m_child{child},
      m_sibling{sibling},
      m_parent{parent},
      m_name{name}
    {}
    key_t m_child;
    key_t m_sibling;
    key_t m_parent;
    tag_t m_name;
  };
public:
  key_t get_subnode(key_t path,tag_t subnode)
  {
    key_t node = path;
    for(node = nodes[node].m_child; node != 0; path = nodes[node].m_sibling)
      if(get_tagname(node) == subnode)
	return node;
    return insert_subnode(path,subnode);
  }
  std::vector<key_t> get_siblings(key_t path) const
  {
    std::vector<key_t> result;
    for(path = nodes[get_parent(path)].m_child; path != 0; path = nodes[path].m_sibling)
      result.push_back(path);
    return result;
  }
  key_t insert_subnode(key_t path, tag_t subnode)
  {
    key_t node = nodes.size();
    nodes.push_back(node_t{subnode,path,nodes[path].m_child,0});
    nodes[path].m_child = node;
    return node;
  }
  key_t get_parent(key_t path) const
  {
    return nodes[path].m_parent;
  }
  tag_t get_tagname(key_t path) const
  {
    return nodes[path].m_name;
  }
  std::vector<tag_t> get_pathname(key_t path) const;
private:
  std::vector<node_t> nodes {{node_t{tag_t{""},0,0,0}}};
};
#endif /* PATH_POOL_H */
