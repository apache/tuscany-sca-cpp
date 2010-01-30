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

package test;

import java.util.List;
import java.util.ArrayList;
import org.apache.tuscany.Service;

public class CalcImpl {
    
    public Double add(Double x, Double y, Adder adder) {
        return adder.add(x, y);
    }
    
    public Double addEval(Double x, Double y, Service adder) {
        return adder.eval("add", x, y);
    }
    
    public Double mult(Double x, Double y) {
        return x * y;
    }
    
    public Boolean even(Double x) {
        return (double)(((int)(double)x / 2) * 2) == (double)x;
    }
    
    public Iterable<Double> square(Iterable<Double> l) {
        List r = new ArrayList();
        for (Double x: l)
            r.add(x * x);
        return r;
    }
    
}
