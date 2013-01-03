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

#ifndef tuscany_opencl_eval_hpp
#define tuscany_opencl_eval_hpp

/**
 * OpenCL kernel evaluation logic.
 */
#ifdef IS_DARWIN
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

#include "list.hpp"
#include "value.hpp"
#include "perf.hpp"

namespace tuscany {
namespace opencl {

/**
 * Convert an OpenCL error code to a string.
 */
const string clError(const cl_int e) {
    ostringstream s;
    s << "error " << e;
    return str(s);
}

/**
 * OpenCL profiling counters.
 */
#ifdef WANT_MAINTAINER_OPENCL_PROF

cl_ulong memtime = 0;
cl_ulong kernelqtime = 0;
cl_ulong kerneltime = 0;
cl_ulong preptime = 0;
cl_ulong evaltime = 0;

/**
 * Reset the OpenCL profiling counters.
 */
const bool resetOpenCLCounters() {
    memtime = kernelqtime = kerneltime = preptime = evaltime = 0;
    return true;
}

/**
 * Print the OpenCL profiling counters.
 */
const bool printOpenCLCounters(const long n) {
    cout << " kernelq " << ((double)kernelqtime / 1000000.0) / (double)n << " ms kernel " << ((double)kerneltime / 1000000.0) / (double)n << " ms memory " << ((double)memtime / 1000000.0) / (double)n << " ms prep " << ((double)preptime / 1000000.0) / (double)n << " ms eval " << ((double)evaltime / 1000000.0) / (double)n << " ms";
    return true;
}

/**
 * Profile a memory event.
 */
const failable<cl_ulong> profileMemEvent(const cl_event evt) {
    cl_ulong start = 0;
    const cl_int serr = clGetEventProfilingInfo(evt, CL_PROFILING_COMMAND_START, sizeof(cl_ulong), &start, NULL);
    if (serr != CL_SUCCESS)
        return mkfailure<cl_ulong>("Couldn't profile memory event start: " + clError(serr));
    cl_ulong end = 0;
    const cl_int eerr = clGetEventProfilingInfo(evt, CL_PROFILING_COMMAND_END, sizeof(cl_ulong), &end, NULL);
    if (eerr != CL_SUCCESS)
        return mkfailure<cl_ulong>("Couldn't profile memory event end: " + clError(eerr));
    const cl_ulong t = end - start;
    memtime += t;
    return t;
}

/**
 * Profile a kernel event.
 */
const failable<cl_ulong> profileKernelEvent(const cl_event evt) {
    cl_ulong queued = 0;
    const cl_int qerr = clGetEventProfilingInfo(evt, CL_PROFILING_COMMAND_QUEUED, sizeof(cl_ulong), &queued, NULL);
    if (qerr != CL_SUCCESS)
        return mkfailure<cl_ulong>("Couldn't profile kernel event queue: " + clError(qerr));
    cl_ulong start = 0;
    const cl_int serr = clGetEventProfilingInfo(evt, CL_PROFILING_COMMAND_START, sizeof(cl_ulong), &start, NULL);
    if (serr != CL_SUCCESS)
        return mkfailure<cl_ulong>("Couldn't profile kernel event start: " + clError(serr));
    cl_ulong end = 0;
    const cl_int eerr = clGetEventProfilingInfo(evt, CL_PROFILING_COMMAND_END, sizeof(cl_ulong), &end, NULL);
    if (eerr != CL_SUCCESS)
        return mkfailure<cl_ulong>("Couldn't profile kernel event end: " + clError(eerr));
    const cl_ulong q = start - queued;
    kernelqtime += q;
    const cl_ulong t = end - start;
    kerneltime += t;
    return t;
}

/**
 * Profile an array of memory events.
 */
const failable<cl_ulong> profileMemEvents(const cl_uint n, const cl_event* const evt) {
    if (n == 0)
        return 0;
    const failable<cl_ulong> t = profileMemEvent(*evt);
    if (!hasContent(t))
        return t;
    const failable<cl_ulong> r = profileMemEvents(n - 1, evt + 1);
    if (!hasContent(r))
        return r;
    return content(t) + content(r);
}

#else

#define resetOpenCLCounters()
#define printOpenCLCounters(n)

#endif

class OpenCLContext;
class OpenCLProgram;
class OpenCLKernel;
class OpenCLBuffer;

/**
 * Represent an OpenCL context.
 */
class OpenCLContext {
public:
#define OPENCL_MAX_DEVICES 64

