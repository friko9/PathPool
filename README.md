# PathPool

[![Codacy Badge](https://app.codacy.com/project/badge/Grade/f9ebc1a9abf74544a509967d81ee2e28)](https://www.codacy.com/manual/friko9/PathPool?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=friko9/PathPool&amp;utm_campaign=Badge_Grade) ![build+unittest](https://github.com/friko9/PathPool/workflows/build+unittest/badge.svg?branch=master) [![Coverage Status](https://coveralls.io/repos/github/friko9/PathPool/badge.svg?branch=master)](https://coveralls.io/github/friko9/PathPool?branch=master)

## Rationale

PathPool concept aims to minimize computional costs for storing and operating on multiple path entries in the system.

Each path in any given namespace must have following qualities:

 1. have parent path
 2. share a common prefix ( also empty prefix )
 3. provide means to explicitly compare any two paths

Minimal requirement for the given path concept requires:

  * sharing namespace on which they navigate
  * using same prefixes
  * keeping order of it's elements.
  * allowing for equality comparison

Each path can be described as:

 1. string describing full path
 2. linked list of tags describing full path
 3. a pair of:
   * reference to parent path
   * tag - element expanding parent path

## Implementation

The most memory efficient solution is a pool of paths described as pairs <parent path, expanding tag>. It allows for sharing prefixes for all paths in a pool so adding a path has memory complexity of O(1), as new memory is allocated only for the expanding tag - instead the whole path. Pair <parent path, expanding tag> can uniquely describe a path and in consequence be coupled to unique path id.
Path pools describe its paths similar to flyweight design pattern. Despite its memory savings flyweights, have also optimal computional cost for copying, moving, and equality-comparison operations. Efficiency loss occurs on path-string retrival.

A design emerges where each node is defined once, and sub-paths contain only reference to their parent node - this leads to a centralized path pool register with tree-like structures.
Each path is a list with head pointing at its least significant tag. Head tag may be shared only with its subpaths. Head may be accessed using its id (`pathid_t`). Next node in the list is head's parent node. Last element of a list is root path shared by all paths in the pool. To allow for path operations, and local taversal, helper `get_*` methods are used.

## Interface description

All PathPool classes share a common interface - it is not formalized, as no inheritence is used.

User interaction is done with 2 types:

- `tag_t` - defined by the user as template parameter. Type must allow for:
 
  * copying
  * equality comparison
  * (for HashPathPool) hashing
 
- `pathid_t` - explicitly describes a path. Allows for:

  * path identification in the pool.
  * comparison
  * hashing
  * value operations like assignment, copying

### Defined types:

  * `tag_t = TagT`
  
  Defines types paths are composed of
  
  * `pathid_t`
  
  Defines path id, by which paths can be reffered to

### Supported methods:

  * `constructor()`
  
	Create pool with default root path - only if `tag_t` provides default constructor
	
  * `constructor(tag_t root)`
  
  * `get_subnode(pathid_t path,tag_t subnode) -> pathid_t`
  
	Returns a path described by `pair<path,subnode>`. Path is created if it doesn't exist
	
  * `get_subnodes(pathid_t path) const -> std::pair<iterator_t,iterator_t>`
  
	Returns a range of iterators to subpath list
	
  * `get_subnodes<ContainerT>(pathid_t path) const -> ResultT`
  
  	Returns all existing sub-paths in any given container supporting `std::back_inserter`
	
  * `get_parent(pathid_t path) const -> pathid_t`
  
	Returns a parent of a given path
	
  * `get_tag(pathid_t path) const -> tag_t`

	Returns a tag assosiated with a given path
	
  * `get_root() const noexcept -> pathid_t`

	Returns root path

### Global functions:

  * `get_taglist(PathPool,pathid_t) -> std::vector<tag_t>`

    Returns list of tags_t describing full path name in reverse order (from the least to the most significant tag)

  * `get_common_path(pathid_t,pathid_t, PathPool) -> std::array<pathid_t,3>`

    Returns 3 elements describing connection point of 2 paths `{common_node, left_subnode, right_subnode}`

## Classes

### ``HashPathPool<TagT, AllocatorT = std::allocator,HashF = std::hash<TagT>,EqualsF = std::equal_to<TagT>>``
 
 Uses hashmap to store subnodes. Faster for pools with large number of subnodes with the same parent
	 
 * `TagT = tag_t` - stored type
 * `AllocatorT` - STL Allocator
 * `HashF` - Hashing function
 * `EqualsF` - equality predicate

### ``ListPathPool<TagT, AllocatorT = std::allocator>``
 
 Uses array linked-lists to store subnodes. Faster in most cases.
	 
 * `TagT = tag_t` - stored type
 * `AllocatorT` - STL Allocator
