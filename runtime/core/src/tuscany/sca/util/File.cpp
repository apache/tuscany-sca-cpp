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

#if defined(WIN32)  || defined (_WINDOWS)
#pragma warning(disable: 4786)
#endif

#include "tuscany/sca/util/File.h"
#include "tuscany/sca/util/Utils.h"
#include "tuscany/sca/util/Exceptions.h"
#include "tuscany/sca/util/Logging.h"
#include <iostream>

#include <string>
#include <iostream>
using namespace std;

#if defined(WIN32)  || defined (_WINDOWS)
#include <windows.h> 
#else
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#endif

namespace tuscany
{
    namespace sca
    {
        File::File(const string& dir, const string& file)
            : directory(dir), fileName(file)
        {
        }
        File::~File()
        {
        }
        
        
        Files::Files(const string& rootDir, const string& pattern, bool subdirectories)
            : rootDirectory(rootDir)
        {
            findFiles(rootDirectory, pattern, subdirectories);
        }
        
        Files::~Files()
        {
        }
        
        unsigned int Files::size()
        {
            return files.size();
        }
        
        const File& Files::operator[] (unsigned int index)
        {
            if (size() <= index)
            {
                throwException(SystemConfigurationException, "Index of of bounds");
            }
            
            FILES::iterator iter = files.begin();
            for (unsigned int i=0; i<index; i++)
            {
                iter++;
            }
            
            return *iter;
        }
        
        
        void Files::findFiles(const string& rootDir, const string& pattern, bool subdirectories)
        {
            
#if defined(WIN32)  || defined (_WINDOWS)
            char currentDir[ _MAX_FNAME];
            
            
            GetCurrentDirectory(_MAX_FNAME, currentDir);
            
            // Set current directory, from which to search.
            if (!SetCurrentDirectory(rootDir.c_str()))
            {
                cout << "Unable to set current directory to: " << rootDir.c_str() << endl;
                return;
            }
            char fullDirname[ _MAX_FNAME];
            GetCurrentDirectory(_MAX_FNAME, fullDirname);
            
            // First, look for all files in this directory that meet the pattern
            char search[ _MAX_FNAME];
            strcpy(search, pattern.c_str());
            
            // Find the first file in the directory
            WIN32_FIND_DATA data;
            HANDLE searchHandle = FindFirstFile(search, &data);
            
            int more = TRUE;
            if (searchHandle != INVALID_HANDLE_VALUE)
            {
                // Found some matching files, so call the function with the details of each one
                while (more)
                {
                    // Skip over directories
                    if (!(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
                    {
                        // Add the file to our list
                        files.push_back(File(fullDirname, data.cFileName));
                    }
                    
                    more = FindNextFile(searchHandle, &data);
                }
            }
            
            
            if (!subdirectories)
                return;
            
            // Now recurse down all the directories
            // Find the first file in the directory
            searchHandle = FindFirstFile( "*.*", &data);
            more = TRUE;
            
            if (searchHandle != INVALID_HANDLE_VALUE)
            {
                // Found some files in the directory.
                while (more)
                {
                    // If directory
                    if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                    {                        
                        // Skip over '.' and '..'
                        if ((strcmp(data.cFileName, ".")) && (strcmp("..", data.cFileName)))
                        {
                            // Recurse
                            findFiles(data.cFileName, pattern, subdirectories);
                        }
                    }
                    
                    more = FindNextFile(searchHandle, &data);
                }
            }
            SetCurrentDirectory(currentDir);
#else
            // Linux
            //char fullDirname[MAX_PATH];
            //getcwd(fullDirname, MAX_PATH);
            
            DIR* root = opendir(rootDir.c_str());
            if (!root)
            {
                logwarning("Unable to open directory: %s", rootDir.c_str());
                return;
            }
            
            bool exactMatch = true;
            string token1, token2;
            if (pattern.find('*') != string::npos)
            {
                exactMatch = false;
                Utils::tokeniseString("*", pattern, token1, token2);
            }
            
            struct dirent *entry=0;
            while ((entry = readdir(root)))
            {
                string entryName = rootDir + "/" + entry->d_name;
                struct stat statbuf;
                if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                    continue;
                if (stat(entryName.c_str(), &statbuf)  != 0)
                {
                    logwarning("Unable to stat entry: %s", entryName.c_str());
                }
                else
                {
                    if (S_ISDIR(statbuf.st_mode))
                    {
                        if (subdirectories)
                        {
                            findFiles(entryName, pattern, subdirectories);
                        }
                    }
                    else if (S_ISREG(statbuf.st_mode))
                    {
                        string filename = entry->d_name;
                
                        if ((exactMatch && filename == pattern) ||
                            (!exactMatch &&
                             ((filename.find(token1) == 0)
                             && (filename.length() >= token2.length())
                             && (filename.rfind(token2) == (filename.length() - token2.length())) )))
                        {
                            // Add the file to our list
                            files.push_back(File(rootDir, filename));
                        }
                    }
                }            
            }
            closedir(root);
#endif
        }
        
        
    } // End namespace sca
} // End namespace tuscany


