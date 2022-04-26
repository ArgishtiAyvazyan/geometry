#include <benchmark/benchmark.h>

#include <unordered_set>

#include "Utils.h"

constexpr auto s_shapeCount = 8 << 13;
constexpr auto s_testCount = 8 << 20;

using TCrt = int32_t;
using point = boost::geometry::model::point<TCrt, 2, boost::geometry::cs::cartesian>;
using box = boost::geometry::model::box<point>;
using value = std::pair<box, bool>;

class DataStorage
{
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

    const auto& BoostIndex() const noexcept
    {
        return m_boostIndex;
    }

    const auto& SpaceIndex() const noexcept
    {
        return m_spaceIndex;
    }

    const auto& BoostQueryBoxList() const noexcept
    {
        return m_boostQueryBoxList;
    }

    const auto& SpaceQueryBoxList() const noexcept
    {
        return m_spaceQueryBoxList;
    }

private:

    DataStorage()
    {
        const auto initialRects = getRandomRectList(s_shapeCount);

        for (auto&& rect : initialRects)
        {
            m_spaceIndex.insert(rect);
            const auto boostRect = test_util::spaceToBoostRect(rect);
            m_boostIndex.insert(std::make_pair(boostRect, false));
        }

        m_spaceQueryBoxList.reserve(s_testCount);
        m_boostQueryBoxList.reserve(s_testCount);

        for (auto&& rect : getRandomRectList(s_testCount))
        {
            m_spaceQueryBoxList.push_back(rect);
            const auto boostRect = test_util::spaceToBoostRect(rect);
            m_boostQueryBoxList.push_back(boostRect);
        }
    }

    std::set<space::Rect<TCrt>> getRandomRectList(size_t count)
    {
        std::set<space::Rect<TCrt>> rects;

        while (rects.size() != count)
        {
            const auto rect = test_util::getRandRect(s_maxPos, s_maxRectWidth, s_maxRectHeight);
            rects.insert(rect);
        }

        return rects;
    }

private:
    boost::geometry::index::rtree<value, boost::geometry::index::quadratic<16>> m_boostIndex;
    space::QuadTree<space::Rect<TCrt>> m_spaceIndex;
    std::vector<box> m_boostQueryBoxList;
    std::vector<space::Rect<TCrt>> m_spaceQueryBoxList;
};

static void BoostSpaceIndexQuery(benchmark::State& state)
{
    const auto& index = DataStorage::Instance().BoostIndex();
    const auto& queryList = DataStorage::Instance().BoostQueryBoxList();

    const auto count = state.range(0);

    std::vector<value> rTreeQueryRes;
    rTreeQueryRes.reserve(s_shapeCount);

    for (auto _ : state)
    {
        for (int i = 0; i < count; ++i)
        {
            index.query(boost::geometry::index::intersects(queryList[i]), std::back_inserter(rTreeQueryRes));
            state.PauseTiming();
            rTreeQueryRes.clear();
            state.ResumeTiming();
        }
    }
    benchmark::DoNotOptimize(rTreeQueryRes);
}

BENCHMARK(BoostSpaceIndexQuery)->Range(512, s_testCount);

static void SpaceQuadTreeQuery(benchmark::State& state)
{
    const auto& index = DataStorage::Instance().SpaceIndex();
    const auto& queryList = DataStorage::Instance().SpaceQueryBoxList();

    const auto count = state.range(0);

    std::vector<space::Rect<TCrt>> quadTreeQueryRes;
    quadTreeQueryRes.reserve(s_shapeCount);

    for (auto _ : state)
    {
        for (int i = 0; i < count; ++i)
        {
            index.query(queryList[i], std::back_inserter(quadTreeQueryRes));
            state.PauseTiming();
            quadTreeQueryRes.clear();
            state.ResumeTiming();
        }
    }
    benchmark::DoNotOptimize(quadTreeQueryRes);
}

BENCHMARK(SpaceQuadTreeQuery)->Range(512, s_testCount);

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
