#ifndef POPULATED_SHORT_H
#define POPULATED_SHORT_H

#include "utest.h"
#include "path.h"
#include "path_pool.h"
#include "field_ref_iterator.h"

#include <gtest/gtest.h>
#include <algorithm>

using namespace std;
using namespace testing;

using types = Types<Path<ListPathPool<int>>,Path<HashPathPool<int>>>;

namespace {  
  template<typename T>
  class Populated : public Test
  {
  public:
    using TestObjT = T;
    using tag_t = typename TestObjT::tag_t;
  public:
    void SetUp() override
    {
      r_t1 = root / tag1;
      r_t2 = root / tag2;
      r_t1_t1 = r_t1 / tag1;
      r_t1_t2 = r_t1 / tag2;
      r_t2_t1 = r_t2 / tag1;
      r_t2_t2 = r_t2 / tag2;
      r_t1_t1_t1 = r_t1_t1 / tag1;
      r_t1_t1_t2 = r_t1_t1 / tag2;
      r_t1_t2_t1 = r_t1_t2 / tag1;
      r_t1_t2_t2 = r_t1_t2 / tag2;
      r_t2_t1_t1 = r_t2_t1 / tag1;
      r_t2_t1_t2 = r_t2_t1 / tag2;
      r_t2_t2_t1 = r_t2_t2 / tag1;
      r_t2_t2_t2 = r_t2_t2 / tag2;
      // sequence does matter
      paths = { root,
		r_t1,
		r_t1_t1, r_t1_t1_t1, r_t1_t1_t2,
		r_t1_t2, r_t1_t2_t1, r_t1_t2_t2,
		r_t2,
		r_t2_t1, r_t2_t1_t1, r_t2_t1_t2,
		r_t2_t2, r_t2_t2_t1, r_t2_t2_t2
 };
    }
  public:
    tag_t tag1 {1};
    tag_t tag2 {2};
    TestObjT root {};
    TestObjT r_t1;
    TestObjT r_t2;
    TestObjT r_t1_t1;
    TestObjT r_t1_t2;
    TestObjT r_t2_t1;
    TestObjT r_t2_t2;
    TestObjT r_t1_t1_t1;
    TestObjT r_t1_t1_t2;
    TestObjT r_t1_t2_t1;
    TestObjT r_t1_t2_t2;
    TestObjT r_t2_t1_t1;
    TestObjT r_t2_t1_t2;
    TestObjT r_t2_t2_t1;
    TestObjT r_t2_t2_t2;
    std::vector<TestObjT> paths;
  };
  TYPED_TEST_CASE(Populated, types);

