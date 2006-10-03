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

#ifndef tuscany_sca_util_file_h
#define tuscany_sca_util_file_h

#include <string>
using std::string;
#include <vector>
using std::vector;

namespace tuscany
{
    namespace sca
    {
        /**
         * File access methods. Provides platform independent
         * access to files.
         */
        class File
        {
        public:
            /**
             * Constructor.
             * @param directory Name of the directory in which this file is located.
             * Either / or \ can be used interchangeably for separating directory elements.
             * @param fileName Name of the file in the dirctory.
             */
            File(const string& directory, const string& fileName);

            /**
             * Destructor.
             */ 
            virtual ~File();

            /**
             * Return the directory in which this file is located.
             * @return Name of the directory.
             */
            const string& getDirectory() const {return directory;}

            /**
             * Name of the file.
             * @return Name of the file.
             */
            const string& getFileName() const {return fileName;}
        private:
            /**
             * Name of the directory.
             */
            string directory;

            /**
             * Name of the file.
             */ 
            string fileName;
        };
        
        /**
         * Collection of File to provide platform independent access
         * to files and directories.
         */
        class Files
        {
        public:
            /**
             * Constructor which will search a given directory with a pattern and return a 
             * new instance of this collection class.
             * @param rootDirectory The directory in which to search.
             * @param pattern A pattern for matching file names. Can include * and ?.
             * @param subdirectories Whether subdirectories should be searched too.
             */
            Files(const string& rootDirectory, const string& pattern, bool subdirectories = false);

            /**
             * Destructor.
             */ 
            virtual ~Files();

            /**
             * Return the number of files found.
             * @return The number of files found.
             */
            unsigned int size();

            /**
             * Return a File at this position in the collection.
             * @param index The index into the collection.
             * @return The File at this index in the collection.
             */
            const File& operator[] (unsigned int index);

        private:
            /**
             * Search the given directory and pattern for matching files.
             * @param rootDirectory The directory in which to search.
             * @param pattern A pattern for matching file names. Can include * and ?.
             * @param subdirectories Whether subdirectories should be searched too.
             */
            void findFiles(const string& rootDirectory, const string& pattern, bool subdirectories);
            
            /**
             * The top level directory to search.
             */ 
            string rootDirectory;

            typedef vector<File> FILES;

            /**
             * Vector of File.
             */ 
            FILES files;
        };
        
    } // End namespace sca
} // End namespace tuscany
#endif // tuscany_sca_util_file_h
