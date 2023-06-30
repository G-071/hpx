//  Copyright (c) 2015 Daniel Bourgeois
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hpx/algorithm.hpp>
#include <hpx/init.hpp>
#include <hpx/modules/testing.hpp>

#include <cstddef>
#include <iostream>
#include <iterator>
#include <numeric>
#include <random>
#include <string>
#include <vector>

#include "test_utils.hpp"

////////////////////////////////////////////////////////////////////////////////
int seed = std::random_device{}();
std::mt19937 gen(seed);
std::uniform_int_distribution<> dis(0, 99);

template <typename ExPolicy, typename IteratorTag>
void test_sorted_until1(ExPolicy policy, IteratorTag)
{
    static_assert(hpx::is_execution_policy<ExPolicy>::value,
        "hpx::is_execution_policy<ExPolicy>::value");

    typedef std::vector<int>::iterator base_iterator;
    typedef test::test_iterator<base_iterator, IteratorTag> iterator;

    std::vector<int> c(10007);
    std::iota(std::begin(c), std::end(c), 0);

    iterator until = hpx::ranges::is_sorted_until(
        policy, iterator(std::begin(c)), iterator(std::end(c)));

    base_iterator test_index = std::end(c);

    HPX_TEST(until == iterator(test_index));

    until = hpx::ranges::is_sorted_until(policy, iterator(std::begin(c)),
        iterator(std::end(c)), std::less<int>(),
        [](int x) { return x == 500 ? -x : x; });

    test_index = std::begin(c) + 500;

    HPX_TEST(until == iterator(test_index));
}

template <typename ExPolicy, typename IteratorTag>
void test_sorted_until1_async(ExPolicy p, IteratorTag)
{
    static_assert(hpx::is_execution_policy<ExPolicy>::value,
        "hpx::is_execution_policy<ExPolicy>::value");

    typedef std::vector<int>::iterator base_iterator;
    typedef test::test_iterator<base_iterator, IteratorTag> iterator;

    std::vector<int> c(10007);
    std::iota(std::begin(c), std::end(c), 0);

    hpx::future<iterator> f1 = hpx::ranges::is_sorted_until(
        p, iterator(std::begin(c)), iterator(std::end(c)));

    base_iterator test_index = std::end(c);

    f1.wait();
    HPX_TEST(f1.get() == iterator(test_index));

    hpx::future<iterator> f2 = hpx::ranges::is_sorted_until(p,
        iterator(std::begin(c)), iterator(std::end(c)), std::less<int>(),
        [](int x) { return x == 500 ? -x : x; });

    f2.wait();
    test_index = std::begin(c) + 500;
    HPX_TEST(f2.get() == iterator(test_index));
}

template <typename IteratorTag>
void test_sorted_until1_seq(IteratorTag)
{
    typedef std::vector<int>::iterator base_iterator;
    typedef test::test_iterator<base_iterator, IteratorTag> iterator;

    std::vector<int> c(10007);
    std::iota(std::begin(c), std::end(c), 0);

    iterator until = hpx::ranges::is_sorted_until(
        iterator(std::begin(c)), iterator(std::end(c)));

    base_iterator test_index = std::end(c);

    HPX_TEST(until == iterator(test_index));

    until = hpx::ranges::is_sorted_until(iterator(std::begin(c)),
        iterator(std::end(c)), std::less<int>(),
        [](int x) { return x == 500 ? -x : x; });

    test_index = std::begin(c) + 500;

    HPX_TEST(until == iterator(test_index));
}

template <typename ExPolicy>
void test_sorted_until1(ExPolicy policy)
{
    static_assert(hpx::is_execution_policy<ExPolicy>::value,
        "hpx::is_execution_policy<ExPolicy>::value");

    std::vector<int> c(10007);
    std::iota(std::begin(c), std::end(c), 0);

    auto until = hpx::ranges::is_sorted_until(policy, c);

    auto test_index = std::end(c);

    HPX_TEST(until == test_index);

    until = hpx::ranges::is_sorted_until(
        policy, c, std::less<int>(), [](int x) { return x == 500 ? -x : x; });

    test_index = std::begin(c) + 500;

    HPX_TEST(until == test_index);
}

template <typename ExPolicy>
void test_sorted_until1_async(ExPolicy p)
{
    static_assert(hpx::is_execution_policy<ExPolicy>::value,
        "hpx::is_execution_policy<ExPolicy>::value");

    std::vector<int> c(10007);
    std::iota(std::begin(c), std::end(c), 0);

    auto f1 = hpx::ranges::is_sorted_until(p, c);

    auto test_index = std::end(c);

    f1.wait();
    HPX_TEST(f1.get() == test_index);

    auto f2 = hpx::ranges::is_sorted_until(
        p, c, std::less<int>(), [](int x) { return x == 500 ? -x : x; });

    f2.wait();
    test_index = std::begin(c) + 500;
    HPX_TEST(f2.get() == test_index);
}

void test_sorted_until1_seq()
{
    std::vector<int> c(10007);
    std::iota(std::begin(c), std::end(c), 0);

    auto until = hpx::ranges::is_sorted_until(c);

    auto test_index = std::end(c);

    HPX_TEST(until == test_index);

    until = hpx::ranges::is_sorted_until(
        c, std::less<int>(), [](int x) { return x == 500 ? -x : x; });

    test_index = std::begin(c) + 500;

    HPX_TEST(until == test_index);
}

template <typename IteratorTag>
void test_sorted_until1()
{
    using namespace hpx::execution;

    test_sorted_until1(seq, IteratorTag());
    test_sorted_until1(par, IteratorTag());
    test_sorted_until1(par_unseq, IteratorTag());

    test_sorted_until1_async(seq(task), IteratorTag());
    test_sorted_until1_async(par(task), IteratorTag());

    test_sorted_until1_seq(IteratorTag());
}