    enum DeviceType {
        DEFAULT = 0, CPU = 1, GPU = 2
    };

    OpenCLContext(const OpenCLContext::DeviceType devtype) : dev(OpenCLContext::DEFAULT), ndevs(0), ctx(0) {
        debug("opencl::OpenCLContext");
        for (int i = 0; i < OPENCL_MAX_DEVICES; i++)
            cq[i] = 0;

        // Get the available platforms
        cl_uint nplatforms;
        cl_platform_id platforms[16];
        const cl_int gperr = clGetPlatformIDs(16, platforms, &nplatforms);
        if(nplatforms == 0 || gperr != CL_SUCCESS) {
            mkfailure<bool>("Couldn't get OpenCL platforms: " + clError(gperr));
            return;
        }
        for(cl_uint i = 0; i < nplatforms; ++i) {
            char vendor[256];
            const cl_int gverr = clGetPlatformInfo(platforms[i], CL_PLATFORM_VENDOR, sizeof(vendor), vendor, NULL);
            if(gverr != CL_SUCCESS) {
                mkfailure<bool>("Couldn't get OpenCL platform: " + clError(gverr));
                return;
            }
            debug(vendor, "opencl::OpenCLContext::vendor");
        }

        // Get the available devices of the requested type
        if (devtype == OpenCLContext::DEFAULT || devtype == OpenCLContext::GPU) {
            for(cl_uint i = 0; i < nplatforms; ++i) {
                const cl_int err = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_GPU, OPENCL_MAX_DEVICES, devid, &ndevs);
                if (err == CL_SUCCESS) {
                    debug(ndevs, "opencl::OpenCLContext::gcpus");
                    dev = OpenCLContext::GPU;
                    break;
                }
            }
        }
        if ((devtype == OpenCLContext::DEFAULT && ndevs == 0) || devtype == OpenCLContext::CPU) {
            for(cl_uint i = 0; i < nplatforms; ++i) {
                const cl_int err = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_CPU, OPENCL_MAX_DEVICES, devid, &ndevs);
                if (err == CL_SUCCESS) {
                    debug(ndevs, "opencl::OpenCLContext::ncpus");
                    dev = OpenCLContext::CPU;
                    break;
                }
            }
        }
        if (ndevs == 0)
            return;

        // Initialize OpenCL context and command queues
        cl_int ccerr;
        ctx = clCreateContext(0, ndevs, devid, NULL, NULL, &ccerr);
        if(!ctx || ccerr != CL_SUCCESS) {
            mkfailure<bool>("Couldn't create OpenCL context: " + clError(ccerr));
            return;
        }

        for (cl_uint i = 0; i < ndevs; i++) {
            cl_int cqerr;
#ifdef WANT_MAINTAINER_OPENCL_PROF
            cq[i] = clCreateCommandQueue(ctx, devid[i], CL_QUEUE_PROFILING_ENABLE, &cqerr);
#else
            cq[i] = clCreateCommandQueue(ctx, devid[i], 0, &cqerr);
#endif
            if (!cq[i] || cqerr != CL_SUCCESS) {
                mkfailure<bool>("Couldn't create OpenCL command queue: " + clError(cqerr));
                return;
            }
        }
    }

    OpenCLContext(const OpenCLContext& c) : dev(c.dev), ndevs(c.ndevs), ctx(c.ctx) {
        for (cl_uint i = 0; i < ndevs; i++) {
            devid[i] = c.devid[i];
            cq[i] = c.cq[i];
            if (cq[i] != 0) {
                const cl_int rcqerr = clRetainCommandQueue(cq[i]);
                if (rcqerr != CL_SUCCESS)
                    mkfailure<bool>(string("Couldn't retain OpenCL command queue: ") + clError(rcqerr));
            }
        }
        const cl_int rcerr = clRetainContext(ctx);
        if (rcerr != CL_SUCCESS)
            mkfailure<bool>(string("Couldn't retain OpenCL context: ") + clError(rcerr));
    }

