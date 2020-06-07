#PathPool

[![Codacy Badge](https://app.codacy.com/project/badge/Grade/f9ebc1a9abf74544a509967d81ee2e28)](https://www.codacy.com/manual/friko9/PathPool?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=friko9/PathPool&amp;utm_campaign=Badge_Grade)

##Rationale

PathPool archetype aims to minimize computional costs when storing and operating on multiple path entries in the system.

Each path in namespace must have following qualities:

1. have parent path
2. provide unique description of destination in namespace

Thou each path can be described as a pair of:

* reference to parent path
* tag - element expanding parent path

## Implementation
Natural consequence of path requirements is implementing path as forward-lists.
Paths share namespace on which they navigate and often share same prefixes - in list implementation they share parent nodes.

An approach emerges where each node is defined once, and sub-paths contain only reference to the parent node - this leads to creaiting centralized PathPool register.
PathPools are tree-like structures where each node is defined once, allows for unique reference for each node, and node-traversal is mostly done locally.

## Interface description

There is a common interface for all *PathPool classes. It may be extended by specific classes, but the core functionality stays uniform.
No inheritence is used - its up to user design. 
User interaction is performed with 2 types: tag\_t and pathid\_t

tag\_t (template parameter). pair of <parent pathid, tag> explicitly describes path.
Must allow for:

* copying
* equality comparison
* (in HashPathPool) hashing

pathid\_t - explicily describes a path.
Allows for:

* path identification in the pool.
* comparison
* hashing
* value operations like assignment, copying

Supported template parameters:

* `TagT` - type to be used to differentiate subnodes

Defined types:

* `tag_t = TagT`
* `pathid_t`

Supported methods:

* `constructor() - only if tag_t has default constructor`
* `constructor(tag_t root)`
* `get_subnode(pathid_t path,tag_t subnode) -> pathid_t`
* `get_subnodes(pathid_t path) const -> std::vector<pathid_t>`
* `get_subnodes<ContainerT>(pathid_t path) const -> ResultT`
* `get_parent(pathid_t path) const -> pathid_t`
* `get_tag(pathid_t path) const -> tag_t`
* `get_root() const noexcept -> pathid_t`

## Classes

1. ``HashPathPool<TagT, AllocatorT = std::allocator,HashF = std::hash<TagT>,EqualsF = std::equal_to<TagT>>``

	Uses hashmap to store subnodes. Fast in structures with large number of subnodes with the same parent
2. ``ListPathPool<TagT, AllocatorT = std::allocator>``

   Uses linked-list in array approach. Faster in most cases.