void sorted_until_test1()
{
    test_sorted_until1<std::random_access_iterator_tag>();
    test_sorted_until1<std::forward_iterator_tag>();

    using namespace hpx::execution;

    test_sorted_until1(seq);
    test_sorted_until1(par);
    test_sorted_until1(par_unseq);

    test_sorted_until1_async(seq(task));
    test_sorted_until1_async(par(task));

    test_sorted_until1_seq();
}

////////////////////////////////////////////////////////////////////////////////
template <typename ExPolicy, typename IteratorTag>
void test_sorted_until2(ExPolicy policy, IteratorTag)
{
    static_assert(hpx::is_execution_policy<ExPolicy>::value,
        "hpx::is_execution_policy<ExPolicy>::value");

    typedef std::vector<int>::iterator base_iterator;
    typedef test::test_iterator<base_iterator, IteratorTag> iterator;

    std::vector<int> c(10007);
    //Fill with sorted values from 0 to 10006
    std::iota(std::begin(c), std::end(c), 0);
    //Add a certain large value in middle of array to ignore
    int ignore = 20000;
    c[c.size() / 2] = ignore;
    //Provide custom predicate to ignore the value of ignore
    //pred should return true when it is given something deemed not sorted
    auto pred = [&ignore](int ahead, int behind) {
        return behind > ahead && behind != ignore;
    };

    iterator until = hpx::ranges::is_sorted_until(
        policy, iterator(std::begin(c)), iterator(std::end(c)), pred);

    base_iterator test_index = std::end(c);

    HPX_TEST(until == iterator(test_index));

    until = hpx::ranges::is_sorted_until(policy, iterator(std::begin(c)),
        iterator(std::end(c)), std::less<int>(), [&](int x) {
            return x == ignore ? static_cast<int>(c.size()) / 2 : x;
        });

    test_index = std::end(c);

    HPX_TEST(until == iterator(test_index));
}

template <typename ExPolicy, typename IteratorTag>
void test_sorted_until2_async(ExPolicy p, IteratorTag)
{
    static_assert(hpx::is_execution_policy<ExPolicy>::value,
        "hpx::is_execution_policy<ExPolicy>::value");

    typedef std::vector<int>::iterator base_iterator;
    typedef test::test_iterator<base_iterator, IteratorTag> iterator;

    std::vector<int> c(10007);
    //Fill with sorted values from 0 to 10006
    std::iota(std::begin(c), std::end(c), 0);
    //Add a certain large value in middle of array to ignore
    int ignore = 20000;
    c[c.size() / 2] = ignore;
    //Provide custom predicate to ignore the value of ignore
    //pred should return true when it is given something deemed not sorted
    auto pred = [&ignore](int ahead, int behind) {
        return behind > ahead && behind != ignore;
    };

    hpx::future<iterator> f1 = hpx::ranges::is_sorted_until(
        p, iterator(std::begin(c)), iterator(std::end(c)), pred);

    base_iterator test_index = std::end(c);
    f1.wait();
    HPX_TEST(f1.get() == iterator(test_index));

    hpx::future<iterator> f2 =
        hpx::ranges::is_sorted_until(p, iterator(std::begin(c)),
            iterator(std::end(c)), std::less<int>(), [&](int x) {
                return x == ignore ? static_cast<int>(c.size()) / 2 : x;
            });

    f2.wait();
    test_index = std::end(c);
    HPX_TEST(f2.get() == iterator(test_index));
}

template <typename IteratorTag>
void test_sorted_until2_seq(IteratorTag)
{
    typedef std::vector<int>::iterator base_iterator;
    typedef test::test_iterator<base_iterator, IteratorTag> iterator;

    std::vector<int> c(10007);
    //Fill with sorted values from 0 to 10006
    std::iota(std::begin(c), std::end(c), 0);
    //Add a certain large value in middle of array to ignore
    int ignore = 20000;
    c[c.size() / 2] = ignore;
    //Provide custom predicate to ignore the value of ignore
    //pred should return true when it is given something deemed not sorted
    auto pred = [&ignore](int ahead, int behind) {
        return behind > ahead && behind != ignore;
    };

    iterator until = hpx::ranges::is_sorted_until(
        iterator(std::begin(c)), iterator(std::end(c)), pred);

    base_iterator test_index = std::end(c);

    HPX_TEST(until == iterator(test_index));

    until = hpx::ranges::is_sorted_until(iterator(std::begin(c)),
        iterator(std::end(c)), std::less<int>(), [&](int x) {
            return x == ignore ? static_cast<int>(c.size()) / 2 : x;
        });

    test_index = std::end(c);

    HPX_TEST(until == iterator(test_index));
}

template <typename ExPolicy>
void test_sorted_until2(ExPolicy policy)
{
    static_assert(hpx::is_execution_policy<ExPolicy>::value,
        "hpx::is_execution_policy<ExPolicy>::value");

    std::vector<int> c(10007);
    //Fill with sorted values from 0 to 10006
    std::iota(std::begin(c), std::end(c), 0);
    //Add a certain large value in middle of array to ignore
    int ignore = 20000;
    c[c.size() / 2] = ignore;
    //Provide custom predicate to ignore the value of ignore
    //pred should return true when it is given something deemed not sorted
    auto pred = [&ignore](int ahead, int behind) {
        return behind > ahead && behind != ignore;
    };

    auto until = hpx::ranges::is_sorted_until(policy, c, pred);

    auto test_index = std::end(c);

    HPX_TEST(until == test_index);

    until =
        hpx::ranges::is_sorted_until(policy, c, std::less<int>(), [&](int x) {
            return x == ignore ? static_cast<int>(c.size()) / 2 : x;
        });

    test_index = std::end(c);

    HPX_TEST(until == test_index);
}

