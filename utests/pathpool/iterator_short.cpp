#ifndef ITERATOR_SHORT_H
#define ITERATOR_SHORT_H

#include "utest.h"
#include "testobj.h"
#include "path_pool.h"

#include <gtest/gtest.h>

using namespace std::string_literals;

using namespace std;
using namespace testing;

namespace {

  class Iterators : public TestWithParam<const TestObjBase<int>*>
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

  using tag_t = int;
  tag_t root_tag = 0;
  
  //TEST SUITE
  //SUBJECT empty <PathPool>
  //RESULT root subnodes range start = end
  TEST_P(Iterators, EmptyRange)
  {
    auto root = m_test_obj->get_root();
    auto root_subs = m_test_obj->get_subnodes(root);
    ASSERT_EQ(root_subs.first,root_subs.second);
    auto root_subs2 = m_test_obj->get_subnodes(root);
    ASSERT_EQ(root_subs2.first,root_subs.first);
    ASSERT_EQ(root_subs2.first,root_subs.second);
  }

  //TEST SUITE
  //SUBJECT populated <PathPool>
  //RESULT root subnodes range doesn't change
  TEST_P(Iterators, EqualRanges)
  {
    auto root = m_test_obj->get_root();
    m_test_obj->get_subnode(root,1);
    //Test
    auto root_subs = m_test_obj->get_subnodes(root);
    auto root_subs2 = m_test_obj->get_subnodes(root);
    ASSERT_EQ(root_subs2.first,root_subs.first);
    ASSERT_EQ(root_subs2.second,root_subs.second);
  }

  //TEST SUITE
  //SUBJECT populated <PathPool>
  //RESULT proper prefix incrementation
  TEST_P(Iterators, PrefixIncrement)
  {
    auto root = m_test_obj->get_root();
    m_test_obj->get_subnode(root,1);
    //Test
    auto root_subs = m_test_obj->get_subnodes(root);
    ASSERT_EQ(++root_subs.first,root_subs.second);
  }

  //TEST SUITE
  //SUBJECT populated <PathPool>
  //RESULT proper postfix incrementation
  TEST_P(Iterators, PostfixIncrement)
  {
    auto root = m_test_obj->get_root();
    m_test_obj->get_subnode(root,1);
    //Test
    auto root_subs = m_test_obj->get_subnodes(root);
    auto root_subs2 = m_test_obj->get_subnodes(root);
    ASSERT_EQ(root_subs2.first++,root_subs.first);
  }

  auto test_objects = Values( NEW_TEST_OBJ_DEFAULT(HashPathPool<tag_t>),
			      NEW_TEST_OBJ_DEFAULT(ListPathPool<tag_t>),
			      NEW_TEST_OBJ(HashPathPool<int>,root_tag),
			      NEW_TEST_OBJ(ListPathPool<int>,root_tag));

  //TEST DATASET
  INSTANTIATE_TEST_CASE_P(PathPools, Iterators, test_objects );
}

#endif /* ITERATOR_SHORT_H */
