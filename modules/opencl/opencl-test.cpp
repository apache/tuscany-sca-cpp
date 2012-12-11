/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

/* $Rev$ $Date$ */

/**
 * Test OpenCL kernel evaluator.
 */

#include <assert.h>
#include "stream.hpp"
#include "string.hpp"
#include "driver.hpp"
#include "parallel.hpp"
#include "perf.hpp"

namespace tuscany {
namespace opencl {

const string testFloatsCPU =
        "kernel void add(const float x, const float y, global float* r) {\n"
        "    float l_x;\n"
        "    float l_y;\n"
        "    l_x = x;\n"
        "    l_y = y;\n"
        "    *r = l_x + l_y;\n"
        "}\n"
        "kernel void float_add(const float x, const float y, global float* r) {\n"
        "    add(x, y, r);\n"
        "}\n";

const string testFloatsGPU =
        "kernel void add(const float x, const float y, global float* r) {\n"
        "    local float l_x;\n"
        "    local float l_y;\n"
        "    l_x = x;\n"
        "    l_y = y;\n"
        "    barrier(CLK_LOCAL_MEM_FENCE);\n"
        "    *r = l_x + l_y;\n"
        "}\n"
        "kernel void float_add(const float x, const float y, global float* r) {\n"
        "    add(x, y, r);\n"
        "}\n";

const string testBoolsCPU =
        "kernel void int_or(const int x, const int y, global int* r) {\n"
        "    int l_x;\n"
        "    int l_y;\n"
        "    l_x = x;\n"
        "    l_y = y;\n"
        "    *r = l_x || l_y;\n"
        "}\n";

const string testBoolsGPU =
        "kernel void int_or(const int x, const int y, global int* r) {\n"
        "    local int l_x;\n"
        "    local int l_y;\n"
        "    l_x = x;\n"
        "    l_y = y;\n"
        "    barrier(CLK_LOCAL_MEM_FENCE);\n"
        "    *r = l_x || l_y;\n"
        "}\n";

const string testCharsCPU =
        "kernel void add(const float xl, global const char* x, const float yl, global const char* y, global char* r) {\n"
        "    const int ixl = (int)xl;\n"
        "    const int iyl = (int)yl;\n"
        "    for (int i = 0; i < ixl; i++)\n"
        "        r[i] = x[i];\n"
        "    for (int i = 0; i < iyl; i++)\n"
        "        r[ixl + i] = y[i];\n"
        "    r[ixl + iyl] = '\\0';\n"
        "}\n"
        "kernel void char128_add(const float xl, global const char* x, const float yl, global const char* y, global char* r) {\n"
        "    add(xl, x, yl, y, r);\n"
        "}\n";

const string testCharsCopyGPU =
        "kernel void add(const float xl, global const char* x, const float yl, global const char* y, global char* r) {\n"
        "    const int ixl = (int)xl;\n"
        "    const int iyl = (int)yl;\n"
        "    local char l_x[128];\n"
        "    local char l_y[128];\n"
        "    event_t re = async_work_group_copy(l_x, x, ixl, 0);\n"
        "    async_work_group_copy(l_y, y, iyl, re);\n"
        "    wait_group_events(1, &re);\n"
        "    local char l_r[128];\n"
        "    for (int i = 0; i < ixl; i++)\n"
        "        l_r[i] = l_x[i];\n"
        "    for (int i = 0; i < iyl; i++)\n"
        "        l_r[ixl + i] = l_y[i];\n"
        "    l_r[ixl + iyl] = '\\0';\n"
        "    event_t we = async_work_group_copy(r, l_r, ixl + iyl + 1, 0);\n"
        "    wait_group_events(1, &we);\n"
        "}\n"
        "kernel void char128(const float xl, global const char* x, const float yl, global const char* y, global char* r) {\n"
        "    add(xl, x, yl, y, r);\n"
        "}\n";

const string testCharsGPU =
        "kernel void add(const float xl, global const char* x, const float yl, global const char* y, global char* r) {\n"
        "    const int ixl = (int)xl;\n"
        "    const int iyl = (int)yl;\n"
        "    for (int i = 0; i < ixl; i++)\n"
        "        r[i] = x[i];\n"
        "    for (int i = 0; i < iyl; i++)\n"
        "        r[ixl + i] = y[i];\n"
        "    r[ixl + iyl] = '\\0';\n"
        "}\n"
        "kernel void char128(const float xl, global const char* x, const float yl, global const char* y, global char* r) {\n"
        "    add(xl, x, yl, y, r);\n"
        "}\n";

const string testCharsParallelCPU =
        "kernel void add(const float xl, global const char* x, const float yl, global const char* y, global char* r) {\n"
        "    const int i = get_global_id(0);\n"
        "    const int ixl = (int)xl;\n"
        "    const int iyl = (int)yl;\n"
        "    r[i] = i < ixl? x[i] : i < ixl + iyl? y[i - ixl] : '\\0';\n"
        "}\n";

const string testCharsParallelCopyGPU =
        "kernel void add(const float xl, global const char* x, const float yl, global const char* y, global char* r) {\n"
        "    const int i = get_global_id(0);\n"
        "    const int ixl = (int)xl;\n"
        "    const int iyl = (int)yl;\n"
        "    local char l_x[128];\n"
        "    local char l_y[128];\n"
        "    event_t re = async_work_group_copy(l_x, x, ixl, 0);\n"
        "    async_work_group_copy(l_y, y, iyl, re);\n"
        "    wait_group_events(1, &re);\n"
        "    local char l_r[128];\n"
        "    l_r[i] = i < ixl? l_x[i] : i < ixl + iyl? l_y[i - ixl] : '\\0';\n"
        "    event_t we = async_work_group_copy(r, l_r, ixl + iyl + 1, 0);\n"
        "    wait_group_events(1, &we);\n"
        "}\n";

const string testCharsParallelGPU =
        "kernel void add(const float xl, global const char* x, const float yl, global const char* y, global char* r) {\n"
        "    const int i = get_global_id(0);\n"
        "    const int ixl = (int)xl;\n"
        "    const int iyl = (int)yl;\n"
        "    r[i] = i < ixl? x[i] : i < ixl + iyl? y[i - ixl] : '\\0';\n"
        "}\n";

const bool testTaskParallel(const OpenCLContext::DeviceType dev) {
    const gc_scoped_pool pool;
    OpenCLContext cl(dev);
    if (!devices(cl) != 0)
        return true;

    {
        istringstream is(dev == OpenCLContext::CPU? testFloatsCPU : testFloatsGPU);
        failable<OpenCLProgram> clprog = readProgram("kernel.cl", is, cl);
        assert(hasContent(clprog));
        const value exp = mklist<value>("float_add", 2, 3);
        const failable<value> r = evalKernel(createKernel(exp, content(clprog)), exp, cl);
        assert(hasContent(r));
        assert(content(r) == value(5));
    }
    if (true) return true;
    {
        istringstream is(dev == OpenCLContext::CPU? testFloatsCPU : testFloatsGPU);
        failable<OpenCLProgram> clprog = readProgram("kernel.cl", is, cl);
        assert(hasContent(clprog));
        const value exp = mklist<value>("add", 2, 3);
        const failable<value> r = evalKernel(createKernel(exp, content(clprog)), exp, 1, value::Number, 1, cl);
        assert(hasContent(r));
        assert(content(r) == value(5));
    }
    {
        istringstream is(dev == OpenCLContext::CPU? testBoolsCPU : testBoolsGPU);
        failable<OpenCLProgram> clprog = readProgram("kernel.cl", is, cl);
        assert(hasContent(clprog));

        const value exp = mklist<value>("int_or", true, false);
        const failable<value> r = evalKernel(createKernel(exp, content(clprog)), exp, cl);
        assert(hasContent(r));
        assert(content(r) == trueValue);

        const value exp2 = mklist<value>("int_or", false, false);
        const failable<value> r2 = evalKernel(createKernel(exp2, content(clprog)), exp2, cl);
        assert(hasContent(r2));
        assert(content(r2) == falseValue);
    }
    {
        istringstream is(dev == OpenCLContext::CPU? testCharsCPU : testCharsGPU);
        failable<OpenCLProgram> clprog = readProgram("kernel.cl", is, cl);
        assert(hasContent(clprog));

        const value exp = mklist<value>("char128", 60, string("Hello Hello Hello Hello Hello Hello Hello Hello Hello Hello "), 60, string("World World World World World World World World World World "));
        const failable<value> r = evalKernel(createKernel(exp, content(clprog)), exp, cl);
        assert(hasContent(r));
        assert(content(r) == value(string("Hello Hello Hello Hello Hello Hello Hello Hello Hello Hello World World World World World World World World World World ")));
    }
    {
        istringstream is(dev == OpenCLContext::CPU? testCharsCPU : testCharsGPU);
        failable<OpenCLProgram> clprog = readProgram("kernel.cl", is, cl);
        assert(hasContent(clprog));

        const value exp = mklist<value>("add", 60, string("Hello Hello Hello Hello Hello Hello Hello Hello Hello Hello "), 60, string("World World World World World World World World World World "));
        const failable<value> r = evalKernel(createKernel(exp, content(clprog)), exp, 1, value::String, 128, cl);
        assert(hasContent(r));
        assert(content(r) == value(string("Hello Hello Hello Hello Hello Hello Hello Hello Hello Hello World World World World World World World World World World ")));
    }

    return true;
}

const bool testTaskParallelPerf(const OpenCLContext::DeviceType dev, const bool copy) {
    const gc_scoped_pool pool;
    OpenCLContext cl(dev);
    if (!devices(cl) != 0)
        return true;

    istringstream is(dev == OpenCLContext::CPU? testCharsCPU : copy? testCharsCopyGPU : testCharsGPU);
    failable<OpenCLProgram> fclprog = readProgram("kernel.cl", is, cl);
    assert(hasContent(fclprog));

    resetOpenCLCounters();
    OpenCLProgram clprog = content(fclprog);
    const value exp = mklist<value>("add", 60, string("Hello Hello Hello Hello Hello Hello Hello Hello Hello Hello "), 60, string("World World World World World World World World World World "));
    const blambda el = [cl, clprog, exp]() -> const bool {
        const failable<value> r = evalKernel(createKernel(exp, clprog), exp, 1, value::String, 128, cl);
        assert(hasContent(r));
        assert(content(r) == value(string("Hello Hello Hello Hello Hello Hello Hello Hello Hello Hello World World World World World World World World World World ")));
        return true;
    };
    cout << "OpenCL task-parallel eval " << (dev == OpenCLContext::CPU? "CPU" : "GPU") << (copy? " copy" : "") << " test " << time(el, 5, 500) << " ms";
    printOpenCLCounters(500);
    cout << endl;
    return true;
}

const bool testDataParallel(const OpenCLContext::DeviceType dev) {
    const gc_scoped_pool pool;
    OpenCLContext cl(dev);
    if (!devices(cl) != 0)
        return true;

    {
        istringstream is(dev == OpenCLContext::CPU? testCharsParallelCPU : testCharsParallelGPU);
        failable<OpenCLProgram> clprog = readProgram("kernel.cl", is, cl);
        assert(hasContent(clprog));

        const value exp = mklist<value>("add", 6, string("Hello "), 5, string("World"));
        const failable<value> r = evalKernel(createKernel(exp, content(clprog)), exp, 121, value::String, 128, cl);
        assert(hasContent(r));
        assert(content(r) == value(string("Hello World")));
    }

    return true;
}

const bool testDataParallelPerf(const OpenCLContext::DeviceType dev, const bool copy) {
    const gc_scoped_pool pool;
    OpenCLContext cl(dev);
    if (!devices(cl) != 0)
        return true;

    istringstream is(dev == OpenCLContext::CPU? testCharsParallelCPU : copy? testCharsParallelCopyGPU : testCharsParallelGPU);
    failable<OpenCLProgram> fclprog = readProgram("kernel.cl", is, cl);
    assert(hasContent(fclprog));

    resetOpenCLCounters();
    OpenCLProgram clprog = content(fclprog);
    const value exp = mklist<value>("add", 60, string("Hello Hello Hello Hello Hello Hello Hello Hello Hello Hello "), 60, string("World World World World World World World World World World "));
    const blambda el = [cl, clprog, exp]() -> const bool {
        const failable<value> r = evalKernel(createKernel(exp, clprog), exp, 121, value::String, 128, cl);
        assert(hasContent(r));
        assert(content(r) == value(string("Hello Hello Hello Hello Hello Hello Hello Hello Hello Hello World World World World World World World World World World ")));
        return true;
    };
    cout << "OpenCL data-parallel eval " << (dev == OpenCLContext::CPU? "CPU" : "GPU") << (copy? " copy" : "") << " test " << time(el, 5, 500) << " ms";
    printOpenCLCounters(500);
    cout << endl;
    return true;
}

}
}

int main() {
    tuscany::cout << "Testing..." << tuscany::endl;

    tuscany::opencl::testTaskParallel(tuscany::opencl::OpenCLContext::CPU);
    tuscany::opencl::testTaskParallelPerf(tuscany::opencl::OpenCLContext::CPU, false);
    tuscany::opencl::testDataParallel(tuscany::opencl::OpenCLContext::CPU);
    tuscany::opencl::testDataParallelPerf(tuscany::opencl::OpenCLContext::CPU, false);

    tuscany::opencl::testTaskParallel(tuscany::opencl::OpenCLContext::GPU);
    tuscany::opencl::testTaskParallelPerf(tuscany::opencl::OpenCLContext::GPU, false);
    tuscany::opencl::testTaskParallelPerf(tuscany::opencl::OpenCLContext::GPU, true);
    tuscany::opencl::testDataParallel(tuscany::opencl::OpenCLContext::GPU);
    tuscany::opencl::testDataParallelPerf(tuscany::opencl::OpenCLContext::GPU, false);
    tuscany::opencl::testDataParallelPerf(tuscany::opencl::OpenCLContext::GPU, true);

    tuscany::cout << "OK" << tuscany::endl;
    return 0;
}