template <typename ExPolicy>
void test_sorted_until2_async(ExPolicy p)
{
    static_assert(hpx::is_execution_policy<ExPolicy>::value,
        "hpx::is_execution_policy<ExPolicy>::value");

    std::vector<int> c(10007);
    //Fill with sorted values from 0 to 10006
    std::iota(std::begin(c), std::end(c), 0);
    //Add a certain large value in middle of array to ignore
    int ignore = 20000;
    c[c.size() / 2] = ignore;
    //Provide custom predicate to ignore the value of ignore
    //pred should return true when it is given something deemed not sorted
    auto pred = [&ignore](int ahead, int behind) {
        return behind > ahead && behind != ignore;
    };

    auto f1 = hpx::ranges::is_sorted_until(p, c, pred);

    auto test_index = std::end(c);
    f1.wait();
    HPX_TEST(f1.get() == test_index);

    auto f2 = hpx::ranges::is_sorted_until(p, c, std::less<int>(), [&](int x) {
        return x == ignore ? static_cast<int>(c.size()) / 2 : x;
    });

    f2.wait();
    test_index = std::end(c);
    HPX_TEST(f2.get() == test_index);
}

void test_sorted_until2_seq()
{
    std::vector<int> c(10007);
    //Fill with sorted values from 0 to 10006
    std::iota(std::begin(c), std::end(c), 0);
    //Add a certain large value in middle of array to ignore
    int ignore = 20000;
    c[c.size() / 2] = ignore;
    //Provide custom predicate to ignore the value of ignore
    //pred should return true when it is given something deemed not sorted
    auto pred = [&ignore](int ahead, int behind) {
        return behind > ahead && behind != ignore;
    };

    auto until = hpx::ranges::is_sorted_until(c, pred);

    auto test_index = std::end(c);

    HPX_TEST(until == test_index);

    until = hpx::ranges::is_sorted_until(c, std::less<int>(), [&](int x) {
        return x == ignore ? static_cast<int>(c.size()) / 2 : x;
    });

    test_index = std::end(c);

    HPX_TEST(until == test_index);
}

template <typename IteratorTag>
void test_sorted_until2()
{
    using namespace hpx::execution;
    test_sorted_until2(seq, IteratorTag());
    test_sorted_until2(par, IteratorTag());
    test_sorted_until2(par_unseq, IteratorTag());

    test_sorted_until2_async(seq(task), IteratorTag());
    test_sorted_until2_async(par(task), IteratorTag());

    test_sorted_until2_seq(IteratorTag());
}

void sorted_until_test2()
{
    test_sorted_until2<std::random_access_iterator_tag>();
    test_sorted_until2<std::forward_iterator_tag>();

    using namespace hpx::execution;

    test_sorted_until2(seq);
    test_sorted_until2(par);
    test_sorted_until2(par_unseq);

    test_sorted_until2_async(seq(task));
    test_sorted_until2_async(par(task));

    test_sorted_until2_seq();
}

////////////////////////////////////////////////////////////////////////////////
template <typename ExPolicy, typename IteratorTag>
void test_sorted_until3(ExPolicy policy, IteratorTag)
{
    static_assert(hpx::is_execution_policy<ExPolicy>::value,
        "hpx::is_execution_policy<ExPolicy>::value");

    typedef std::vector<int>::iterator base_iterator;
    typedef test::test_iterator<base_iterator, IteratorTag> iterator;
    //test the following:
    // put unsorted elements at each ends
    // put two unsorted elements in the middle

    std::vector<int> c1(10007);
    std::vector<int> c2(10007);
    std::iota(std::begin(c1), std::end(c1), 0);
    std::iota(std::begin(c2), std::end(c2), 0);

    iterator until1 =
        hpx::ranges::is_sorted_until(policy, iterator(std::begin(c1)),
            iterator(std::end(c1)), std::less<int>(), [&](int x) {
                if (x == 0)
                {
                    return 20000;
                }
                else if (x == static_cast<int>(c1.size()) - 1)
                {
                    return 0;
                }
                else
                {
                    return x;
                }
            });
    iterator until2 =
        hpx::ranges::is_sorted_until(policy, iterator(std::begin(c2)),
            iterator(std::end(c2)), std::less<int>(), [&](int x) {
                if (x == static_cast<int>(c2.size()) / 3 ||
                    x == 2 * static_cast<int>(c2.size()) / 3)
                {
                    return 0;
                }
                else
                {
                    return x;
                }
            });

    base_iterator test_index1 = std::begin(c1) + 1;
    base_iterator test_index2 = std::begin(c2) + c2.size() / 3;

    HPX_TEST(until1 == iterator(test_index1));
    HPX_TEST(until2 == iterator(test_index2));
}

template <typename ExPolicy, typename IteratorTag>
void test_sorted_until3_async(ExPolicy p, IteratorTag)
{
    static_assert(hpx::is_execution_policy<ExPolicy>::value,
        "hpx::is_execution_policy<ExPolicy>::value");

    typedef std::vector<int>::iterator base_iterator;
    typedef test::test_iterator<base_iterator, IteratorTag> iterator;
    //test the following:
    // put unsorted elements at each ends
    // put two unsorted elements in the middle

    std::vector<int> c1(10007);
    std::vector<int> c2(10007);
    std::iota(std::begin(c1), std::end(c1), 0);
    std::iota(std::begin(c2), std::end(c2), 0);

    hpx::future<iterator> f1 =
        hpx::ranges::is_sorted_until(p, iterator(std::begin(c1)),
            iterator(std::end(c1)), std::less<int>(), [&](int x) {
                if (x == 0)
                {
                    return 20000;
                }
                else if (x == static_cast<int>(c1.size()) - 1)
                {
                    return 0;
                }
                else
                {
                    return x;
                }
            });
    hpx::future<iterator> f2 =
        hpx::ranges::is_sorted_until(p, iterator(std::begin(c2)),
            iterator(std::end(c2)), std::less<int>(), [&](int x) {
                if (x == static_cast<int>(c2.size()) / 3 ||
                    x == 2 * static_cast<int>(c2.size()) / 3)
                {
                    return 0;
                }
                else
                {
                    return x;
                }
            });

    base_iterator test_index1 = std::begin(c1) + 1;
    base_iterator test_index2 = std::begin(c2) + c2.size() / 3;

    f1.wait();
    HPX_TEST(f1.get() == iterator(test_index1));
    f2.wait();
    HPX_TEST(f2.get() == iterator(test_index2));
}

