#include <benchmark/benchmark.h>

#include <unordered_set>

#include "Utils.h"

constexpr auto s_testCount = 8 << 17;

using TCrt = int32_t;
using point = boost::geometry::model::point<TCrt, 2, boost::geometry::cs::cartesian>;
using box = boost::geometry::model::box<point>;
using value = std::pair<box, unsigned>;

class DataStorage
{

    static constexpr auto s_count = s_testCount;
    static constexpr auto s_maxPos = 1'000'000;
    static constexpr auto s_maxRectWidth = 1'000;
    static constexpr auto s_maxRectHeight = 1'000;
public:
    static DataStorage& Instance()
    {
        static DataStorage s_instance;
        return s_instance;
    }

    DataStorage(DataStorage&&) = delete;
    DataStorage(const DataStorage&) = delete;
    DataStorage operator=(DataStorage&&) = delete;
    DataStorage operator=(const DataStorage&) = delete;

public:

    const auto& BoostBoxList() const noexcept
    {
        return m_boostBoxList;
    }

    const auto& SpaceBoxList() const noexcept
    {
        return m_spaceBoxList;
    }

private:

    DataStorage()
    {
        std::set<space::Rect<TCrt>> initialRects;

        while (initialRects.size() != s_count)
        {
            const auto rect = test_util::getRandRect(s_maxPos, s_maxRectWidth, s_maxRectHeight);
            initialRects.insert(rect);
        }

        m_spaceBoxList.reserve(std::size(initialRects));
        m_boostBoxList.reserve(std::size(initialRects));
        for (auto&& rect : initialRects)
        {
            m_spaceBoxList.push_back(rect);
            const auto boostRect = test_util::spaceToBoostRect(rect);
            m_boostBoxList.push_back(boostRect);
        }
    }

private:
    std::vector<box> m_boostBoxList;
    std::vector<space::Rect<TCrt>> m_spaceBoxList;
};

static void BoostSpaceIndexInsert(benchmark::State& state)
{
    const auto& boxList = DataStorage::Instance().BoostBoxList();
    const auto count = state.range(0);

    boost::geometry::index::rtree<value, boost::geometry::index::quadratic<16>> rtree;
    for (auto _ : state)
    {
        for (int i = 0; i < count; ++i)
        {
            rtree.insert(std::make_pair(boxList[i], 0));
        }
    }
}
// Register the function as a benchmark
BENCHMARK(BoostSpaceIndexInsert)->Range(512, s_testCount);

static void SpaceQuadTreeInsert(benchmark::State& state)
{
    const auto& boxList = DataStorage::Instance().SpaceBoxList();
    const auto count = state.range(0);

    space::QuadTree<space::Rect<TCrt>> quadTree;
    for (auto _ : state)
    {
        for (int i = 0; i < count; ++i)
        {
            quadTree.insert(boxList[i]);
        }
    }
}
// Register the function as a benchmark
BENCHMARK(SpaceQuadTreeInsert)->Range(512, s_testCount);



int main(int argc, char** argv)
{
    auto& dataStorage = DataStorage::Instance();
    ::benchmark::Initialize(&argc, argv);
    if (::benchmark::ReportUnrecognizedArguments(argc, argv))
    {
        return 1;
    }
    ::benchmark::RunSpecifiedBenchmarks();
}

// BENCHMARK_MAIN();