    ~OpenCLContext() {
        for (cl_uint i = 0; i < ndevs; i++) {
            if (cq[i] != 0)
                clReleaseCommandQueue(cq[i]);
        }
        if (ctx != 0)
            clReleaseContext(ctx);
    }

private:
    OpenCLContext::DeviceType dev;
    cl_uint ndevs;
    cl_device_id devid[OPENCL_MAX_DEVICES];
    cl_context ctx;
    cl_command_queue cq[OPENCL_MAX_DEVICES];

    friend const cl_uint devices(const OpenCLContext& cl);
    friend const cl_command_queue commandq(const OpenCLContext& cl);
    friend const failable<OpenCLBuffer> readOnlyBuffer(const size_t size, const void* p, const OpenCLContext& cl, cl_command_queue cq);
    friend const failable<OpenCLBuffer> writeOnlyBuffer(const size_t size, const OpenCLContext& cl);
    friend const failable<value> evalKernel(const failable<OpenCLKernel>& kernel, const value& expr, const size_t gwsize, const value::ValueType type, const size_t n, const OpenCLContext& cl);
    friend const failable<OpenCLProgram> readProgram(const string& path, istream& is, const OpenCLContext& cl);
};

/**
 * Return the number of computing devices available in a context.
 */
const cl_uint devices(const OpenCLContext& cl) {
    return cl.ndevs;
}

/**
 * Return a command queue from a context.
 */
const cl_command_queue commandq(const OpenCLContext& cl) {
    return cl.cq[0];
}

/**
 * Represents an OpenCL program.
 */
class OpenCLProgram {
public:
    OpenCLProgram() : prog(0) {
    }

    OpenCLProgram(const cl_program prog) : prog(prog) {
    }

    OpenCLProgram(const OpenCLProgram& c) : prog(c.prog) {
        const cl_int rperr = clRetainProgram(prog);
        if (rperr != CL_SUCCESS)
            mkfailure<bool>(string("Couldn't retain OpenCL program: ") + clError(rperr));
    }

    ~OpenCLProgram() {
        if (!prog)
            return;
        const cl_int rperr = clReleaseProgram(prog);
        if (rperr != CL_SUCCESS)
            mkfailure<bool>(string("Couldn't release OpenCL program: ") + clError(rperr));
    }

private:
    const cl_program prog;

    friend const failable<OpenCLKernel> createKernel(const value& expr, const OpenCLProgram& clprog);
    friend const failable<value> evalKernel(const failable<OpenCLKernel>& kernel, const value& expr, const size_t gwsize, const value::ValueType type, const size_t n, const OpenCLContext& cl);
};

/**
 * Represents an OpenCL kernel.
 */
class OpenCLKernel {
public:
    OpenCLKernel() : k(0) {
    }

    OpenCLKernel(const cl_kernel k) : k(k) {
    }

    OpenCLKernel(const OpenCLKernel& c) : k(c.k) {
        const cl_int rkerr = clRetainKernel(k);
        if (rkerr != CL_SUCCESS)
            mkfailure<bool>(string("Couldn't retain OpenCL kernel: ") + clError(rkerr));
    }

    ~OpenCLKernel() {
        if (!k)
            return;
        const cl_int rkerr = clReleaseKernel(k);
        if (rkerr != CL_SUCCESS)
            mkfailure<bool>(string("Couldn't release OpenCL kernel: ") + clError(rkerr));
    }

private:
    const cl_kernel k;

    friend const failable<OpenCLBuffer> valueToKernelArg(const cl_uint i, const size_t size, const void* p, const failable<OpenCLBuffer>& buf, const OpenCLKernel& kernel);
    friend const failable<value> evalKernel(const failable<OpenCLKernel>& kernel, const value& expr, const size_t gwsize, const value::ValueType type, const size_t n, const OpenCLContext& cl);
};

/**
 * Represents an OpenCL buffer.
 */
