#ifndef PATH_POOL_H
#define PATH_POOL_H

#include "field_ref_iterator.h"

#include <unordered_map>
#include <string>
#include <vector>

#include <functional>

template<typename TagT, template<typename T> class AllocatorT, typename HashF,typename EqualsF> 
class HashPathPool;

template<typename TagT, template<typename T> class AllocatorT> 
class ListPathPool;

template<typename TagT, template<typename T> class AllocatorT = std::allocator, typename HashF = std::hash<TagT>,typename EqualsF = std::equal_to<TagT>> 
class HashPathPool
{
  static_assert(std::is_copy_constructible<TagT>(),"Template argument is not copy-constructible");
  // TagT is EmplaceConstructible - EmplaceConstructible is well formed
  // TagT must be Erasable, CopyInsertable, allow for: unordered_map::hasher(TagT), TagT == TagT
  struct node_t;
  using ContainerT = typename std::vector<node_t,AllocatorT<node_t>>;
  using PoolT = HashPathPool<TagT,AllocatorT,HashF,EqualsF>;
public:
  class iterator_t;
  using tag_t = TagT;
  using pathid_t = typename ContainerT::size_type;
public:
  template <typename = typename std::enable_if<std::is_default_constructible<tag_t>::value>::type>
    HashPathPool():
      HashPathPool(tag_t{})
  {}
  explicit
  HashPathPool(tag_t root):
    m_nodes {{node_t{root,null,{}}}}
  {}
  pathid_t get_subnode(pathid_t path,tag_t subnode)
  {
    auto result = m_nodes[path].emplace(subnode,m_nodes.size());
    if(result.second)
      m_nodes.push_back(node_t{subnode,path,{}});
    return result.first->second;
  }
  std::pair<iterator_t,iterator_t> get_subnodes(pathid_t path) const
  {
    return { iterator_t{m_nodes[path].cbegin()}, iterator_t{m_nodes[path].cend()}};
  }
  template<typename ResultT>
  ResultT get_subnodes(pathid_t path) const
  {
    ResultT result;
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
public:
  class iterator_t
  {
    using it_t = typename node_t::MapT::const_iterator;
  public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = pathid_t;
    using difference_type = decltype (std::declval<value_type>() - std::declval<value_type>());
    using pointer = value_type*;
    using reference = value_type&;
  public:
    explicit
    iterator_t(it_t it):
      m_it{it}
    {}
    ~iterator_t(){};
    value_type operator *(){return m_it->second;}
    iterator_t& operator ++(){ ++m_it; return *this; };
    iterator_t operator ++(int){ iterator_t result = *this; ++*this; return result; }
    iterator_t& operator --(){ return --m_it; };
    iterator_t operator --(int){ iterator_t result = *this; --*this; return result; }
    bool operator ==(iterator_t const& r) const
    {
      return m_it == r.m_it;
    }
    bool operator !=(iterator_t const& r) const
    {
      return m_it != r.m_it;
    }
  private:
    it_t m_it;
  };
private:
  struct node_t : public std::unordered_map<tag_t, pathid_t, HashF, EqualsF, AllocatorT<std::pair<tag_t,pathid_t>>>
  {
    using MapT = std::unordered_map<tag_t, pathid_t, HashF, EqualsF, AllocatorT<std::pair<tag_t,pathid_t>>>;
    node_t(tag_t name, pathid_t parent,const MapT& children):
      MapT{children},
      m_parent{parent},
      m_name{name}
    {}
    pathid_t m_parent;
    tag_t m_name;
  };
private:
  static constexpr pathid_t null {0};
  static constexpr pathid_t rootid {0};
  ContainerT m_nodes;
};

template<typename TagT, template<typename T> class AllocatorT = std::allocator> 
class ListPathPool
{
  static_assert(std::is_copy_constructible<TagT>(),"Template argument is not copy-constructible");
  // TagT is EmplaceConstructible - EmplaceConstructible is well formed
  // TagT must be Erasable, CopyInsertable, allow for: unordered_map::hasher(TagT), TagT == TagT
  struct node_t;
  using PoolT =  ListPathPool<TagT,AllocatorT>;
  using ContainerT = typename std::vector<node_t,AllocatorT<node_t>>;
public:
  struct iterator_t;
  using tag_t = TagT;
  using pathid_t = typename ContainerT::size_type;
public:
  template <typename = typename std::enable_if<std::is_default_constructible<tag_t>::value>::type>
  ListPathPool():
    ListPathPool(tag_t{})
  {}
  explicit
  ListPathPool(tag_t root):
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
  std::pair<iterator_t,iterator_t> get_subnodes(pathid_t path) const
  {
    return { iterator_t{m_nodes[path].m_child, *this } , iterator_t{ null, *this}  };
  }
  template<typename ResultT>
  ResultT get_subnodes(pathid_t path) const
  {
    ResultT result;
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
  pathid_t get_next_sibling(pathid_t path) const
  {
    return m_nodes[path].m_sibling;
  }
public:
  struct iterator_t
  {
    using iterator_category = std::forward_iterator_tag;
    using value_type = pathid_t;
    using difference_type = decltype (std::declval<value_type>() - std::declval<value_type>());
    using pointer = value_type*;
    using reference = value_type&;
  public:
    explicit
    iterator_t(value_type val,const PoolT& pool):
      m_pool{pool},
      m_val{val}
    {}
    ~iterator_t(){};
    value_type operator *(){return m_val;}
    iterator_t& operator ++(){ m_val = m_pool.get_next_sibling(m_val); return *this; };
    iterator_t operator ++(int){ iterator_t result = *this; ++*this; return result; }
    bool operator ==(iterator_t const& r) const
    {
      return (m_val == r.m_val) & (&m_pool == &r.m_pool);
    }
    bool operator !=(iterator_t const& r) const
    {
      return !(*this == r);
    }
  private:
    const PoolT& m_pool;
    value_type m_val;
  };
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
  static constexpr pathid_t null {0};
  static constexpr pathid_t rootid {0};
  ContainerT m_nodes;
};

template<typename PathPoolT>
std::vector<typename PathPoolT::tag_t>
get_taglist(const PathPoolT& pool,typename PathPoolT::pathid_t path)
{
  std::vector<typename PathPoolT::tag_t> result;  
  for(; path != PathPoolT::root; path = pool.get_parent(path))
    result.push_back(pool.get_tag(path));
  result.push_back(pool.get_tag(path));
  return result;
}

#endif /* PATH_POOL_H */