  TYPED_TEST(Populated, EqualityOperator)
  {
    auto& paths = this->paths;
    for(size_t i=0;i < paths.size(); ++i)
      for(size_t j=0;j < paths.size(); ++j)
	if(i == j)
	  ASSERT_EQ( paths[i], paths[j] )<<"i = "<<i<<",j = "<<j;
	else
	  ASSERT_FALSE( paths[i] == paths[j] )<<"i = "<<i<<",j = "<<j;
  }
  TYPED_TEST(Populated, LessOperator)
  {
    auto& paths = this->paths;
    for(size_t i=0;i < paths.size(); ++i)
      for(size_t j=0;j < paths.size(); ++j)
	if(i < j)
	  ASSERT_LT( paths[i], paths[j] )<<"i = "<<i<<",j = "<<j;
	else
	  ASSERT_FALSE( paths[i] < paths[j] )<<"i = "<<i<<",j = "<<j;
  }
  TYPED_TEST(Populated, SamePathInstancesEqual)
  {
    auto tag1 = this->tag1;
    auto tag2 = this->tag2;
    ASSERT_EQ(this->r_t1, this->root/tag1 );
    ASSERT_EQ(this->r_t2, this->root/tag2);
    ASSERT_EQ(this->r_t1_t1, this->r_t1/tag1);
    ASSERT_EQ(this->r_t1_t2, this->r_t1/tag2);
    ASSERT_EQ(this->r_t2_t1, this->r_t2/tag1);
    ASSERT_EQ(this->r_t2_t2, this->r_t2/tag2);
    ASSERT_EQ(this->r_t1_t1_t1, this->r_t1_t1/tag1);
    ASSERT_EQ(this->r_t1_t1_t2, this->r_t1_t1/tag2);
    ASSERT_EQ(this->r_t1_t2_t1, this->r_t1_t2/tag1);
    ASSERT_EQ(this->r_t1_t2_t2, this->r_t1_t2/tag2);
    ASSERT_EQ(this->r_t2_t1_t1, this->r_t2_t1/tag1);
    ASSERT_EQ(this->r_t2_t1_t2, this->r_t2_t1/tag2);
    ASSERT_EQ(this->r_t2_t2_t1, this->r_t2_t2/tag1);
    ASSERT_EQ(this->r_t2_t2_t2, this->r_t2_t2/tag2);
  }
  TYPED_TEST(Populated, GetTag)
  {
    ASSERT_EQ(this->r_t1.get_tag(), 1);
    ASSERT_EQ(this->r_t2.get_tag(), 2);
    ASSERT_EQ(this->r_t1_t1.get_tag(), 1);
    ASSERT_EQ(this->r_t1_t2.get_tag(), 2);
    ASSERT_EQ(this->r_t2_t1.get_tag(), 1);
    ASSERT_EQ(this->r_t2_t2.get_tag(), 2);
    ASSERT_EQ(this->r_t1_t1_t1.get_tag(), 1);
    ASSERT_EQ(this->r_t1_t1_t2.get_tag(), 2);
    ASSERT_EQ(this->r_t1_t2_t1.get_tag(), 1);
    ASSERT_EQ(this->r_t1_t2_t2.get_tag(), 2);
    ASSERT_EQ(this->r_t2_t1_t1.get_tag(), 1);
    ASSERT_EQ(this->r_t2_t1_t2.get_tag(), 2);
    ASSERT_EQ(this->r_t2_t2_t1.get_tag(), 1);
    ASSERT_EQ(this->r_t2_t2_t2.get_tag(), 2);
  }
  TYPED_TEST(Populated, GetParent)
  {
    ASSERT_EQ(this->r_t1.get_parent(), this->root);
    ASSERT_EQ(this->r_t2.get_parent(), this->root);
    ASSERT_EQ(this->r_t1_t1.get_parent(), this->r_t1);
    ASSERT_EQ(this->r_t1_t2.get_parent(), this->r_t1);
    ASSERT_EQ(this->r_t2_t1.get_parent(), this->r_t2);
    ASSERT_EQ(this->r_t2_t2.get_parent(), this->r_t2);
    ASSERT_EQ(this->r_t1_t1_t1.get_parent(), this->r_t1_t1);
    ASSERT_EQ(this->r_t1_t1_t2.get_parent(), this->r_t1_t1);
    ASSERT_EQ(this->r_t1_t2_t1.get_parent(), this->r_t1_t2);
    ASSERT_EQ(this->r_t1_t2_t2.get_parent(), this->r_t1_t2);
    ASSERT_EQ(this->r_t2_t1_t1.get_parent(), this->r_t2_t1);
    ASSERT_EQ(this->r_t2_t1_t2.get_parent(), this->r_t2_t1);
    ASSERT_EQ(this->r_t2_t2_t1.get_parent(), this->r_t2_t2);
    ASSERT_EQ(this->r_t2_t2_t2.get_parent(), this->r_t2_t2);
  }
  TYPED_TEST(Populated, FullPathRange)
  {
    auto range_t1 = std::make_pair( this->r_t1.begin(), this->r_t1.end());
    auto range_t2 = std::make_pair( this->r_t2.begin(), this->r_t2.end());
    auto range_t1_t1 = std::make_pair( this->r_t1_t1.begin(), this->r_t1_t1.end());
    auto range_t1_t2 = std::make_pair( this->r_t1_t2.begin(), this->r_t1_t2.end());
    auto range_t2_t1 = std::make_pair( this->r_t2_t1.begin(), this->r_t2_t1.end());
    auto range_t2_t2 = std::make_pair( this->r_t2_t2.begin(), this->r_t2_t2.end());
    auto range_t1_t1_t1 = std::make_pair( this->r_t1_t1_t1.begin(), this->r_t1_t1_t1.end());
    auto range_t1_t1_t2 = std::make_pair( this->r_t1_t1_t2.begin(), this->r_t1_t1_t2.end());
    auto range_t1_t2_t1 = std::make_pair( this->r_t1_t2_t1.begin(), this->r_t1_t2_t1.end());
    auto range_t1_t2_t2 = std::make_pair( this->r_t1_t2_t2.begin(), this->r_t1_t2_t2.end());
    auto range_t2_t1_t1 = std::make_pair( this->r_t2_t1_t1.begin(), this->r_t2_t1_t1.end());
    auto range_t2_t1_t2 = std::make_pair( this->r_t2_t1_t2.begin(), this->r_t2_t1_t2.end());
    auto range_t2_t2_t1 = std::make_pair( this->r_t2_t2_t1.begin(), this->r_t2_t2_t1.end());
    auto range_t2_t2_t2 = std::make_pair( this->r_t2_t2_t2.begin(), this->r_t2_t2_t2.end());

    // ASSERT_EQ( distance(range_t1.first,range_t1.second), 1);
    // ASSERT_EQ( distance(range_t2.first,range_t2.second), 1);
    // ASSERT_EQ( *range_t1.first, 1);
    // ASSERT_EQ( *range_t2.first, 2);
    ASSERT_TRUE( equals(range_t1, {this->r_t1}));
    ASSERT_TRUE( equals(range_t2, {this->r_t2}));
    
    ASSERT_TRUE( equals(range_t1_t1, {this->r_t1_t1, this->r_t1}));
    ASSERT_TRUE( equals(range_t1_t2, {this->r_t1_t2, this->r_t1}));
    ASSERT_TRUE( equals(range_t2_t1, {this->r_t2_t1, this->r_t2}));
    ASSERT_TRUE( equals(range_t2_t2, {this->r_t2_t2, this->r_t2}));
    ASSERT_TRUE( equals(range_t1_t1_t1, {this->r_t1_t1_t1, this->r_t1_t1, this->r_t1}));
    ASSERT_TRUE( equals(range_t1_t1_t2, {this->r_t1_t1_t2, this->r_t1_t1, this->r_t1}));
    ASSERT_TRUE( equals(range_t1_t2_t1, {this->r_t1_t2_t1, this->r_t1_t2, this->r_t1}));
    ASSERT_TRUE( equals(range_t1_t2_t2, {this->r_t1_t2_t2, this->r_t1_t2, this->r_t1}));

    ASSERT_TRUE( equals(range_t2_t1_t1, {this->r_t2_t1_t1, this->r_t2_t1, this->r_t2}));
    ASSERT_TRUE( equals(range_t2_t1_t2, {this->r_t2_t1_t2, this->r_t2_t1, this->r_t2}));
    ASSERT_TRUE( equals(range_t2_t2_t1, {this->r_t2_t2_t1, this->r_t2_t2, this->r_t2}));
    ASSERT_TRUE( equals(range_t2_t2_t2, {this->r_t2_t2_t2, this->r_t2_t2, this->r_t2}));
  }