template <typename IteratorTag>
void test_sorted_until3_seq(IteratorTag)
{
    typedef std::vector<int>::iterator base_iterator;
    typedef test::test_iterator<base_iterator, IteratorTag> iterator;
    //test the following:
    // put unsorted elements at each ends
    // put two unsorted elements in the middle

    std::vector<int> c1(10007);
    std::vector<int> c2(10007);
    std::iota(std::begin(c1), std::end(c1), 0);
    std::iota(std::begin(c2), std::end(c2), 0);

    iterator until1 = hpx::ranges::is_sorted_until(iterator(std::begin(c1)),
        iterator(std::end(c1)), std::less<int>(), [&](int x) {
            if (x == 0)
            {
                return 20000;
            }
            else if (x == static_cast<int>(c1.size()) - 1)
            {
                return 0;
            }
            else
            {
                return x;
            }
        });
    iterator until2 = hpx::ranges::is_sorted_until(iterator(std::begin(c2)),
        iterator(std::end(c2)), std::less<int>(), [&](int x) {
            if (x == static_cast<int>(c2.size()) / 3 ||
                x == 2 * static_cast<int>(c2.size()) / 3)
            {
                return 0;
            }
            else
            {
                return x;
            }
        });

    base_iterator test_index1 = std::begin(c1) + 1;
    base_iterator test_index2 = std::begin(c2) + c2.size() / 3;

    HPX_TEST(until1 == iterator(test_index1));
    HPX_TEST(until2 == iterator(test_index2));
}

template <typename ExPolicy>
void test_sorted_until3(ExPolicy policy)
{
    static_assert(hpx::is_execution_policy<ExPolicy>::value,
        "hpx::is_execution_policy<ExPolicy>::value");

    //test the following:
    // put unsorted elements at each ends
    // put two unsorted elements in the middle

    std::vector<int> c1(10007);
    std::vector<int> c2(10007);
    std::iota(std::begin(c1), std::end(c1), 0);
    std::iota(std::begin(c2), std::end(c2), 0);

    auto until1 =
        hpx::ranges::is_sorted_until(policy, c1, std::less<int>(), [&](int x) {
            if (x == 0)
            {
                return 20000;
            }
            else if (x == static_cast<int>(c1.size()) - 1)
            {
                return 0;
            }
            else
            {
                return x;
            }
        });
    auto until2 =
        hpx::ranges::is_sorted_until(policy, c2, std::less<int>(), [&](int x) {
            if (x == static_cast<int>(c2.size()) / 3 ||
                x == 2 * static_cast<int>(c2.size()) / 3)
            {
                return 0;
            }
            else
            {
                return x;
            }
        });

    auto test_index1 = std::begin(c1) + 1;
    auto test_index2 = std::begin(c2) + c2.size() / 3;

    HPX_TEST(until1 == test_index1);
    HPX_TEST(until2 == test_index2);
}

template <typename ExPolicy>
void test_sorted_until3_async(ExPolicy p)
{
    static_assert(hpx::is_execution_policy<ExPolicy>::value,
        "hpx::is_execution_policy<ExPolicy>::value");

    //test the following:
    // put unsorted elements at each ends
    // put two unsorted elements in the middle

    std::vector<int> c1(10007);
    std::vector<int> c2(10007);
    std::iota(std::begin(c1), std::end(c1), 0);
    std::iota(std::begin(c2), std::end(c2), 0);

    auto f1 = hpx::ranges::is_sorted_until(p, c1, std::less<int>(), [&](int x) {
        if (x == 0)
        {
            return 20000;
        }
        else if (x == static_cast<int>(c1.size()) - 1)
        {
            return 0;
        }
        else
        {
            return x;
        }
    });
    auto f2 = hpx::ranges::is_sorted_until(p, c2, std::less<int>(), [&](int x) {
        if (x == static_cast<int>(c2.size()) / 3 ||
            x == 2 * static_cast<int>(c2.size()) / 3)
        {
            return 0;
        }
        else
        {
            return x;
        }
    });

    auto test_index1 = std::begin(c1) + 1;
    auto test_index2 = std::begin(c2) + c2.size() / 3;

    f1.wait();
    HPX_TEST(f1.get() == test_index1);
    f2.wait();
    HPX_TEST(f2.get() == test_index2);
}

void test_sorted_until3_seq()
{
    //test the following:
    // put unsorted elements at each ends
    // put two unsorted elements in the middle

    std::vector<int> c1(10007);
    std::vector<int> c2(10007);
    std::iota(std::begin(c1), std::end(c1), 0);
    std::iota(std::begin(c2), std::end(c2), 0);

    auto until1 =
        hpx::ranges::is_sorted_until(c1, std::less<int>(), [&](int x) {
            if (x == 0)
            {
                return 20000;
            }
            else if (x == static_cast<int>(c1.size()) - 1)
            {
                return 0;
            }
            else
            {
                return x;
            }
        });
    auto until2 =
        hpx::ranges::is_sorted_until(c2, std::less<int>(), [&](int x) {
            if (x == static_cast<int>(c2.size()) / 3 ||
                x == 2 * static_cast<int>(c2.size()) / 3)
            {
                return 0;
            }
            else
            {
                return x;
            }
        });

    auto test_index1 = std::begin(c1) + 1;
    auto test_index2 = std::begin(c2) + c2.size() / 3;

    HPX_TEST(until1 == test_index1);
    HPX_TEST(until2 == test_index2);
}