class OpenCLBuffer {
public:
    OpenCLBuffer() : mem(0), evt(0) {
    }

    OpenCLBuffer(const cl_mem mem, const cl_event evt) : mem(mem), evt(evt) {
    }

    OpenCLBuffer(const OpenCLBuffer& c) : mem(c.mem), evt(c.evt) {
        if (mem != 0) {
            const cl_int rmerr = clRetainMemObject(mem);
            if (rmerr != CL_SUCCESS)
                mkfailure<bool>(string("Couldn't retain OpenCL buffer: ") + clError(rmerr));
        }
        if (evt != 0) {
            const cl_int reerr = clRetainEvent(evt);
            if (reerr != CL_SUCCESS)
                mkfailure<bool>(string("Couldn't retain OpenCL event: ") + clError(reerr));
        }
    }

    ~OpenCLBuffer() {
        if (mem != 0) {
            const cl_int rmerr = clReleaseMemObject(mem);
            if (rmerr != CL_SUCCESS)
                mkfailure<bool>(string("Couldn't release OpenCL buffer: ") + clError(rmerr));
        }
        if (evt != 0) {
            const cl_int reerr = clReleaseEvent(evt);
            if (reerr != CL_SUCCESS)
                mkfailure<bool>(string("Couldn't release OpenCL event: ") + clError(reerr));
        }
    }

private:
    const cl_mem mem;
    const cl_event evt;

    friend const cl_uint writeBufferEvents(const list<OpenCLBuffer>& buf, cl_event* evt);
    friend const failable<OpenCLBuffer> valueToKernelArg(const cl_uint i, const size_t size, const void* p, const failable<OpenCLBuffer>& buf, const OpenCLKernel& kernel);
    friend const failable<value> evalKernel(const failable<OpenCLKernel>& kernel, const value& expr, const size_t gwsize, const value::ValueType type, const size_t n, const OpenCLContext& cl);
};

/**
 * Return a read-only memory buffer.
 */
const failable<OpenCLBuffer> readOnlyBuffer(const size_t size, const void* const p, const OpenCLContext& cl, const cl_command_queue cq) {
    if (cl.dev == OpenCLContext::CPU) {
        cl_int err;
        const cl_mem buf = clCreateBuffer(cl.ctx, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, size, const_cast<void*>(p), &err);
        if (!buf || err != CL_SUCCESS)
            return mkfailure<OpenCLBuffer>(string("Couldn't map OpenCL host memory: ") + clError(err));
        return OpenCLBuffer(buf, 0);
    }
    cl_int berr;
    const cl_mem buf = clCreateBuffer(cl.ctx, CL_MEM_READ_ONLY, size, NULL, &berr);
    if (!buf || berr != CL_SUCCESS)
        return mkfailure<OpenCLBuffer>(string("Couldn't allocate OpenCL device memory: ") + clError(berr));
    cl_event wevt;
    const cl_int werr = clEnqueueWriteBuffer(cq, buf, CL_FALSE, 0, size, p, 0, NULL, &wevt);
    if (werr != CL_SUCCESS) {
        clReleaseMemObject(buf);
        return mkfailure<OpenCLBuffer>(string("Couldn't enqueue write to device memory: ") + clError(werr));
    }
    return OpenCLBuffer(buf, wevt);
}

/**
 * Fill an array of write events for a given list of buffers.
 */
const cl_uint writeBufferEvents(const list<OpenCLBuffer>& buf, cl_event* const evt) {
    if (isNull(buf))
        return 0;
    const cl_event e = car(buf).evt;
    if (e == 0)
        return writeBufferEvents(cdr(buf), evt);
    *evt = e;
    return 1 + writeBufferEvents(cdr(buf), evt + 1);
}

/**
 * Return a write-only memory buffer.
 */
