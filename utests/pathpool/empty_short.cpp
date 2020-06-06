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
    ASSERT_TRUE(subnodes.empty());
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
    ASSERT_EQ( m_test_obj->get_subnodes(subnode).empty(), true);
    ASSERT_EQ( m_test_obj->get_subnodes(root).size(),1u);
    ASSERT_EQ( m_test_obj->get_subnodes(root)[0],subnode);
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
    ASSERT_EQ( m_test_obj->get_subnodes(subnode).empty(), true);
    ASSERT_EQ( m_test_obj->get_parent(subnode_2), root);
    ASSERT_EQ( m_test_obj->get_tag(subnode_2), tag2);
    ASSERT_EQ( m_test_obj->get_subnodes(subnode_2).empty(), true);
    auto root_subnodes = m_test_obj->get_subnodes(root);
    ASSERT_EQ( root_subnodes.size(),2u);
    ASSERT_EQ( std::count(root_subnodes.begin(),root_subnodes.end(),subnode),1u);
    ASSERT_EQ( std::count(root_subnodes.begin(),root_subnodes.end(),subnode_2),1u);
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
    auto subnode_2 = m_test_obj->get_subnode(subnode, tag2);

    ASSERT_EQ( m_test_obj->get_parent(subnode), root);
    ASSERT_EQ( m_test_obj->get_tag(subnode), tag);
    ASSERT_EQ( m_test_obj->get_subnodes(subnode).size(), 1);
    ASSERT_EQ( m_test_obj->get_parent(subnode_2), subnode);
    ASSERT_EQ( m_test_obj->get_tag(subnode_2), tag2);
    ASSERT_EQ( m_test_obj->get_subnodes(subnode_2).empty(), true);
    auto root_subnodes = m_test_obj->get_subnodes(root);
    auto subnode_subnodes = m_test_obj->get_subnodes(subnode);
    ASSERT_EQ( root_subnodes.size(),1u);
    ASSERT_EQ( std::count(root_subnodes.begin(),root_subnodes.end(),subnode),1u);
    ASSERT_EQ( std::count(subnode_subnodes.begin(),subnode_subnodes.end(),subnode_2),1u);
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
    ASSERT_EQ( m_test_obj->get_subnodes(subnode).empty(), true);
    auto root_subnodes = m_test_obj->get_subnodes(root);
    ASSERT_EQ( root_subnodes.size(),1u);
    ASSERT_EQ( std::count(root_subnodes.begin(),root_subnodes.end(),subnode),1u);
  }
  
  auto test_objects = Values( NEW_TEST_OBJ(HashPathPool<int>,root_tag),
			      NEW_TEST_OBJ(ListPathPool<int>,root_tag));

  //TEST DATASET
  INSTANTIATE_TEST_CASE_P(PathPools, EmptyShort, test_objects );
}