template <typename IteratorTag>
void test_sorted_until3()
{
    using namespace hpx::execution;
    test_sorted_until3(seq, IteratorTag());
    test_sorted_until3(par, IteratorTag());
    test_sorted_until3(par_unseq, IteratorTag());

    test_sorted_until3_async(seq(task), IteratorTag());
    test_sorted_until3_async(par(task), IteratorTag());

    test_sorted_until3_seq(IteratorTag());
}

void sorted_until_test3()
{
    test_sorted_until3<std::random_access_iterator_tag>();
    test_sorted_until3<std::forward_iterator_tag>();

    using namespace hpx::execution;

    test_sorted_until3(seq);
    test_sorted_until3(par);
    test_sorted_until3(par_unseq);

    test_sorted_until3_async(seq(task));
    test_sorted_until3_async(par(task));

    test_sorted_until3_seq();
}

////////////////////////////////////////////////////////////////////////////////
template <typename ExPolicy, typename IteratorTag>
void test_sorted_until_exception(ExPolicy policy, IteratorTag)
{
    static_assert(hpx::is_execution_policy<ExPolicy>::value,
        "hpx::is_execution_policy<ExPolicy>::value");

    typedef std::vector<int>::iterator base_iterator;
    typedef test::test_iterator<base_iterator, IteratorTag> iterator;
    typedef test::decorated_iterator<base_iterator, IteratorTag>
        decorated_iterator;

    std::vector<int> c(10007);
    //fill first half of array with even numbers and second half
    //with odd numbers
    std::iota(std::begin(c), std::end(c), 0);

    bool caught_exception = false;
    try
    {
        hpx::ranges::is_sorted_until(policy,
            decorated_iterator(
                std::begin(c), []() { throw std::runtime_error("test"); }),
            decorated_iterator(
                std::end(c), []() { throw std::runtime_error("test"); }));
    }
    catch (hpx::exception_list const& e)
    {
        caught_exception = true;
        test::test_num_exceptions<ExPolicy, IteratorTag>::call(policy, e);
    }
    catch (...)
    {
        HPX_TEST(false);
    }

    HPX_TEST(caught_exception);

    caught_exception = false;
    try
    {
        hpx::ranges::is_sorted_until(policy, iterator(std::begin(c)),
            iterator(std::end(c)),
            [](int, int) -> bool { throw std::runtime_error("test"); });
    }
    catch (hpx::exception_list const& e)
    {
        caught_exception = true;
        test::test_num_exceptions<ExPolicy, IteratorTag>::call(policy, e);
    }
    catch (...)
    {
        HPX_TEST(false);
    }

    HPX_TEST(caught_exception);

    caught_exception = false;
    try
    {
        hpx::ranges::is_sorted_until(policy, iterator(std::begin(c)),
            iterator(std::end(c)), std::less<int>(),
            [](int) -> int { throw std::runtime_error("test"); });
    }
    catch (hpx::exception_list const& e)
    {
        caught_exception = true;
        test::test_num_exceptions<ExPolicy, IteratorTag>::call(policy, e);
    }
    catch (...)
    {
        HPX_TEST(false);
    }

    HPX_TEST(caught_exception);
}

template <typename ExPolicy, typename IteratorTag>
void test_sorted_until_async_exception(ExPolicy p, IteratorTag)
{
    typedef std::vector<int>::iterator base_iterator;
    typedef test::test_iterator<base_iterator, IteratorTag> iterator;
    typedef test::decorated_iterator<base_iterator, IteratorTag>
        decorated_iterator;

    std::vector<int> c(10007);
    std::iota(std::begin(c), std::end(c), 0);

    bool caught_exception = false;
    try
    {
        hpx::future<decorated_iterator> f = hpx::ranges::is_sorted_until(p,
            decorated_iterator(
                std::begin(c), []() { throw std::runtime_error("test"); }),
            decorated_iterator(
                std::end(c), []() { throw std::runtime_error("test"); }));
        f.get();

        HPX_TEST(false);
    }
    catch (hpx::exception_list const& e)
    {
        caught_exception = true;
        test::test_num_exceptions<ExPolicy, IteratorTag>::call(p, e);
    }
    catch (...)
    {
        HPX_TEST(false);
    }

    HPX_TEST(caught_exception);

    caught_exception = false;
    try
    {
        hpx::future<iterator> f = hpx::ranges::is_sorted_until(p,
            iterator(std::begin(c)), iterator(std::end(c)),
            [](int, int) -> int { throw std::runtime_error("test"); });
        f.get();

        HPX_TEST(false);
    }
    catch (hpx::exception_list const& e)
    {
        caught_exception = true;
        test::test_num_exceptions<ExPolicy, IteratorTag>::call(p, e);
    }
    catch (...)
    {
        HPX_TEST(false);
    }

    HPX_TEST(caught_exception);

    caught_exception = false;
    try
    {
        hpx::future<iterator> f = hpx::ranges::is_sorted_until(p,
            iterator(std::begin(c)), iterator(std::end(c)), std::less<int>(),
            [](int) -> bool { throw std::runtime_error("test"); });
        f.get();

        HPX_TEST(false);
    }
    catch (hpx::exception_list const& e)
    {
        caught_exception = true;
        test::test_num_exceptions<ExPolicy, IteratorTag>::call(p, e);
    }
    catch (...)
    {
        HPX_TEST(false);
    }

    HPX_TEST(caught_exception);
}

