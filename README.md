# PathPool

[![Codacy Badge](https://app.codacy.com/project/badge/Grade/f9ebc1a9abf74544a509967d81ee2e28)](https://www.codacy.com/manual/friko9/PathPool?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=friko9/PathPool&amp;utm_campaign=Badge_Grade) ![build+unittest](https://github.com/friko9/PathPool/workflows/build+unittest/badge.svg?branch=master) [![Coverage Status](https://coveralls.io/repos/github/friko9/PathPool/badge.svg?branch=master)](https://coveralls.io/github/friko9/PathPool?branch=master)

## Rationale

PathPool concept aims to minimize computational costs for storing and operating on multiple path entries in the system.

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
Path pools describe its paths similar to flyweight design pattern. Despite its memory savings flyweights, have also optimal computational cost for copying, moving, and equality-comparison operations. Efficiency loss occurs on path-string retrieval.

A design emerges where each node is defined once, and sub-paths contain only reference to their parent node - this leads to a centralized path pool register with tree-like structures.
Each path is a list with head pointing at its least significant tag. Head tag may be shared only with its subpaths. Head may be accessed using its id (`pathid_t`). Next node in the list is head's parent node. Last element of a list is root path shared by all paths in the pool. To allow for path operations, and local traversal, helper `get_*` methods are used.

## PathPool classes

All PathPool classes share a common interface - it is not formalized, as no inheritance is used.

### ``HashPathPool<TagT, AllocatorT, HashF, EqualsF>``
 
 Uses hash-map to store subnodes. Faster for pools with large number of subnodes with the same parent
	 
 * `TagT` 
 
 Stored type. `tag_t = TagT`
 
 * `AllocatorT = std::allocator`
 
 STL Allocator
 
 * `HashF = std::hash<TagT>`
 
 Hashing function.
 
 * `EqualsF = std::equal_to<TagT>`

 Equality predicate

### ``ListPathPool<TagT, AllocatorT>``
 
 Uses array linked-lists to store subnodes. Faster in most cases.
	 
 * `TagT` 
 
 Stored type. `tag_t = TagT`

 * `AllocatorT = std::allocator`
 
 STL Allocator
 
### Defined types:

  * `tag_t = TagT`
  
  Defines types paths are composed of. Type must allow for:

	* copying
	* equality comparison
	* (for HashPathPool) hashing
  
  * `pathid_t`
  
  Defines path-id which explicitly describes a path. Allows for:

	* path identification in the pool.
	* comparison
	* hashing
	* value operations like assignment, copying

### Methods:

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

	Returns a tag associated with a given path
	
  * `get_root() const noexcept -> pathid_t`

	Returns root path

### Global functions:

  * `get_taglist(PathPool,pathid_t) -> std::vector<tag_t>`

    Returns list of tags_t describing full path name in reverse order (from the least to the most significant tag)

  * `get_common_path(pathid_t,pathid_t, PathPool) -> std::array<pathid_t,3>`

    Returns 3 elements describing connection point of 2 paths `{common_node, left_subnode, right_subnode}`

## Path class

### ``Path<PoolT,int poolno = 0>``

Object representation of `pathid_t`. Takes only a space of `sizeof(pathid_t)`, and shares `PathPool` object for all its types. User cannot access `PathPool` object. Tag of root path. holds default value.

 * `PoolT` - concrete PathPool class
 * `poolno` - pool number. Allows creation of multiple pools of the same PoolT.
 
Example: `Path<ListPathPool<int>,0> root_1;`  
Example: `Path<ListPathPool<int>,1> root_2;`  
Example: `Path<HashPathPool<int>,0> root_3;`  

`root_1`,`root_2`,`root_3` represent 3 different types and no interaction between them is possible.

### Defined types

 * `tag_t = PathPool::tag_t`
 
 Defines type used for creating subpaths. tag_t must have default constructor.
 
 * `vertical_iterator`

 Forward-iterator used for traversing the path up.

 * `horizontal_iterator`

 Forward-iterator used for traversing the paths of the same parent.

### Methods

 * `constructor()`
 
 Creates root path.
 
 * `constructor(const Path&,const tag_t&)`
 
 Creates subpath.
 
 * `operator / (const tag_t&) const -> Path`
 
 Creates subpath.
 
 * `get_subpaths() -> std::pair<horizontal_iterator,horizontal_iterator>`
 
 Returns all subpaths referenced from the called path.
 
 * `get_parent() const -> Path`
 
 Returns parent of the called object
 
 * `get_tag() -> tag_t`
 
 Returns tag given on path creation.
 
 * `begin() -> iterator_t`
 
 Returns iterator pointing at called Path.
 
 * `end() -> iterator_t`
 
 Returns iterator pointing at root Path.
 
### Global functions

 * `operator == (const Path&, const Path&) -> bool`
 
 * `operator < (const Path&, const Path&) -> bool`

 * `common_path(const Path&, const Path&) -> Path`

 Returns common path of its two arguments.
