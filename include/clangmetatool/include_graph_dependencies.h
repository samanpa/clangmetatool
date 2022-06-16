#ifndef INCLUDED_CLANGMETATOOL_INCLUDE_GRAPH_DEPENDENCIES_H
#define INCLUDED_CLANGMETATOOL_INCLUDE_GRAPH_DEPENDENCIES_H

#include <clangmetatool/collectors/include_graph_data.h>

namespace clangmetatool {


/**
 * Collect stateless functions to query and and modify the state of
 * dependencies of a given `clangmetatool::IncludeGraphData` structure.
 */
struct IncludeGraphDependencies {
  /**
   * Decrement the reference count tracked for this edge, notifying the caller
   * if the operation was successful.
   *
   * Return a boolean if indicating we were able to decrement the count:
   *  - false if the edge doesn't exist or has no balance to decrement
   *  - true otherwise. This will always reduce the count by 1
   */
  static bool
  decrementUsageRefCount(clangmetatool::collectors::IncludeGraphData *data,
                         const clangmetatool::types::FileGraphEdge &edge);


  /**
   * Collect all direct and transitive includes of a file without checking
   * whether or not the initial file actually depends on them.
   */
  static std::set<clangmetatool::types::FileUID>
  collectAllIncludes(const clangmetatool::collectors::IncludeGraphData* data,
                     const clangmetatool::types::FileUID &fileFUID);


  /**
   * Get the 'live' dependencies of a header within the given include graph.
   * This is a subset of the set of transitive dependencies.
   *
   * Header B is called a 'live' dependency of B if A uses or references a
   * name originating from B or any of its transitive dependencies, i.e.:
   * - A references a macro provided through B
   * - A references a variable declaration provided through B
   * - A references a type declaration provided through B
   * - A redeclares a named declaration in B.
   */
  static std::set<clangmetatool::types::FileUID>
  liveDependencies(const clangmetatool::collectors::IncludeGraphData *data,
                   const clangmetatool::types::FileUID &fileFUID);

  /*
   * A data structure for include graph weak dependencies analysis
   * for a specific source file
   *
   * Key: all headers the file directly depends on
   * Value: a set of direct included headers that could allow the file to
   * access the key header
   *
   * Example:
   * { "def1.h": {"a.h", "b.h"},
   *   "def2.h": {"a.h", "c.h"} }
   *
   * So that given the example data above it means current analyzing file:
   * - depends on definitions from "def1.h", "def2.h" directly
   * - includes "a.h", "b.h", "c.h"
   * - can access "def1.h" by "a.h" and "b.h"
   * - can access "def2.h" by "a.h" and "c.h"
   */
  typedef std::map<clangmetatool::types::FileUID,
                   std::set<clangmetatool::types::FileUID>> DirectDependenciesMap;

  /**
   * Get the 'live' weak dependencies of a header within the given include graph.
   * This will return data structure(called DirectDependenciesMap) shows weak dependencies of headers
   *
   * (See docstring of DirectDependenciesMap typedef above for more details of the data structure)
   */
  static DirectDependenciesMap
  liveWeakDependencies(const clangmetatool::collectors::IncludeGraphData *data,
                       const clangmetatool::types::FileUID &fileFUID);

}; // struct IncludeGraphDependencies
} // namespace clangmetatool

#endif

// ----------------------------------------------------------------------------
// Copyright 2018 Bloomberg Finance L.P.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// ----------------------------- END-OF-FILE ----------------------------------