template <typename IteratorTag>
void test_sorted_until_seq_exception(IteratorTag)
{
    typedef std::vector<int>::iterator base_iterator;
    typedef test::test_iterator<base_iterator, IteratorTag> iterator;
    typedef test::decorated_iterator<base_iterator, IteratorTag>
        decorated_iterator;

    std::vector<int> c(10007);
    //fill first half of array with even numbers and second half
    //with odd numbers
    std::iota(std::begin(c), std::end(c), 0);

    bool caught_exception = false;
    try
    {
        hpx::ranges::is_sorted_until(
            decorated_iterator(
                std::begin(c), []() { throw std::runtime_error("test"); }),
            decorated_iterator(
                std::end(c), []() { throw std::runtime_error("test"); }));
    }
    catch (hpx::exception_list const& e)
    {
        caught_exception = true;
        test::test_num_exceptions<hpx::execution::sequenced_policy,
            IteratorTag>::call(hpx::execution::seq, e);
    }
    catch (...)
    {
        HPX_TEST(false);
    }

    HPX_TEST(caught_exception);

    caught_exception = false;
    try
    {
        hpx::ranges::is_sorted_until(iterator(std::begin(c)),
            iterator(std::end(c)),
            [](int, int) -> int { throw std::runtime_error("test"); });
    }
    catch (hpx::exception_list const& e)
    {
        caught_exception = true;
        test::test_num_exceptions<hpx::execution::sequenced_policy,
            IteratorTag>::call(hpx::execution::seq, e);
    }
    catch (...)
    {
        HPX_TEST(false);
    }

    HPX_TEST(caught_exception);

    caught_exception = false;
    try
    {
        hpx::ranges::is_sorted_until(iterator(std::begin(c)),
            iterator(std::end(c)), std::less<int>(),
            [](int) -> int { throw std::runtime_error("test"); });
    }
    catch (hpx::exception_list const& e)
    {
        caught_exception = true;
        test::test_num_exceptions<hpx::execution::sequenced_policy,
            IteratorTag>::call(hpx::execution::seq, e);
    }
    catch (...)
    {
        HPX_TEST(false);
    }

    HPX_TEST(caught_exception);
}

template <typename ExPolicy>
void test_sorted_until_exception(ExPolicy policy)
{
    static_assert(hpx::is_execution_policy<ExPolicy>::value,
        "hpx::is_execution_policy<ExPolicy>::value");

    std::vector<int> c(10007);
    //fill first half of array with even numbers and second half
    //with odd numbers
    std::iota(std::begin(c), std::end(c), 0);

    bool caught_exception = false;
    try
    {
        hpx::ranges::is_sorted_until(policy, c,
            [](int, int) -> bool { throw std::runtime_error("test"); });
    }
    catch (hpx::exception_list const& e)
    {
        caught_exception = true;
        test::test_num_exceptions_base<ExPolicy>::call(policy, e);
    }
    catch (...)
    {
        HPX_TEST(false);
    }

    HPX_TEST(caught_exception);

    caught_exception = false;
    try
    {
        hpx::ranges::is_sorted_until(policy, c, std::less<int>(),
            [](int) -> int { throw std::runtime_error("test"); });
    }
    catch (hpx::exception_list const& e)
    {
        caught_exception = true;
        test::test_num_exceptions_base<ExPolicy>::call(policy, e);
    }
    catch (...)
    {
        HPX_TEST(false);
    }

    HPX_TEST(caught_exception);
}

template <typename ExPolicy>
void test_sorted_until_async_exception(ExPolicy p)
{
    std::vector<int> c(10007);
    std::iota(std::begin(c), std::end(c), 0);

    bool caught_exception = false;
    try
    {
        auto f = hpx::ranges::is_sorted_until(
            p, c, [](int, int) -> int { throw std::runtime_error("test"); });
        f.get();

        HPX_TEST(false);
    }
    catch (hpx::exception_list const& e)
    {
        caught_exception = true;
        test::test_num_exceptions_base<ExPolicy>::call(p, e);
    }
    catch (...)
    {
        HPX_TEST(false);
    }

    HPX_TEST(caught_exception);

    caught_exception = false;
    try
    {
        auto f = hpx::ranges::is_sorted_until(p, c, std::less<int>(),
            [](int) -> bool { throw std::runtime_error("test"); });
        f.get();

        HPX_TEST(false);
    }
    catch (hpx::exception_list const& e)
    {
        caught_exception = true;
        test::test_num_exceptions_base<ExPolicy>::call(p, e);
    }
    catch (...)
    {
        HPX_TEST(false);
    }

    HPX_TEST(caught_exception);
}

void test_sorted_until_seq_exception()
{
    std::vector<int> c(10007);
    //fill first half of array with even numbers and second half
    //with odd numbers
    std::iota(std::begin(c), std::end(c), 0);

    bool caught_exception = false;
    try
    {
        hpx::ranges::is_sorted_until(
            c, [](int, int) -> int { throw std::runtime_error("test"); });
    }
    catch (hpx::exception_list const& e)
    {
        caught_exception = true;
        test::test_num_exceptions_base<hpx::execution::sequenced_policy>::call(
            hpx::execution::seq, e);
    }
    catch (...)
    {
        HPX_TEST(false);
    }

    HPX_TEST(caught_exception);

    caught_exception = false;
    try
    {
        hpx::ranges::is_sorted_until(c, std::less<int>(),
            [](int) -> int { throw std::runtime_error("test"); });
    }
    catch (hpx::exception_list const& e)
    {
        caught_exception = true;
        test::test_num_exceptions_base<hpx::execution::sequenced_policy>::call(
            hpx::execution::seq, e);
    }
    catch (...)
    {
        HPX_TEST(false);
    }

    HPX_TEST(caught_exception);
}

template <typename IteratorTag>
void test_sorted_until_exception()
{
    using namespace hpx::execution;
    //If the execution policy object is of type vector_execution_policy,
    //  std::terminate shall be called. Therefore we do not test exceptions
    //  with a vector execution policy
    test_sorted_until_exception(seq, IteratorTag());
    test_sorted_until_exception(par, IteratorTag());

    test_sorted_until_async_exception(seq(task), IteratorTag());
    test_sorted_until_async_exception(par(task), IteratorTag());

    test_sorted_until_seq_exception(IteratorTag());
}

