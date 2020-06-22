#ifndef GET_TAGLIST_SHORT_H
#define GET_TAGLIST_SHORT_H

#include "utest.h"
#include "path_pool.h"

#include <gtest/gtest.h>

#include <algorithm>

namespace {
  using namespace std::string_literals;
  using namespace std;
  using namespace testing;
  
  template<typename T>
  class GetTagList : public Test
  {
  public:
    using TestObjT = T;
    using tag_t = typename TestObjT::tag_t;
    using path_t = typename TestObjT::pathid_t;
  public:
    void SetUp() override
    {
      root = test_obj.get_root();
      r_t1 = test_obj.get_subnode(root,tag1);
      r_t2 = test_obj.get_subnode(root,tag2);
      r_t1_t1 = test_obj.get_subnode(r_t1,tag1);
      r_t1_t2 = test_obj.get_subnode(r_t1,tag2);
      r_t2_t1 = test_obj.get_subnode(r_t2,tag1);
      r_t2_t2 = test_obj.get_subnode(r_t2,tag2);
      r_t1_t1_t1 = test_obj.get_subnode(r_t1_t1,tag1);
      r_t1_t1_t2 = test_obj.get_subnode(r_t1_t1,tag2);
      r_t1_t2_t1 = test_obj.get_subnode(r_t1_t2,tag1);
      r_t1_t2_t2 = test_obj.get_subnode(r_t1_t2,tag2);
      r_t2_t1_t1 = test_obj.get_subnode(r_t2_t1,tag1);
      r_t2_t1_t2 = test_obj.get_subnode(r_t2_t1,tag2);
      r_t2_t2_t1 = test_obj.get_subnode(r_t2_t2,tag1);
      r_t2_t2_t2 = test_obj.get_subnode(r_t2_t2,tag2);
    }
  public:
    TestObjT test_obj {0};
    tag_t tag1 {1};
    tag_t tag2 {2};
    path_t root;
    path_t r_t1;
    path_t r_t2;
    path_t r_t1_t1;
    path_t r_t1_t2;
    path_t r_t2_t1;
    path_t r_t2_t2;
    path_t r_t1_t1_t1;
    path_t r_t1_t1_t2;
    path_t r_t1_t2_t1;
    path_t r_t1_t2_t2;
    path_t r_t2_t1_t1;
    path_t r_t2_t1_t2;
    path_t r_t2_t2_t1;
    path_t r_t2_t2_t2;
  };

  using types = Types<ListPathPool<int>,HashPathPool<int>>;
  TYPED_TEST_CASE(GetTagList, types);

  template<typename T1,typename T2>
  bool equals(T1 const& cont1, initializer_list<T2> cont2)
  {
    return std::equal(cont1.begin(),cont1.end(),cont2.begin(),cont2.end());
  }

  TYPED_TEST(GetTagList, Root)
  {
    auto expect = {0};
    auto result = get_taglist(this->test_obj, this->root);
    ASSERT_TRUE( equals(result, expect) );
  }
  TYPED_TEST(GetTagList, Children)
  {
    auto result1 = get_taglist(this->test_obj, this->r_t1);
    auto result2 = get_taglist(this->test_obj, this->r_t2);
    ASSERT_TRUE( equals(result1, {1,0}));
    ASSERT_TRUE( equals(result2, {2,0}));
  }
  TYPED_TEST(GetTagList, GrandChildren)
  {
    auto result1 = get_taglist(this->test_obj, this->r_t1_t1_t1);
    auto result2 = get_taglist(this->test_obj, this->r_t1_t1_t2);
    auto result3 = get_taglist(this->test_obj, this->r_t1_t2_t1);
    auto result4 = get_taglist(this->test_obj, this->r_t1_t2_t2);
    auto result5 = get_taglist(this->test_obj, this->r_t2_t1_t1);
    auto result6 = get_taglist(this->test_obj, this->r_t2_t1_t2);
    auto result7 = get_taglist(this->test_obj, this->r_t2_t2_t1);
    auto result8 = get_taglist(this->test_obj, this->r_t2_t2_t2);
    ASSERT_TRUE( equals(result1, {1,1,1,0}));
    ASSERT_TRUE( equals(result2, {2,1,1,0}));
    ASSERT_TRUE( equals(result3, {1,2,1,0}));
    ASSERT_TRUE( equals(result4, {2,2,1,0}));
    ASSERT_TRUE( equals(result5, {1,1,2,0}));
    ASSERT_TRUE( equals(result6, {2,1,2,0}));
    ASSERT_TRUE( equals(result7, {1,2,2,0}));
    ASSERT_TRUE( equals(result8, {2,2,2,0}));
  }
}

#endif /* GET_TAGLIST_SHORT_H */
