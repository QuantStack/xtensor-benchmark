/***************************************************************************
* Copyright (c) Johan Mabille, Sylvain Corlay and Wolf Vollprecht          *
* Copyright (c) QuantStack                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/

#include <benchmark/benchmark.h>

#ifdef HAS_XTENSOR
#include "xtensor/xnoalias.hpp"
#include "xtensor/xio.hpp"
#include "xtensor/xrandom.hpp"
#include "xtensor/xtensor.hpp"
#include "xtensor/xarray.hpp"
#include "xtensor/xstrided_view.hpp"
#include "xtensor/xview.hpp"
#include "xtensor/xdynamic_view.hpp"
#include "xtensor/xadapt.hpp"
#endif

#ifdef HAS_EIGEN
#include <Eigen/Dense>
#include <Eigen/Core>
#endif

#ifdef HAS_BLITZ
#include <blitz/array.h>
#endif

#ifdef HAS_ARMADILLO
#include <armadillo>
#endif

#ifdef HAS_VIGRA
#include "vigra/multi_array.hxx"
#include "vigra/multi_math.hxx"
#endif

#define RANGE 3, 1000
#define MULTIPLIER 8


#ifdef HAS_XTENSOR
void Add2dView_XTensor(benchmark::State& state)
{
    using namespace xt;

    xtensor<double,2> vA = random::rand<double>({state.range(0), state.range(0)});
    xtensor<double,2> vB = random::rand<double>({state.range(0), state.range(0)});

    auto vAView = xt::view(vA, all(), all());
    auto vBView = xt::view(vB, all(), all());

    for (auto _ : state)
    {
        xtensor<double,2> vRes(vAView + vBView);
        benchmark::DoNotOptimize(vRes.data());
    }
}
BENCHMARK(Add2dView_XTensor)->RangeMultiplier(MULTIPLIER)->Range(RANGE);

void Assign2dView_XTensor(benchmark::State& state)
{
    using namespace xt;

    xtensor<double,2> vA = random::rand<double>({state.range(0), state.range(0)});
    xtensor<double,2> vB = random::rand<double>({state.range(0), state.range(0)});

    auto vAView = xt::view(vA, all(), all());

    for (auto _ : state)
    {
        xt::noalias(vAView) = vB;
        benchmark::DoNotOptimize(vA.data());
    }
}
BENCHMARK(Assign2dView_XTensor)->RangeMultiplier(MULTIPLIER)->Range(RANGE);

void Assign2dNotContiguousHalfView_XTensor(benchmark::State& state)
{
    using namespace xt;

    size_t size = state.range(0);

    xtensor<double, 2> vA = random::rand<double>({size, size});
    xtensor<double, 2> vB = random::rand<double>({size/2, size/2});

    auto vAView = xt::view(vA, range(size/4, size/4 + size/2),
                               range(size/4, size/4 + size/2));

    for (auto _ : state)
    {
        xt::noalias(vAView) = vB;
        benchmark::DoNotOptimize(vA.data());
    }
}
BENCHMARK(Assign2dNotContiguousHalfView_XTensor)->RangeMultiplier(MULTIPLIER)->Range(RANGE);

void Assign2dContiguousHalfView_XTensor(benchmark::State& state)
{
    using namespace xt;

    size_t size = state.range(0);

    xtensor<double, 2> vA = random::rand<double>({size, size});
    xtensor<double, 2> vB = random::rand<double>({size/4, size});

    auto vAView = xt::view(vA, range(0, size/4),
                               range(0, size));

    for (auto _ : state)
    {
        xt::noalias(vAView) = vB;
        benchmark::DoNotOptimize(vA.data());
    }
}
BENCHMARK(Assign2dContiguousHalfView_XTensor)->RangeMultiplier(MULTIPLIER)->Range(RANGE);

void Add2dStridedView_XTensor(benchmark::State& state)
{
    using namespace xt;

    xtensor<double, 2> vA = random::rand<double>({state.range(0), state.range(0)});
    xtensor<double, 2> vB = random::rand<double>({state.range(0), state.range(0)});

    auto vAView = xt::strided_view(vA, {all(), all()});
    auto vBView = xt::strided_view(vB, {all(), all()});

    for (auto _ : state)
    {
        xtensor<double, 2> vRes(vAView + vBView);
        benchmark::DoNotOptimize(vRes.data());
    }
}
BENCHMARK(Add2dStridedView_XTensor)->RangeMultiplier(MULTIPLIER)->Range(RANGE);

void Assign2dStridedView_XTensor(benchmark::State& state)
{
    using namespace xt;

    xtensor<double, 2> vA = random::rand<double>({state.range(0), state.range(0)});
    xtensor<double, 2> vB = random::rand<double>({state.range(0), state.range(0)});

    auto vAView = xt::strided_view(vA, {all(), all()});

    for (auto _ : state)
    {
        xt::noalias(vAView) = vB;
        benchmark::DoNotOptimize(vA.data());
    }
}
BENCHMARK(Assign2dStridedView_XTensor)->RangeMultiplier(MULTIPLIER)->Range(RANGE);

void Assign2dNotContiguousHalfStridedView_XTensor(benchmark::State& state)
{
    using namespace xt;

    size_t size = state.range(0);

    xtensor<double, 2> vA = random::rand<double>({size, size});
    xtensor<double, 2> vB = random::rand<double>({size/2, size/2});

    auto vAView = xt::strided_view(vA, {range(size/4, size/4 + size/2),
                                        range(size/4, size/4 + size/2)});

    for (auto _ : state)
    {
        xt::noalias(vAView) = vB;
        benchmark::DoNotOptimize(vA.data());
    }
}
BENCHMARK(Assign2dNotContiguousHalfStridedView_XTensor)->RangeMultiplier(MULTIPLIER)->Range(RANGE);

void Assign2dContiguousHalfStridedView_XTensor(benchmark::State& state)
{
    using namespace xt;

    size_t size = state.range(0);

    xtensor<double, 2> vA = random::rand<double>({size, size});
    xtensor<double, 2> vB = random::rand<double>({size/4, size});

    auto vAView = xt::strided_view(vA, {range(0, size/4),
                                        range(0, size)});

    for (auto _ : state)
    {
        xt::noalias(vAView) = vB;
        benchmark::DoNotOptimize(vA.data());
    }
}
BENCHMARK(Assign2dContiguousHalfStridedView_XTensor)->RangeMultiplier(MULTIPLIER)->Range(RANGE);

void Add2dDynamicView_XTensor(benchmark::State& state)
{
    using namespace xt;

    xtensor<double, 2> vA = random::rand<double>({state.range(0), state.range(0)});
    xtensor<double, 2> vB = random::rand<double>({state.range(0), state.range(0)});

    auto vAView = xt::dynamic_view(vA, {all(), all()});
    auto vBView = xt::dynamic_view(vB, {all(), all()});

    for (auto _ : state)
    {
        xtensor<double, 2> vRes(vAView + vBView);
        benchmark::DoNotOptimize(vRes.data());
    }
}
BENCHMARK(Add2dDynamicView_XTensor)->RangeMultiplier(MULTIPLIER)->Range(RANGE);

void Add2dAdapt_XTensor(benchmark::State& state)
{
    using namespace xt;

    xtensor<double, 2> vA = random::rand<double>({state.range(0), state.range(0)});
    xtensor<double, 2> vB = random::rand<double>({state.range(0), state.range(0)});
    std::size_t vSize = static_cast<std::size_t>(state.range(0));
    std::array<std::size_t, 2> vShape = {vSize, vSize};
    auto vAView = xt::adapt(std::move(vA.data()), vShape);
    auto vBView = xt::adapt(std::move(vB.data()), vShape);

    for (auto _ : state)
    {
        xtensor<double, 2> vRes(vAView + vBView);
        benchmark::DoNotOptimize(vRes.data());
    }
}
BENCHMARK(Add2dAdapt_XTensor)->RangeMultiplier(MULTIPLIER)->Range(RANGE);

void Add2dLoop_XTensor(benchmark::State& state)
{
    using namespace xt;

    size_t size = state.range(0);

    xtensor<double, 2> vA = random::rand<double>({size, size});
    xtensor<double, 2> vB = random::rand<double>({size, size});
    std::array<std::size_t, 2> vShape = {static_cast<std::size_t>(size), static_cast<std::size_t>(size)};
    for (auto _ : state)
    {
        xtensor<double, 2> vRes(vShape);
        for (std::size_t i = 0; i < vRes.shape()[0]; ++i)
        {
            for (std::size_t j = 0; j < vRes.shape()[1]; ++j)
            {
                vRes(i, j) = vA(i, j) + vB(i, j);
            }
        }
        benchmark::DoNotOptimize(vRes.data());
    }
}
BENCHMARK(Add2dLoop_XTensor)->RangeMultiplier(MULTIPLIER)->Range(RANGE);

void Assign2dLoop_XTensor(benchmark::State& state)
{
    using namespace xt;

    size_t size = state.range(0);

    xtensor<double, 2> vA = random::rand<double>({size, size});
    xtensor<double, 2> vB = random::rand<double>({size, size});
    for (auto _ : state)
    {
        for (std::size_t i = 0; i < size; ++i)
        {
            for (std::size_t j = 0; j < size; ++j)
            {
                vA(i, j) = vB(i, j);
            }
        }
        benchmark::DoNotOptimize(vA.data());
    }
}
BENCHMARK(Assign2dLoop_XTensor)->RangeMultiplier(MULTIPLIER)->Range(RANGE);

void Assign2dNotContiguousHalfLoop_XTensor(benchmark::State& state)
{
    using namespace xt;

    size_t size = state.range(0);

    xtensor<double, 2> vA = random::rand<double>({size, size});
    xtensor<double, 2> vB = random::rand<double>({size/2, size/2});
    for (auto _ : state)
    {
        for (std::size_t i = 0; i < size / 2; ++i) {
            for (std::size_t j = 0; j < size / 2; ++j) {
                vA(i + size / 4, j + size / 4) = vB(i, j);
            }
        }
        benchmark::DoNotOptimize(vA.data());
    }
}
BENCHMARK(Assign2dNotContiguousHalfLoop_XTensor)->RangeMultiplier(MULTIPLIER)->Range(RANGE);
#endif

#ifdef HAS_VIGRA
void Add2dView_Vigra(benchmark::State& state)
{
    using namespace vigra::multi_math;

    vigra::MultiArray<2,double> vA(state.range(0), state.range(0));
    vigra::MultiArray<2,double> vB(state.range(0), state.range(0));

    vigra::MultiArrayView<2,double> vAView = vA.subarray({0, 0}, {state.range(0), state.range(0)});
    vigra::MultiArrayView<2,double> vBView = vB.subarray({0, 0}, {state.range(0), state.range(0)});

    for (auto _ : state)
    {
        vigra::MultiArray<2,double> vRes(vAView + vBView);
        benchmark::DoNotOptimize(vRes.data());
    }
}
BENCHMARK(Add2dView_Vigra)->RangeMultiplier(MULTIPLIER)->Range(RANGE);

void Assign2dView_Vigra(benchmark::State& state)
{
    using namespace vigra::multi_math;

    vigra::MultiArray<2,double> vA(state.range(0), state.range(0));
    vigra::MultiArray<2,double> vB(state.range(0), state.range(0));

    vA = 1;
    vB = 2;

    vigra::MultiArrayView<2,double> vAView = vA.subarray({0, 0}, {state.range(0), state.range(0)});

    for (auto _ : state)
    {
        vAView = vB;
        benchmark::DoNotOptimize(vA.data());
    }
}
BENCHMARK(Assign2dView_Vigra)->RangeMultiplier(MULTIPLIER)->Range(RANGE);

void Assign2dHalfView_Vigra(benchmark::State& state)
{
    using namespace vigra::multi_math;

    size_t size = state.range(0);

    vigra::MultiArray<2,double> vA(size, size);
    vigra::MultiArray<2,double> vB(size/2, size/2);

    vA = 1;
    vB = 2;

    for (auto _ : state)
    {
        vA.subarray({size/4, size/4}, {size/4 + size/2, size/4 + size/2}) = vB;
        benchmark::DoNotOptimize(vA.data());
    }
}
BENCHMARK(Assign2dHalfView_Vigra)->RangeMultiplier(MULTIPLIER)->Range(RANGE);
#endif

#ifdef HAS_EIGEN
void Add2dView_Eigen(benchmark::State& state)
{
    using namespace Eigen;
    MatrixXd vA = MatrixXd::Random(state.range(0), state.range(0));
    MatrixXd vB = MatrixXd::Random(state.range(0), state.range(0));

    auto vAView = vA.topLeftCorner(state.range(0), state.range(0));
    auto vBView = vB.topLeftCorner(state.range(0), state.range(0));

    for (auto _ : state)
    {
        MatrixXd vRes(state.range(0), state.range(0));
        vRes.noalias() = vAView + vBView;
        benchmark::DoNotOptimize(vRes.data());
    }
}
BENCHMARK(Add2dView_Eigen)->RangeMultiplier(MULTIPLIER)->Range(RANGE);

//void Add1dMap_Eigen(benchmark::State& state)
//{
//    using namespace Eigen;
//    MatrixXd vA = VectorXd::Random(state.range(0));
//    MatrixXd vB = VectorXd::Random(state.range(0));
//
//    auto vAView = Map<VectorXd, 0, InnerStride<1>>(vA.data(), vA.size());
//    auto vBView = Map<VectorXd, 0, InnerStride<1>>(vB.data(), vB.size());
//
//    for (auto _ : state)
//    {
//        VectorXd vRes(vAView + vBView);
//        benchmark::DoNotOptimize(vRes.data());
//    }
//}
//BENCHMARK(Add1dMap_Eigen)->RangeMultiplier(MULTIPLIER)->Range(RANGE);
#endif

#undef RANGE
#undef MULTIPLIER

