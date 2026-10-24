#include <gtest/gtest.h>
#include <LRUCache.h>

#include <string>

// ---------- Basic behavior ----------

TEST(LRUCache, GetOnEmptyReturnsMinusOne)
{
    LRUCache cache(2);
    EXPECT_EQ(cache.get("missing"), -1);
}

TEST(LRUCache, PutThenGetReturnsValue)
{
    LRUCache cache(2);
    cache.put("a", 1);
    EXPECT_EQ(cache.get("a"), 1);
}

TEST(LRUCache, PutOverwritesExistingValue)
{
    LRUCache cache(2);
    cache.put("a", 1);
    cache.put("a", 42);
    EXPECT_EQ(cache.get("a"), 42);
}

TEST(LRUCache, GetMissingKeyDoesNotEvict)
{
    LRUCache cache(2);
    cache.put("a", 1);
    cache.put("b", 2);
    EXPECT_EQ(cache.get("nope"), -1);
    // Both originals still present.
    EXPECT_EQ(cache.get("a"), 1);
    EXPECT_EQ(cache.get("b"), 2);
}

// ---------- Eviction ----------

TEST(LRUCache, EvictsLeastRecentlyUsedOnOverflow)
{
    LRUCache cache(2);
    cache.put("a", 1);
    cache.put("b", 2);
    cache.put("c", 3); // "a" should be evicted
    EXPECT_EQ(cache.get("a"), -1);
    EXPECT_EQ(cache.get("b"), 2);
    EXPECT_EQ(cache.get("c"), 3);
}

TEST(LRUCache, GetPromotesToMostRecentlyUsed)
{
    LRUCache cache(2);
    cache.put("a", 1);
    cache.put("b", 2);
    cache.get("a");    // "a" is now MRU, "b" is LRU
    cache.put("c", 3); // should evict "b", not "a"
    EXPECT_EQ(cache.get("a"), 1);
    EXPECT_EQ(cache.get("b"), -1);
    EXPECT_EQ(cache.get("c"), 3);
}

TEST(LRUCache, PutOnExistingKeyPromotesToMostRecentlyUsed)
{
    LRUCache cache(2);
    cache.put("a", 1);
    cache.put("b", 2);
    cache.put("a", 10); // update "a" -> MRU, "b" is LRU
    cache.put("c", 3);  // should evict "b"
    EXPECT_EQ(cache.get("a"), 10);
    EXPECT_EQ(cache.get("b"), -1);
    EXPECT_EQ(cache.get("c"), 3);
}

TEST(LRUCache, PutExistingKeyDoesNotGrowSize)
{
    LRUCache cache(2);
    cache.put("a", 1);
    cache.put("b", 2);
    cache.put("a", 99); // no new entry
    cache.put("c", 3);  // one eviction total
    EXPECT_EQ(cache.get("a"), 99);
    EXPECT_EQ(cache.get("b"), -1);
    EXPECT_EQ(cache.get("c"), 3);
}

// ---------- Repeated churn ----------

TEST(LRUCache, RepeatedEvictionsKeepWorking)
{
    LRUCache cache(3);
    cache.put("a", 1);
    cache.put("b", 2);
    cache.put("c", 3);
    cache.put("d", 4); // evicts "a"
    cache.put("e", 5); // evicts "b"
    cache.put("f", 6); // evicts "c"
    EXPECT_EQ(cache.get("a"), -1);
    EXPECT_EQ(cache.get("b"), -1);
    EXPECT_EQ(cache.get("c"), -1);
    EXPECT_EQ(cache.get("d"), 4);
    EXPECT_EQ(cache.get("e"), 5);
    EXPECT_EQ(cache.get("f"), 6);
}

TEST(LRUCache, CapacityOneEvictsOnEveryNewKey)
{
    LRUCache cache(1);
    cache.put("a", 1);
    cache.put("b", 2);
    EXPECT_EQ(cache.get("a"), -1);
    EXPECT_EQ(cache.get("b"), 2);
}

// ---------- Values ----------

TEST(LRUCache, HandlesNegativeAndZeroValues)
{
    LRUCache cache(2);
    cache.put("neg", -5);
    cache.put("zero", 0);
    EXPECT_EQ(cache.get("neg"), -5);
    EXPECT_EQ(cache.get("zero"), 0);
}

TEST(LRUCache, HandlesLargeKeysAndValues)
{
    LRUCache cache(2);
    std::string big_key(1000, 'k');
    cache.put(big_key, 123456789);
    EXPECT_EQ(cache.get(big_key), 123456789);
}