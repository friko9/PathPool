#include "testobj.h"
#include "path_pool.h"
#include "utest.h"

#include <gtest/gtest.h>

using namespace std::string_literals;

using namespace std;
using namespace testing;

class EmptyShort : public TestWithParam<const TestObjBase<int>*>
{
public:
  void SetUp() override
  {
    m_test_obj = (this->GetParam())->clone();
  }
  void TearDown() override
  {
    delete m_test_obj;
  }
public:
  TestObjBase<int>* m_test_obj;
};

namespace {
  using tag_t = int;
  tag_t root_tag = 0;
  
  
  //TEST SUITE
  //SUBJECT empty <PathPool>
  //RESULT <PathPool> root parent = root
  TEST_P(EmptyShort, RootParentIsRoot)
  {
    auto root = m_test_obj->get_root();
    auto root_parent = m_test_obj->get_parent(root);
    ASSERT_EQ(root,root_parent);
  }

  //TEST SUITE
  //SUBJECT empty <PathPool>
  //RESULT root path is as defined
  TEST_P(EmptyShort, RootNameIsAsSpecified)
  {
    auto root = m_test_obj->get_root();
    ASSERT_EQ(m_test_obj->get_tag(root),root_tag);
  }

  //TEST SUITE
  //SUBJECT empty <PathPool>
  //RESULT root path has no subnodes
  TEST_P(EmptyShort, RootHasNoSiblings)
  {
    auto root = m_test_obj->get_root();
    auto subnodes = m_test_obj->get_subnodes(root);
    ASSERT_EQ(std::distance(subnodes.first,subnodes.second), 0);
  }

  //TEST SUITE
  //SUBJECT empty <PathPool>
  //ACTION single insertion to root
  //RESULT <PathPool> consistent after insertion
  TEST_P(EmptyShort, ProperRootInsert)
  {
    tag_t tag {1};
    auto root = m_test_obj->get_root();
    auto subnode = m_test_obj->get_subnode(root, tag);

    ASSERT_EQ( m_test_obj->get_parent(subnode), root);
    ASSERT_EQ( m_test_obj->get_tag(subnode), tag);
    auto subnode_subs = m_test_obj->get_subnodes(subnode);
    ASSERT_EQ( std::distance(subnode_subs.first,subnode_subs.second), 0);
    auto root_subs = m_test_obj->get_subnodes(root);
    ASSERT_EQ( std::distance(root_subs.first,root_subs.second),1u);
    ASSERT_EQ( *root_subs.first,subnode);
  }

  //TEST SUITE
  //SUBJECT empty <PathPool>
  //ACTION 2x insertions to root
  //RESULT <PathPool> consistent
  TEST_P(EmptyShort, ProperRootInsertTwice)
  {
    tag_t tag {1};
    tag_t tag2 {2};
    auto root = m_test_obj->get_root();
    auto subnode = m_test_obj->get_subnode(root, tag);
    auto subnode_2 = m_test_obj->get_subnode(root, tag2);

    ASSERT_EQ( m_test_obj->get_parent(subnode), root);
    ASSERT_EQ( m_test_obj->get_tag(subnode), tag);
    auto subnode_subs = m_test_obj->get_subnodes(subnode);
    ASSERT_EQ( std::distance(subnode_subs.first,subnode_subs.second), 0);
    ASSERT_EQ( m_test_obj->get_parent(subnode_2), root);
    ASSERT_EQ( m_test_obj->get_tag(subnode_2), tag2);
    auto subnode_subs2 = m_test_obj->get_subnodes(subnode);
    ASSERT_EQ( std::distance(subnode_subs2.first,subnode_subs2.second), 0);
    auto root_subnodes = m_test_obj->get_subnodes(root);
    ASSERT_EQ( std::distance(root_subnodes.first,root_subnodes.second),2u);
    ASSERT_EQ( std::count(root_subnodes.first,root_subnodes.second,subnode),1u);
    ASSERT_EQ( std::count(root_subnodes.first,root_subnodes.second,subnode_2),1u);
  }

  //TEST SUITE
  //SUBJECT empty <PathPool>
  //ACTION insertion to root, insertion to root-subnode
  //RESULT <PathPool> consistent
  TEST_P(EmptyShort, ProperStackedInsertion)
  {
    tag_t tag {1};
    tag_t tag2 {2};
    auto root = m_test_obj->get_root();
    auto subnode = m_test_obj->get_subnode(root, tag);
    auto subnode2 = m_test_obj->get_subnode(subnode, tag2);

    auto root_subs = m_test_obj->get_subnodes(root);
    ASSERT_EQ( std::distance(root_subs.first,root_subs.second),1u);
    ASSERT_EQ( std::count(root_subs.first,root_subs.second,subnode),1u);
    auto subnode_subs = m_test_obj->get_subnodes(subnode);
    ASSERT_EQ( m_test_obj->get_parent(subnode), root);
    ASSERT_EQ( m_test_obj->get_tag(subnode), tag);
    ASSERT_EQ( std::distance(subnode_subs.first,subnode_subs.second), 1);
    ASSERT_EQ( std::count(subnode_subs.first,subnode_subs.second,subnode2),1u);
    auto subnode2_subs = m_test_obj->get_subnodes(subnode2);
    ASSERT_EQ( m_test_obj->get_parent(subnode2), subnode);
    ASSERT_EQ( m_test_obj->get_tag(subnode2), tag2);
    ASSERT_EQ( std::distance(subnode2_subs.first,subnode2_subs.second), 0);

  }

  //TEST SUITE
  //SUBJECT empty <PathPool>
  //ACTION 2x same element insertions to root
  //RESULT <PathPool> consistent
  TEST_P(EmptyShort, ProperRootInsertSameTwice)
  {
    tag_t tag {1};
    auto root = m_test_obj->get_root();
    auto subnode = m_test_obj->get_subnode(root, tag);
    auto subnode_2 = m_test_obj->get_subnode(root, tag);
   
    ASSERT_EQ(subnode,subnode_2);
    ASSERT_EQ( m_test_obj->get_parent(subnode), root);
    ASSERT_EQ( m_test_obj->get_tag(subnode), tag);
    auto subnode_subs = m_test_obj->get_subnodes(subnode);
    ASSERT_EQ( std::distance(subnode_subs.first,subnode_subs.second), 0);
    auto root_subnodes = m_test_obj->get_subnodes(root);
    ASSERT_EQ( std::distance(root_subnodes.first,root_subnodes.second),1u);
    ASSERT_EQ( std::count(root_subnodes.first,root_subnodes.second,subnode),1u);
  }
  
  auto test_objects = Values( NEW_TEST_OBJ(HashPathPool<int>,root_tag),
			      NEW_TEST_OBJ(ListPathPool<int>,root_tag));

  //TEST DATASET
  INSTANTIATE_TEST_CASE_P(PathPools, EmptyShort, test_objects );
}