const failable<OpenCLBuffer> writeOnlyBuffer(const size_t size, const OpenCLContext& cl) {
    if (cl.dev == OpenCLContext::CPU) {
        cl_int err;
        const cl_mem buf = clCreateBuffer(cl.ctx, CL_MEM_WRITE_ONLY | CL_MEM_ALLOC_HOST_PTR, size, NULL, &err);
        if (!buf || err != CL_SUCCESS)
            return mkfailure<OpenCLBuffer>(string("Couldn't map OpenCL host memory: ") + clError(err));
        return OpenCLBuffer(buf, 0);
    }
    cl_int err;
    const cl_mem buf = clCreateBuffer(cl.ctx, CL_MEM_WRITE_ONLY, size, NULL, &err);
    if (!buf || err != CL_SUCCESS)
        return mkfailure<OpenCLBuffer>(string("Couldn't allocate OpenCL device memory: ") + clError(err));
    return OpenCLBuffer(buf, 0);
}

/**
 * Convert a value to a kernel arg.
 */
const failable<OpenCLBuffer> valueToKernelArg(const cl_uint i, const size_t size, const void* const p, const failable<OpenCLBuffer>& buf, const OpenCLKernel& kernel) {
    if (!hasContent(buf))
        return buf;
    if (p != NULL) {
        const cl_int err = clSetKernelArg(kernel.k, (cl_uint)i, size, p);
        if (err != CL_SUCCESS)
            return mkfailure<OpenCLBuffer>(string("Couldn't set OpenCL simple kernel arg: ") + clError(err));
        return buf;
    }
    const OpenCLBuffer b = content(buf);
    const cl_int err = clSetKernelArg(kernel.k, i, sizeof(cl_mem), &b.mem);
    if (err != CL_SUCCESS)
        return mkfailure<OpenCLBuffer>(string("Couldn't set OpenCL buffer kernel arg: ") + clError(err));
    return buf;
}

const failable<OpenCLBuffer> valueToKernelArg(const value& v, const cl_uint i, const OpenCLKernel& kernel, const OpenCLContext& cl, const cl_command_queue cq) {
    switch (type(v)) {
    case value::Symbol: {
        const string s = string("'") + (string)v;
        return valueToKernelArg(i, 0, NULL, readOnlyBuffer(length(s) + 1, c_str(s), cl, cq), kernel);
    }
    case value::String: {
        const string s = (string)v;
        return valueToKernelArg(i, 0, NULL, readOnlyBuffer(length(s) + 1, c_str(s), cl, cq), kernel);
    }
    case value::Number: {
        const cl_float d = (cl_float)((double)v);
        return valueToKernelArg(i, sizeof(cl_float), &d, OpenCLBuffer(), kernel);
    }
    case value::Bool: {
        const cl_int b = (cl_int)((bool)v);
        return valueToKernelArg(i, sizeof(cl_int), &b, OpenCLBuffer(), kernel);
    }
    default: {
        return valueToKernelArg(i, sizeof(cl_mem), NULL, readOnlyBuffer(sizeof(value), &v, cl, cq), kernel);
    }
    }
}

/**
 * Convert a list of values to kernel args.
 */
const failable<list<OpenCLBuffer>> valuesToKernelArgsListHelper(const list<value>& v, const cl_uint i, const OpenCLKernel& kernel, const OpenCLContext& cl, const cl_command_queue cq) {
    if (isNull(v))
        return list<OpenCLBuffer>();
    const failable<OpenCLBuffer> a = valueToKernelArg(car(v), i, kernel, cl, cq);
    if (!hasContent(a))
        return mkfailure<list<OpenCLBuffer>>(a);
    const failable<list<OpenCLBuffer>> al = valuesToKernelArgsListHelper(cdr(v), i + 1, kernel, cl, cq);
    if (!hasContent(al))
        return al;
    return cons<OpenCLBuffer>(content(a), content(al));
}

const failable<list<OpenCLBuffer>> valuesToKernelArgs(const list<value>& v, const OpenCLKernel& kernel, const OpenCLContext& cl, const cl_command_queue cq) {
    return valuesToKernelArgsListHelper(v, 0, kernel, cl, cq);
}

/**
 * Convert a kernel result to a value.
 */