void sorted_until_exception_test()
{
    test_sorted_until_exception<std::random_access_iterator_tag>();
    test_sorted_until_exception<std::forward_iterator_tag>();

    using namespace hpx::execution;

    test_sorted_until_exception(seq);
    test_sorted_until_exception(par);

    test_sorted_until_async_exception(seq(task));
    test_sorted_until_async_exception(par(task));

    test_sorted_until_seq_exception();
}

////////////////////////////////////////////////////////////////////////////////
template <typename ExPolicy, typename IteratorTag>
void test_sorted_until_bad_alloc(ExPolicy policy, IteratorTag)
{
    static_assert(hpx::is_execution_policy<ExPolicy>::value,
        "hpx::is_execution_policy<ExPolicy>::value");

    typedef std::vector<int>::iterator base_iterator;
    typedef test::test_iterator<base_iterator, IteratorTag> iterator;
    typedef test::decorated_iterator<base_iterator, IteratorTag>
        decorated_iterator;

    std::vector<int> c(10007);
    //fill first half of array with even numbers and second half
    //with odd numbers
    std::iota(std::begin(c), std::end(c), 0);

    bool caught_bad_alloc = false;
    try
    {
        hpx::ranges::is_sorted_until(policy,
            decorated_iterator(
                std::begin(c), []() { throw std::runtime_error("test"); }),
            decorated_iterator(
                std::end(c), []() { throw std::runtime_error("test"); }));
    }
    catch (hpx::exception_list const& e)
    {
        caught_bad_alloc = true;
    }
    catch (...)
    {
        HPX_TEST(false);
    }

    HPX_TEST(caught_bad_alloc);

    caught_bad_alloc = false;
    try
    {
        hpx::ranges::is_sorted_until(policy, iterator(std::begin(c)),
            iterator(std::end(c)),
            [](int, int) -> bool { throw std::runtime_error("test"); });
    }
    catch (hpx::exception_list const& e)
    {
        caught_bad_alloc = true;
    }
    catch (...)
    {
        HPX_TEST(false);
    }

    HPX_TEST(caught_bad_alloc);

    caught_bad_alloc = false;
    try
    {
        hpx::ranges::is_sorted_until(policy, iterator(std::begin(c)),
            iterator(std::end(c)), std::less<int>(),
            [](int) -> int { throw std::runtime_error("test"); });
    }
    catch (hpx::exception_list const& e)
    {
        caught_bad_alloc = true;
    }
    catch (...)
    {
        HPX_TEST(false);
    }

    HPX_TEST(caught_bad_alloc);
}

template <typename ExPolicy, typename IteratorTag>
void test_sorted_until_async_bad_alloc(ExPolicy p, IteratorTag)
{
    typedef std::vector<int>::iterator base_iterator;
    typedef test::test_iterator<base_iterator, IteratorTag> iterator;
    typedef test::decorated_iterator<base_iterator, IteratorTag>
        decorated_iterator;

    std::vector<int> c(10007);
    std::iota(std::begin(c), std::end(c), 0);

    bool caught_bad_alloc = false;
    try
    {
        hpx::future<decorated_iterator> f = hpx::ranges::is_sorted_until(p,
            decorated_iterator(
                std::begin(c), []() { throw std::runtime_error("test"); }),
            decorated_iterator(
                std::end(c), []() { throw std::runtime_error("test"); }));
        f.get();

        HPX_TEST(false);
    }
    catch (hpx::exception_list const& e)
    {
        caught_bad_alloc = true;
    }
    catch (...)
    {
        HPX_TEST(false);
    }

    HPX_TEST(caught_bad_alloc);

    caught_bad_alloc = false;
    try
    {
        hpx::future<iterator> f = hpx::ranges::is_sorted_until(p,
            iterator(std::begin(c)), iterator(std::end(c)),
            [](int, int) -> int { throw std::runtime_error("test"); });
        f.get();

        HPX_TEST(false);
    }
    catch (hpx::exception_list const& e)
    {
        caught_bad_alloc = true;
    }
    catch (...)
    {
        HPX_TEST(false);
    }

    HPX_TEST(caught_bad_alloc);

    caught_bad_alloc = false;
    try
    {
        hpx::future<iterator> f = hpx::ranges::is_sorted_until(p,
            iterator(std::begin(c)), iterator(std::end(c)), std::less<int>(),
            [](int) -> bool { throw std::runtime_error("test"); });
        f.get();

        HPX_TEST(false);
    }
    catch (hpx::exception_list const& e)
    {
        caught_bad_alloc = true;
    }
    catch (...)
    {
        HPX_TEST(false);
    }

    HPX_TEST(caught_bad_alloc);
}

template <typename IteratorTag>
void test_sorted_until_seq_bad_alloc(IteratorTag)
{
    typedef std::vector<int>::iterator base_iterator;
    typedef test::test_iterator<base_iterator, IteratorTag> iterator;
    typedef test::decorated_iterator<base_iterator, IteratorTag>
        decorated_iterator;

    std::vector<int> c(10007);
    //fill first half of array with even numbers and second half
    //with odd numbers
    std::iota(std::begin(c), std::end(c), 0);

    bool caught_bad_alloc = false;
    try
    {
        hpx::ranges::is_sorted_until(
            decorated_iterator(
                std::begin(c), []() { throw std::runtime_error("test"); }),
            decorated_iterator(
                std::end(c), []() { throw std::runtime_error("test"); }));
    }
    catch (hpx::exception_list const& e)
    {
        caught_bad_alloc = true;
    }
    catch (...)
    {
        HPX_TEST(false);
    }

    HPX_TEST(caught_bad_alloc);

    caught_bad_alloc = false;
    try
    {
        hpx::ranges::is_sorted_until(iterator(std::begin(c)),
            iterator(std::end(c)),
            [](int, int) -> int { throw std::runtime_error("test"); });
    }
    catch (hpx::exception_list const& e)
    {
        caught_bad_alloc = true;
    }
    catch (...)
    {
        HPX_TEST(false);
    }

    HPX_TEST(caught_bad_alloc);

    caught_bad_alloc = false;
    try
    {
        hpx::ranges::is_sorted_until(iterator(std::begin(c)),
            iterator(std::end(c)), std::less<int>(),
            [](int) -> int { throw std::runtime_error("test"); });
    }
    catch (hpx::exception_list const& e)
    {
        caught_bad_alloc = true;
    }
    catch (...)
    {
        HPX_TEST(false);
    }

    HPX_TEST(caught_bad_alloc);
}

