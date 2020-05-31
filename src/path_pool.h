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
  using pathid_t = size_t;
  using tag_t = boost::flyweight<std::string>;
public:
  static constexpr pathid_t null {0};
public:
  HashPathPool(tag_t root = {}):
    m_nodes {{node_t{root,null,{}}}}
  {}
  pathid_t get_subnode(pathid_t path,tag_t subnode)
  {
    auto result = m_nodes[path].emplace(subnode,m_nodes.size());
    if(result.second)
      m_nodes.push_back(node_t{subnode,path,{}});
    return result.first->second;
  }
  std::vector<pathid_t> get_subnodes(pathid_t path) const
  {
    std::vector<pathid_t> result;
    auto value_t_ref = &std::decay<decltype(*m_nodes[path].begin())>::type::second;
    std::copy(field_ref(m_nodes[path].begin(),value_t_ref),
	      field_ref(m_nodes[path].end(),value_t_ref),
	      std::back_inserter(result));
    return result;
  }
  pathid_t get_parent(pathid_t path) const
  {
    return m_nodes[path].m_parent;
  }
  tag_t get_tag(pathid_t path) const
  {
    return m_nodes[path].m_name;
  }
  pathid_t get_root() const noexcept
  {
    return rootid;
  }
private:
  struct node_t : public std::unordered_map<tag_t,pathid_t>
  {
    node_t(tag_t name, pathid_t parent,const std::unordered_map<tag_t,pathid_t>& children):
      std::unordered_map<tag_t,pathid_t>{children},
      m_parent{parent},
      m_name{name}
    {}
    pathid_t m_parent;
    tag_t m_name;
  };
private:
  static constexpr pathid_t rootid {0};
  std::vector<node_t> m_nodes;
};


struct ListPathPool
{
  using pathid_t = size_t;
  using tag_t = boost::flyweight<std::string>;
  static constexpr pathid_t null {0};
public:
  ListPathPool(tag_t root = {}):
    m_nodes {{node_t{root,null,null,null}}}
  {}
  pathid_t get_subnode(pathid_t path,tag_t subnode)
  {
    pathid_t node = path;
    for(node = m_nodes[node].m_child; node != null; node = m_nodes[node].m_sibling)
      if(get_tag(node) == subnode)
	return node;
    return insert_subnode(path,subnode);
  }
  std::vector<pathid_t> get_subnodes(pathid_t path) const
  {
    std::vector<pathid_t> result;
    for(path = m_nodes[path].m_child; path != null; path = m_nodes[path].m_sibling)
      result.push_back(path);
    return result;
  }
  pathid_t get_parent(pathid_t path) const
  {
    return m_nodes[path].m_parent;
  }
  tag_t get_tag(pathid_t path) const
  {
    return m_nodes[path].m_name;
  }
  pathid_t get_root() const noexcept
  {
    return rootid;
  }
private:
  pathid_t insert_subnode(pathid_t path, tag_t subnode)
  {
    pathid_t i = m_nodes.size();
    m_nodes.push_back(node_t{subnode,path,m_nodes[path].m_child,null});
    return m_nodes[path].m_child = i;
  }
private:
    private:
  struct node_t
  {
    node_t(tag_t name, pathid_t parent,pathid_t sibling, pathid_t child):
      m_child{child},
      m_sibling{sibling},
      m_parent{parent},
      m_name{name}
    {}
    pathid_t m_child;
    pathid_t m_sibling;
    pathid_t m_parent;
    tag_t m_name;
  };
private:
  static constexpr pathid_t rootid {0};
  std::vector<node_t> m_nodes;
};

template<typename PathPoolT>
std::vector<typename PathPoolT::tag_t>
to_taglist(const PathPoolT& pool,typename PathPoolT::pathid_t path)
{
  std::vector<typename PathPoolT::tag_t> result;  
  for(; path != PathPoolT::root; path = pool.get_parent(path))
    result.push_back(pool.get_tag(path));
  result.push_back(pool.get_tag(path));
  return result;
}
#endif /* PATH_POOL_H */