const value kernelResultToValue(const void* const p, const value::ValueType type) {
    switch(type) {
    case value::Symbol: {
        const char* const s = (const char*)p;
        const size_t l = strlen(s);
        if (l != 0 && *s == '\'')
            return value(s + 1);
        return value(s);
    }
    case value::String: {
        const char* const s = (const char*)p;
        const size_t l = strlen(s);
        if (l != 0 && *s == '\'')
            return value(s + 1);
        return value(string(s, l));
    }
    case value::Number:
        return (double)(*(const cl_float*)p);
    case value::Bool:
        return (bool)(*(const cl_int*)p);
    default:
        return *(const value*)p;
    }
}

/**
 * Return the value type corresponding to a C99 type name.
 */
const value::ValueType valueType(const string& t) {
    if (t == "float")
        return value::Number;
    if (t == "int")
        return value::Bool;
    if (t == "char")
        return value::String;
    return value::Nil;
}

/**
 * Return the size of a C99 type corresponding to a value type.
 */
const size_t valueSize(const value::ValueType type) {
    switch(type) {
    case value::Number:
        return sizeof(cl_float);
    case value::Bool:
        return sizeof(cl_int);
    case value::Symbol:
        return sizeof(cl_char);
    case value::String:
        return sizeof(cl_char);
    default:
        return sizeof(value);
    }
}

/**
 * Return the result type of a kernel.
 */
class OpenCLResultType {
public:
    OpenCLResultType(const value::ValueType type, const size_t n, const size_t size) : type(type), n(n), size(size) {}
    const value::ValueType type;
    const size_t n;
    const size_t size;
};

const OpenCLResultType kernelResultType(const string& fn, value::ValueType type, const size_t n) {
    if (type != value::Nil)
        return OpenCLResultType(type, n, valueSize(type));
    const string s = car(tokenize("_", fn));
    const size_t d = find_first_of(s, "0123456789");
    if (d == length(s)) {
        const value::ValueType vt = valueType(s);
        return OpenCLResultType(vt, 1, valueSize(vt));
    }
    const value::ValueType vt = valueType(substr(s, 0, d));
    return OpenCLResultType(vt, atoi(c_str(substr(s, d))), valueSize(vt));
}

/**
 * Create the kernel implementing an expression.
 */
const failable<OpenCLKernel> createKernel(const value& expr, const OpenCLProgram& clprog) {

    // Create an OpenCL kernel for the requested function
    const value fn = car<value>(expr);
    cl_int ckerr;
    const cl_kernel k = clCreateKernel(clprog.prog, c_str(fn), &ckerr);
    if (k == NULL || ckerr != CL_SUCCESS) {

        // The start, stop, and restart functions are optional
        //if (fn == "start" || fn == "stop")
            //return value(lvvlambda());

        return mkfailure<OpenCLKernel>(string("Couldn't find function: ") + (string)car<value>(expr) + " : " + clError(ckerr));
    }
    return OpenCLKernel(k);
}

/**
 * Evaluate an expression implemented by a kernel.
 */