template <typename ExPolicy>
void test_sorted_until_bad_alloc(ExPolicy policy)
{
    static_assert(hpx::is_execution_policy<ExPolicy>::value,
        "hpx::is_execution_policy<ExPolicy>::value");

    std::vector<int> c(10007);
    //fill first half of array with even numbers and second half
    //with odd numbers
    std::iota(std::begin(c), std::end(c), 0);

    bool caught_bad_alloc = false;
    try
    {
        hpx::ranges::is_sorted_until(policy, c,
            [](int, int) -> bool { throw std::runtime_error("test"); });
    }
    catch (hpx::exception_list const& e)
    {
        caught_bad_alloc = true;
    }
    catch (...)
    {
        HPX_TEST(false);
    }

    HPX_TEST(caught_bad_alloc);

    caught_bad_alloc = false;
    try
    {
        hpx::ranges::is_sorted_until(policy, c, std::less<int>(),
            [](int) -> int { throw std::runtime_error("test"); });
    }
    catch (hpx::exception_list const& e)
    {
        caught_bad_alloc = true;
    }
    catch (...)
    {
        HPX_TEST(false);
    }

    HPX_TEST(caught_bad_alloc);
}

template <typename ExPolicy>
void test_sorted_until_async_bad_alloc(ExPolicy p)
{
    std::vector<int> c(10007);
    std::iota(std::begin(c), std::end(c), 0);

    bool caught_bad_alloc = false;
    try
    {
        auto f = hpx::ranges::is_sorted_until(
            p, c, [](int, int) -> int { throw std::runtime_error("test"); });
        f.get();

        HPX_TEST(false);
    }
    catch (hpx::exception_list const& e)
    {
        caught_bad_alloc = true;
    }
    catch (...)
    {
        HPX_TEST(false);
    }

    HPX_TEST(caught_bad_alloc);

    caught_bad_alloc = false;
    try
    {
        auto f = hpx::ranges::is_sorted_until(p, c, std::less<int>(),
            [](int) -> bool { throw std::runtime_error("test"); });
        f.get();

        HPX_TEST(false);
    }
    catch (hpx::exception_list const& e)
    {
        caught_bad_alloc = true;
    }
    catch (...)
    {
        HPX_TEST(false);
    }

    HPX_TEST(caught_bad_alloc);
}

void test_sorted_until_seq_bad_alloc()
{
    std::vector<int> c(10007);
    //fill first half of array with even numbers and second half
    //with odd numbers
    std::iota(std::begin(c), std::end(c), 0);

    bool caught_bad_alloc = false;
    try
    {
        hpx::ranges::is_sorted_until(
            c, [](int, int) -> int { throw std::runtime_error("test"); });
    }
    catch (hpx::exception_list const& e)
    {
        caught_bad_alloc = true;
    }
    catch (...)
    {
        HPX_TEST(false);
    }

    HPX_TEST(caught_bad_alloc);

    caught_bad_alloc = false;
    try
    {
        hpx::ranges::is_sorted_until(c, std::less<int>(),
            [](int) -> int { throw std::runtime_error("test"); });
    }
    catch (hpx::exception_list const& e)
    {
        caught_bad_alloc = true;
    }
    catch (...)
    {
        HPX_TEST(false);
    }

    HPX_TEST(caught_bad_alloc);
}

template <typename IteratorTag>
void test_sorted_until_bad_alloc()
{
    using namespace hpx::execution;

    // If the execution policy object is of type vector_execution_policy,
    // std::terminate shall be called. therefore we do not test exceptions
    // with a vector execution policy
    test_sorted_until_bad_alloc(par, IteratorTag());
    test_sorted_until_bad_alloc(seq, IteratorTag());

    test_sorted_until_async_bad_alloc(seq(task), IteratorTag());
    test_sorted_until_async_bad_alloc(par(task), IteratorTag());

    test_sorted_until_seq_bad_alloc(IteratorTag());
}

void sorted_until_bad_alloc_test()
{
    test_sorted_until_bad_alloc<std::random_access_iterator_tag>();
    test_sorted_until_bad_alloc<std::forward_iterator_tag>();

    using namespace hpx::execution;

    test_sorted_until_bad_alloc(par);
    test_sorted_until_bad_alloc(seq);

    test_sorted_until_async_bad_alloc(seq(task));
    test_sorted_until_async_bad_alloc(par(task));

    test_sorted_until_seq_bad_alloc();
}

int hpx_main()
{
    sorted_until_test1();
    sorted_until_test2();
    sorted_until_test3();
    sorted_until_exception_test();
    sorted_until_bad_alloc_test();

    std::vector<int> c(100);
    hpx::future<void> f =
        hpx::dataflow(hpx::ranges::is_sorted, hpx::execution::par, c);
    f = hpx::dataflow(
        hpx::unwrapping(hpx::ranges::is_sorted), hpx::execution::par, c, f);
    f.get();

    return hpx::local::finalize();
}

int main(int argc, char* argv[])
{
    using namespace hpx::program_options;
    options_description desc_commandline(
        "Usage: " HPX_APPLICATION_STRING " [options]");

    std::vector<std::string> const cfg = {"hpx.os_threads=all"};

    hpx::local::init_params init_args;
    init_args.desc_cmdline = desc_commandline;
    init_args.cfg = cfg;

    HPX_TEST_EQ_MSG(hpx::local::init(hpx_main, argc, argv, init_args), 0,
        "HPX main exited with non-zero status");

    return hpx::util::report_errors();
}