    TYPED_TEST(Populated, GetSubpaths)
  {
    auto range_root = this->root.get_subpaths();
    auto range_t1 = this->r_t1.get_subpaths();
    auto range_t2 = this->r_t2.get_subpaths();
    auto range_t1_t1 = this->r_t1_t1.get_subpaths();
    auto range_t1_t2 = this->r_t1_t2.get_subpaths();
    auto range_t2_t1 = this->r_t2_t1.get_subpaths();
    auto range_t2_t2 = this->r_t2_t2.get_subpaths();
    auto range_t1_t1_t1 = this->r_t1_t1_t1.get_subpaths();
    auto range_t1_t1_t2 = this->r_t1_t1_t2.get_subpaths();
    auto range_t1_t2_t1 = this->r_t1_t2_t1.get_subpaths();
    auto range_t1_t2_t2 = this->r_t1_t2_t2.get_subpaths();
    auto range_t2_t1_t1 = this->r_t2_t1_t1.get_subpaths();
    auto range_t2_t1_t2 = this->r_t2_t1_t2.get_subpaths();
    auto range_t2_t2_t1 = this->r_t2_t2_t1.get_subpaths();
    auto range_t2_t2_t2 = this->r_t2_t2_t2.get_subpaths();

    ASSERT_TRUE( contains_all(range_root, {this->r_t1, this->r_t2}));
    ASSERT_TRUE( contains_all(range_t1, {this->r_t1_t1, this->r_t1_t2}));
    ASSERT_TRUE( contains_all(range_t2, {this->r_t2_t1, this->r_t2_t2}));
    ASSERT_TRUE( contains_all(range_t1_t1, {this->r_t1_t1_t1, this->r_t1_t1_t2}));
    ASSERT_TRUE( contains_all(range_t1_t2, {this->r_t1_t2_t1, this->r_t1_t2_t2}));
    ASSERT_TRUE( contains_all(range_t2_t1, {this->r_t2_t1_t1, this->r_t2_t1_t2}));
    ASSERT_TRUE( contains_all(range_t2_t2, {this->r_t2_t2_t1, this->r_t2_t2_t2}));
    ASSERT_EQ( range_t1_t1_t1.size(), 0);
    ASSERT_EQ( range_t1_t1_t2.size(), 0);
    ASSERT_EQ( range_t1_t2_t1.size(), 0);
    ASSERT_EQ( range_t1_t2_t2.size(), 0);
    ASSERT_EQ( range_t2_t1_t1.size(), 0);
    ASSERT_EQ( range_t2_t1_t2.size(), 0);
    ASSERT_EQ( range_t2_t2_t1.size(), 0);
    ASSERT_EQ( range_t2_t2_t2.size(), 0);
  }
}

#endif /* POPULATED_SHORT_H */