const failable<value> evalKernel(const failable<OpenCLKernel>& fkernel, const value& expr, const size_t gwsize, const value::ValueType type, const size_t n, const OpenCLContext& cl) {

#ifdef WANT_MAINTAINER_OPENCL_PROF
    const cl_uint estart = (cl_uint)timens();
    const cl_uint pstart = estart;
#endif

    if (!hasContent(fkernel))
        return mkfailure<value>(fkernel);
    const OpenCLKernel kernel = content(fkernel);

    // Get a command queue for the specified device type
    const cl_command_queue cq = commandq(cl);

    // Set the kernel input args
    const failable<list<OpenCLBuffer>> args = valuesToKernelArgs(cdr<value>(expr), kernel, cl, cq);
    if (!hasContent(args)) {
        return mkfailure<value>(args);
    }

    // Allocate result buffer in device memory
    const value fn = car<value>(expr);
    const OpenCLResultType rtype = kernelResultType(fn, type, n);
    const size_t rsize = rtype.n * rtype.size;
    const failable<OpenCLBuffer> rbuf = writeOnlyBuffer(rsize, cl);
    if (!hasContent(rbuf))
        return mkfailure<value>(rbuf);

    // Set it as a kernel output arg
    const cl_mem rmem = content(rbuf).mem;
    const failable<OpenCLBuffer> rarg = valueToKernelArg((cl_uint)length(cdr<value>(expr)), sizeof(cl_mem), &rmem, rbuf, kernel);
    if (!hasContent(rarg))
        return mkfailure<value>(rarg);

    // Enqueue the kernel, to be executed after all the writes complete
    cl_event wevt[32];
    const cl_uint nwevt = writeBufferEvents(content(args), wevt);
    cl_event kevt;
    const cl_int qerr = clEnqueueNDRangeKernel(cq, kernel.k, 1, NULL, &gwsize, NULL, nwevt, nwevt != 0? wevt : NULL, &kevt);
    if (qerr != CL_SUCCESS)
        return mkfailure<value>(string("Couldn't enqueue kernel task: ") + clError(qerr));

    // Enqueue result buffer read, to be executed after the kernel completes
    char res[rsize];
    cl_event revt;
    const cl_int rerr = clEnqueueReadBuffer(cq, rmem, CL_FALSE, 0, rsize, res, 1, &kevt, &revt);  
    if (rerr != CL_SUCCESS) {
        clReleaseEvent(kevt);
        return mkfailure<value>(string("Couldn't read from OpenCL device memory: ") + clError(rerr));
    }

#ifdef WANT_MAINTAINER_OPENCL_PROF
    const cl_uint pend = (cl_uint)timens();
    preptime += (pend - pstart);
#endif

    // Wait for completion
    const cl_int werr = clWaitForEvents(1, &revt);
    if (werr != CL_SUCCESS) {
        clReleaseEvent(revt);
        clReleaseEvent(kevt);
        return mkfailure<value>(string("Couldn't wait for kernel completion: ") + clError(werr));
    }

#ifdef WANT_MAINTAINER_OPENCL_PROF
    profileMemEvents(nwevt, wevt);
    profileKernelEvent(kevt);
    profileMemEvent(revt);
#endif

    // Convert the result to a value
    const value v = kernelResultToValue(res, rtype.type);

    // Release OpenCL resources
    clReleaseEvent(revt);
    clReleaseEvent(kevt);

#ifdef WANT_MAINTAINER_OPENCL_PROF
    const cl_uint eend = (cl_uint)timens();
    evaltime += (eend - estart);
#endif

    return v;
}

const failable<value> evalKernel(const failable<OpenCLKernel>& kernel, const value& expr, const OpenCLContext& cl) {
    return evalKernel(kernel, expr, 1, value::Nil, 0, cl);
}

/**
 * Read an opencl program from an input stream.
 */
const failable<OpenCLProgram> readProgram(const string& path, istream& is, const OpenCLContext& cl) {

    // Read the program source
    const list<string> ls = streamList(is);
    ostringstream os;
    write(ls, os);
    const char* cs = c_str(str(os));

    // Create the OpenCL program
    cl_int cperr;
    const cl_program prog = clCreateProgramWithSource(cl.ctx, 1, (const char **)&cs, NULL, &cperr);
    if (!prog || cperr != CL_SUCCESS)
        return mkfailure<OpenCLProgram>(string("Couldn't create OpenCL program from source: ") + path + " : " + clError(cperr));

    // Built it
    const cl_int bperr = clBuildProgram(prog, 0, NULL, NULL, NULL, NULL);
    if(bperr != CL_SUCCESS) {
        size_t l;
        char b[2048];
        clGetProgramBuildInfo(prog, cl.devid[0], CL_PROGRAM_BUILD_LOG, sizeof(b), b, &l);
        return mkfailure<OpenCLProgram>(string("Couldn't build OpenCL program: ") + path + " : " + clError(bperr) + "\n" + string(b));
    }
    return OpenCLProgram(prog);
}

/**
 * Evaluate an expression against an OpenCL program provided as an input stream.
 */
const failable<value> evalKernel(const value& expr, istream& is, const OpenCLContext& cl) {
    failable<OpenCLProgram> clprog = readProgram("program.cl", is, cl);
    if (!hasContent(clprog))
        return mkfailure<value>(clprog);
    return evalKernel(createKernel(expr, content(clprog)), expr, 1, value::Nil, 0, cl);
}

}
}
#endif /* tuscany_opencl_eval_hpp */
